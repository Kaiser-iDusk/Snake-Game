#define is_down(b) input->buttons[b].isDown
#define pressed(b) (input->buttons[b].isDown & input->buttons[b].isChanged)
#define released(b) (!input->buttons[b].isDown & input->buttons[b].isChanged)

struct Information {
    float pastX, pastY, xHalfWidth = 0.5f, yHalfWidth = 0.5f, speed = 10.0f, collision_speed = 3.0f;
    int rewardX, rewardY, gotReward = 1;
};

struct Information* info = new Information;

internal void generate_random(int* x, int mini, int maxi) {
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<int> distr(mini, maxi);
    *x = distr(gen);
}

internal void simulate_game(struct Input* input, float delta) {
    clear_scr();

    // BORDERS
    create_rect(0, 0, 5, render.height, 0x99ccff);
    create_rect(0, 0, render.width, 5, 0x99ccff);
    create_rect(render.width - 5, 0, render.width, render.height, 0x99ccff);
    create_rect(0, render.height - 5, render.width, render.height, 0x99ccff);

    // CHECK REWARD GOT?
    if (info->gotReward) {
        generate_random(&(info->rewardX), 6, render.width - 6);
        generate_random(&(info->rewardY), 6, render.height - 6);
        count++;

        info->gotReward = 0;
    }

    // DRAW REWARD
    create_fruit(info->rewardX, info->rewardY, 0.6f, 0.6f, 0xffff00);

    if (snake.selfCollision()) {
        running = false;
        return;
    }

    float default_speed = 0.5f, delta_speed = 0.9f;
    info->pastX = snake.head().x, info->pastY = snake.head().y;

        //run = false;
    if (pressed(BUTTON_UP) || is_down(BUTTON_UP)) {
        runner = false;
        snake.go_up(delta, default_speed);
    }
    if (pressed(BUTTON_RIGHT) || is_down(BUTTON_RIGHT)) {
        runner = false;
        snake.go_right(delta, default_speed);
    }
    if (pressed(BUTTON_LEFT) || is_down(BUTTON_LEFT)) {
        runner = false;
        snake.go_left(delta, default_speed);
    }
    if (pressed(BUTTON_DOWN) || is_down(BUTTON_DOWN)) {
        runner = false;
        snake.go_down(delta, default_speed);
    }

    char head_direction = snake.head().dir;

    if (head_direction == 'U' && runner) {
        snake.go_up(delta, default_speed);
    }
    if (head_direction == 'R' && runner) {
        snake.go_right(delta, default_speed);
    }
    if (head_direction == 'L' && runner) {
        snake.go_left(delta, default_speed);
    }
    if (head_direction == 'D' && runner) {
        snake.go_down(delta, default_speed);
    }


    float snakeX = snake.head().x, snakeY = snake.head().y, halfSide = snake.head().half_side;
    int collided = collision_handler(snakeX, snakeY, halfSide, halfSide, info->rewardX, info->rewardY, 0.1f, 0.1f);
    if (collided == 1) {
        score++;
        info->gotReward = 1;
        int cap = snake.rewarded();
        int cap1 = snake.rewarded();
        if (cap == -1 || cap1 == -1) {
            running = false;
        }
    }
    else if (collided == -1) {
        running = false;
        if(info->pastX > snake.head().x)
            snake.go_right(1.f, info->collision_speed);
        else
            snake.go_left(1.f, info->collision_speed);
    }
    else if (collided == -2) {
        running = false;
        if(info->pastY > snake.head().y)
            snake.go_up(1.f, info->collision_speed);
        else
            snake.go_down(1.f, info->collision_speed);
    }

    snake.drawSnake(0xff0000);
    //rel_square(player_pos_x, player_pos_y, xHalfWidth, yHalfWidth, 0xff0000);
}
