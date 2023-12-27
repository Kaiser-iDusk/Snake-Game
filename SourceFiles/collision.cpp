internal int collision_handler(float player_pos_x, float player_pos_y, float xHalfWidth, float yHalfWidth, int rewardX, int rewardY, float wid, float hgt) {
	float rX = (rewardX - (render.width / 2.f)), pX = (render.width * player_pos_x * render_scale); //+ (render.width / 2.f);
	float rY = (rewardY - (render.height / 2.f)), pY = (render.width * player_pos_y * render_scale); //+(render.height / 2.f);
	float hwpX = (render.width * xHalfWidth * render_scale), hhpY = (render.width * yHalfWidth * render_scale);
	float hwrX = (render.width * wid) * render_scale, hhrY = (render.width * hgt) * render_scale;
	float half_screen_x = (render.width / 2.0f), half_screen_y = (render.height / 2.0f);

	if ((abs(pX - rX) <= hwpX + hwrX + 3.f) && (abs(pY - rY) <= hhpY + hhrY + 3.f)) {
		return 1;
	}
	else if ((abs(pX) >= (half_screen_x - 5))) {
		return -1;
	}
	else if ((abs(pY) >= (half_screen_y - 5))) {
		return -2;
	}
	return 0;
}