#include "inputbox.h"
#include<iostream>
#include<string>
#include<random>
#include<windows.h>
#include<queue>
#include<fstream>
#include "utils.cpp"

global_variable bool running = true, runner = false;
global_variable float render_scale = 0.01f;
global_variable int score = 0;
global_variable long int count = 0;
std::priority_queue<std::pair<int, std::string>> data_queue;

struct RenderState {
    void* memory;
    int height;
    int width;
    BITMAPINFO bmi;
};

global_variable RenderState render;
#include "renderer.cpp"
#include "platform_specific.cpp"
#include "collision.cpp"
#include "Snake.cpp"

global_variable Snake snake;

#include "game.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
    case WM_DESTROY: {
        running = false;
    }break;

    case WM_SIZE: {
        RECT rect;
        GetClientRect(hwnd, &rect);
        render.height = rect.bottom - rect.top;
        render.width = rect.right - rect.left;

        int size = render.height * render.width * sizeof(unsigned int);

        if (render.memory) VirtualFree(render.memory, 0, MEM_RELEASE);
        render.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        render.bmi.bmiHeader.biSize = sizeof(render.bmi.bmiHeader);
        render.bmi.bmiHeader.biWidth = render.width;
        render.bmi.bmiHeader.biHeight = render.height;
        render.bmi.bmiHeader.biBitCount = 32;
        render.bmi.bmiHeader.biCompression = BI_RGB;
        render.bmi.bmiHeader.biPlanes = 1;

    }break;
    
    case WM_CLOSE:

    default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    //CREATE WINDOW CLASS
    WNDCLASS wc = { };
    wc.lpfnWndProc = window_callback;
    wc.lpszClassName = L"My First Game";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    //REGISTER CLASS
    RegisterClass(&wc);

    // ASK FOR PLAYER NAME
    char Prompt[17] = "Enter your name:";
    char Title[11] = "Snake Game";
    char Default[5] = "NAME";
    char* inputBox = InputBox(Prompt, Title, Default);

    // START THE GAME
    start_game();
    read_scores(data_queue);

    //CREATE WINDOW
    HWND hwnd = CreateWindowW(wc.lpszClassName, L"Game Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 920, 520, 0, 0, hInstance, 0);
    HDC hdc = GetDC(hwnd);
    
    if (hwnd == NULL) {
        std::cout << "Error: Failed to create window.\n";
        return 0;
    }

    MSG message = {};
    Input input = {};

    float delta = 0.0166667f; // fps conversion without using perfromance counter. default fps = 60.
    float perf_freq;
    {
        LARGE_INTEGER perfreq;
        QueryPerformanceFrequency(&perfreq);
        perf_freq = perfreq.QuadPart;
    }

    LARGE_INTEGER frame_begin;
    QueryPerformanceCounter(&frame_begin);
    
    while (running) {
        for (int i = 0; i < BUTTON_COUNT; i++) {
            input.buttons[i].isChanged = false;
            input.buttons[i].isDown = false;
        }

        //INPUT
        BOOL bRet;
        if (bRet = GetMessage(&message, hwnd, 0, 0) != 0) {
            if (bRet == -1) {
                printf("Error: Failed Message Exception!\n");
                return 0;
            }

            if (message.message == WM_KEYDOWN) {
                runner = false;
                u32 vk_code = (u32)message.wParam;
                bool isDown = ((message.lParam & (1 << 31)) == 0);

                switch (vk_code) {
                case VK_UP: {
                    input.buttons[BUTTON_UP].isChanged = (isDown != input.buttons[BUTTON_UP].isDown);
                    input.buttons[BUTTON_UP].isDown = isDown;
                }break;
                case VK_LEFT: {
                    input.buttons[BUTTON_LEFT].isChanged = (isDown != input.buttons[BUTTON_LEFT].isDown);
                    input.buttons[BUTTON_LEFT].isDown = isDown;
                }break;
                case VK_RIGHT: {
                    input.buttons[BUTTON_RIGHT].isChanged = (isDown != input.buttons[BUTTON_RIGHT].isDown);
                    input.buttons[BUTTON_RIGHT].isDown = isDown;
                }break;
                case VK_DOWN: {
                    input.buttons[BUTTON_DOWN].isChanged = (isDown != input.buttons[BUTTON_DOWN].isDown);
                    input.buttons[BUTTON_DOWN].isDown = isDown;
                }break;
                default: {
                    runner = !isDown;
                }
                }
            }
            if (message.message == WM_KEYUP) {
                runner = true;
            }
            if((message.message == WM_CLOSE) || (message.message == WM_DESTROY) || (message.message == WM_SIZE)) {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
        else {
            return 0;
        }

        //SIMULATE
        simulate_game(&input, delta);

        //RENDER
        StretchDIBits(hdc, 0, 0, render.width, render.height, 0, 0, render.width, render.height, render.memory, &(render.bmi), DIB_RGB_COLORS, SRCCOPY);
    
        LARGE_INTEGER frame_end;
        QueryPerformanceCounter(&frame_end);

        float delta = (float)(frame_end.QuadPart - frame_begin.QuadPart) / perf_freq;
        frame_begin = frame_end;

        Sleep(50);
    }

    if (!running) {
        std::string s = "Congratulations! Your score: " + std::to_string(score);
        MessageBoxA(NULL, s.c_str(), "Game Over", MB_ICONINFORMATION);
    }

    std::string p_name = char_to_string(inputBox);
    std::pair<int, std::string> package = make_pair(score, p_name);
    update_game(package);
    make_highscores(data_queue, 5);

    std::string out = "";
    give_leaderboard(out);

    MessageBoxA(NULL, out.c_str(), "HIGHSCORE", MB_ICONINFORMATION);

    return 0;
}