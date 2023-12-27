internal void render_background() {
    u32* pixel = (u32*)(render.memory);
    for (int i = 0; i < render.height; i++) {
        for (int j = 0; j < render.width; j++) {
            *pixel++ = 0xb33b00 + i * j;
        }
    }
}

internal void clear_scr() {
    u32* pixel = (u32*)(render.memory);
    for (int i = 0; i < render.height; i++) {
        for (int j = 0; j < render.width; j++) {
            *pixel++ = 0x000000;
        }
    }
}

internal void clear_scr(u32 color) {
    u32* pixel = (u32*)(render.memory);
    for (int i = 0; i < render.height; i++) {
        for (int j = 0; j < render.width; j++) {
            *pixel++ = color;
        }
    }
}

internal void create_rect(int x0, int y0, int x1, int y1, u32 color) {
    x0 = clamp(0, x0, render.width);
    y0 = clamp(0, y0, render.height);
    x1 = clamp(0, x1, render.width);
    y1 = clamp(0, y1, render.height);

    for (int y = y0; y < y1; y++) {
        u32* pixel = (u32*) render.memory + y * (render.width);
        for (int x = x0; x < x1; x++) {
            *(pixel + x) = color;
        }
    }
}

internal void rel_rect(float x, float y, float half_x, float half_y, u32 color) {
    
    // UNIT SIMILARITY
    x *= (render.width * render_scale);
    y *= (render.height * render_scale);
    half_x *= (render.width * render_scale);
    half_y *= (render.height * render_scale);

    // CENTERING
    x += (render.width / 2.0f);
    y += (render.height / 2.0f);

    int x0 = x - half_x, y0 = y - half_y, x1 = x + half_x, y1 = y + half_y;
    create_rect(x0, y0, x1, y1, color);
}

internal void rel_square(float x, float y, float half_x, float half_y, u32 color) {

    // UNIT SIMILARITY
    x *= (render.width * render_scale);
    y *= (render.width * render_scale);
    half_x *= (render.width * render_scale);
    half_y *= (render.width * render_scale);

    // CENTERING
    x += (render.width / 2.0f);
    y += (render.height / 2.0f);

    int x0 = x - half_x, y0 = y - half_y, x1 = x + half_x, y1 = y + half_y;
    create_rect(x0, y0, x1, y1, color);
}

internal void create_fruit(int x, int y, float half_x, float half_y, u32 color) {
    float halfX = (render.width * half_x * render_scale), halfY = (render.width * half_y * render_scale);
    int x0 = x - (int)halfX;
    int y0 = y - (int)halfY;
    int x1 = x + (int)halfX;
    int y1 = y + (int)halfY;

    create_rect(x0, y0, x1, y1, color);
}