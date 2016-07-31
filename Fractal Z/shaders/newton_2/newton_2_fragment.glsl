#version 440 core

// Position input
in vec2 pos;

// Output colour of the pixel
out vec4 colour;

// Image from image load/store
layout (binding = 0, r16i) uniform iimage2D pixels;

// Uniforms
uniform float ratio;
uniform float zoom;
uniform int maxIterations;
uniform ivec2 offset;

uniform int WINDOW_WIDTH;
uniform int WINDOW_HEIGHT;
uniform int BLOCK_WIDTH;
uniform int BLOCK_HEIGHT;
uniform int IMAGE_WIDTH;
uniform int IMAGE_HEIGHT;

uniform float normaliser;

// Function to convert from HSV to RGB colour spaces
vec4 hsv_to_rgb(float h, float s, float v, float a)
{
	float c = v * s;
	h = mod((h * 6.0), 6.0);
	float x = c * (1.0 - abs(mod(h, 2.0) - 1.0));
	vec4 color;

	if (0.0 <= h && h < 1.0)
		color = vec4(c, x, 0.0, a);
	else if (1.0 <= h && h < 2.0)
		color = vec4(x, c, 0.0, a);
	else if (2.0 <= h && h < 3.0)
		color = vec4(0.0, c, x, a);
	else if (3.0 <= h && h < 4.0)
		color = vec4(0.0, x, c, a);
	else if (4.0 <= h && h < 5.0)
		color = vec4(x, 0.0, c, a);
	else if (5.0 <= h && h < 6.0)
		color = vec4(c, 0.0, x, a);
	else
		color = vec4(0.0, 0.0, 0.0, a);

	color.rgb += v - c;

	return color;
}

void main()
{
	// Calculate pixel to load from pixel position
	int m = int(floor((pos.x + 1.0) * (WINDOW_WIDTH / 2)));
	int n = int(floor((pos.y + 1.0) * (WINDOW_HEIGHT / 2)));
	ivec2 s = ivec2(m + (BLOCK_WIDTH * 1), n + (BLOCK_HEIGHT * 1)) + offset;

	if (s.x >= IMAGE_WIDTH)
		s.x -= IMAGE_WIDTH;
	if (s.y >= IMAGE_HEIGHT)
		s.y -= IMAGE_HEIGHT;

	// Load pixel
	int j = imageLoad(pixels, s).r;
	//int final = int(j / normaliser);

	// Extract root and shade from value
	int root = (j % 10) - 1;
	float shade = float(j - root + 1) / 1000.f;

	// Colour based on root
	switch (root)
	{
	case -1:
		colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 0:
		colour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 1:
		colour = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case 2:
		colour = vec4(0.0f, 0.0f, 1.0f, 1.0f);
		break;
	case 3:
		colour = vec4(1.0f, 1.0f, 0.0f, 1.0f);
		break;
	}

	// Output final pixel colour
	colour = vec4(colour.x * shade, colour.y * shade, colour.z * shade, 1.0f);
}