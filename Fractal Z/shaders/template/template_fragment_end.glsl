
/* END USER CODE */

// Main function
void main()
{
	// Calculate and load pixel
	int m = int(floor((pos.x + 1.0) * (WINDOW_WIDTH / 2)));
	int n = int(floor((pos.y + 1.0) * (WINDOW_HEIGHT / 2)));
	ivec2 s = ivec2(m + (BLOCK_WIDTH * 1), n + (BLOCK_HEIGHT * 1)) + ivec2(int(offset.x), int(offset.y));

	if (s.x >= IMAGE_WIDTH)
		s.x -= IMAGE_WIDTH;
	if (s.y >= IMAGE_HEIGHT)
		s.y -= IMAGE_HEIGHT;

	float p = imageLoad(pixels, s).r;

	// Run user code
	colour = render(p);
}