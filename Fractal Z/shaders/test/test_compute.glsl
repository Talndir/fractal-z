#version 440 core

// Group size
layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

// Image from image load/store
uniform image2D pixels;

// Uniforms
uniform int WINDOW_WIDTH;
uniform int WINDOW_HEIGHT;

uniform float scalar;

void main()
{
	ivec2 pix;

	int j = int(gl_LocalInvocationID.x);
	int k = int(gl_LocalInvocationID.y);

	pix.x = int(gl_WorkGroupID.x * 16) + j;
	pix.y = int(gl_WorkGroupID.y * 16) + k;

	imageStore(pixels, pix, vec4((float(pix.x + pix.y) / float(WINDOW_WIDTH + WINDOW_HEIGHT)) * scalar, 0.0f, 0.0f, 1.0f));
}