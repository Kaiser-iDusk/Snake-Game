static int MAX_LENGTH = 100;
struct Block {
	float x, y;
	float half_side;
	char dir;
};
class Snake {
private:
	Block* body;
	int length;
	float width, height, speed;
public:
	Snake() {
		length = 1;
		body = new Block[MAX_LENGTH];
		body[0].x = 0.0f;
		body[0].y = 0.f;
		width = 0.5f;
		height = 0.5f;
		body[0].half_side = height;
		body[0].dir = 'R';
		speed = 0;
	}
	int rewarded() {
		length++;
		if (length == MAX_LENGTH) {
			return -1;
		}
		body[length - 1] = body[length - 2];
		if (body[0].dir == 'R') {
			body[length - 1].x = body[length - 2].x - (2 * body[length - 2].half_side);
		}
		if (body[0].dir == 'L') {
			body[length - 1].x = body[length - 2].x + (2 * body[length - 2].half_side);
		}
		if (body[0].dir == 'U') {
			body[length - 1].y = body[length - 2].y + (2 * body[length - 2].half_side);
		}
		if (body[0].dir == 'D') {
			body[length - 1].y = body[length - 2].y - (2 * body[length - 2].half_side);
		}
		return 1;
	}
	Block head() {
		return body[0];
	}

	void go_up(float delta, float spd) {
		if (body[0].dir == 'D') {
			return;
		}
		for (int i = length-1; i >= 1; i--) {
			body[i] = body[i - 1];
		}
		body[0].dir = 'U';
		speed = spd * delta;
		body[0].y += (spd);
	}

	void go_down(float delta, float spd) {
		if (body[0].dir == 'U') {
			return;
		}
		for (int i = length - 1; i >= 1; i--) {
			body[i] = body[i - 1];
		}
		body[0].dir = 'D';
		speed = spd * delta;
		body[0].y -= (spd);
	}

	void go_left(float delta, float spd) {
		if (body[0].dir == 'R') {
			return;
		}
		for (int i = length - 1; i >= 1; i--) {
			body[i] = body[i - 1];
		}
		body[0].dir = 'L';
		speed = spd * delta;
		body[0].x -= (spd);
	}

	void go_right(float delta, float spd) {
		if (body[0].dir == 'L') {
			return;
		}
		for (int i = length - 1; i >= 1; i--) {
			body[i] = body[i - 1];
		}
		body[0].dir = 'R';
		speed = spd * delta;
		body[0].x += (spd);
	}

	bool selfCollision() {
		Block head = body[0];
		float headX = head.x, headY = head.y, error = head.half_side;

		for (int i = 1; i < length; i++) {
			Block pixel = body[i];
			if (abs(headX - pixel.x) <= error / 2 && abs(headY - pixel.y) <= error / 2) {
				return 1;
			}
		}
		return 0;
	}

	void drawSnake(u32 color) {
		//rel_square(body[0].x, body[0].y, body[0].half_side / 2, body[0].half_side / 2, 0x5742c2);
		for (int i = 0; i < length; i++) {
			rel_square(body[i].x, body[i].y, body[i].half_side, body[i].half_side, color);
		}
	}
};