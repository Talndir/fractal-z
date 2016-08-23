#version 440 core

// Position input
in vec2 pos;

// Output colour of the pixel
out vec4 colour;

// Image from image load/store
layout (binding = 0, r32f) uniform image2D pixels;

// Uniforms
uniform int WINDOW_WIDTH;
uniform int WINDOW_HEIGHT;
uniform int BLOCKS_EXTRA_HORIZONTAL;
uniform int BLOCKS_EXTRA_VERTICAL;

uniform vec2 offset;

void main()
{
	int m = int(floor((pos.x + 1.0) * (WINDOW_WIDTH / 2)));
	int n = int(floor((pos.y + 1.0) * (WINDOW_HEIGHT / 2)));
	ivec2 s = ivec2(m + (16 * BLOCKS_EXTRA_HORIZONTAL), n + (16 * BLOCKS_EXTRA_VERTICAL));
	s  = ivec2(int(s.x + offset.x), int(s.y + offset.y));

	if (s.x >= WINDOW_WIDTH)
		s.x -= WINDOW_WIDTH;
	else if (s.x < 0)
		s.x += WINDOW_WIDTH;

	if (s.y >= WINDOW_HEIGHT)
		s.y -= WINDOW_HEIGHT;
	else if (s.y < 0)
		s.y += WINDOW_HEIGHT;

	float j = imageLoad(pixels, s).r;

	colour = vec4(j, 0.0f, 0.0f, 1.0f);
}