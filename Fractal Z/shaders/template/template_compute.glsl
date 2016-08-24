/* TEMPLATE COMPUTE SHADER */
#version 440 core

// Work group size
layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

// Image from image load/store
uniform image2D pixels;

// Standard uniforms
uniform int WINDOW_WIDTH;
uniform int WINDOW_HEIGHT;
uniform int BLOCK_WIDTH;
uniform int BLOCK_HEIGHT;
uniform int IMAGE_WIDTH;
uniform int IMAGE_HEIGHT;
uniform vec2 renderOffset;
uniform vec2 offset;
uniform vec2 origin;
uniform float ratio;
uniform float zoom;

// Standard functions



/* BEGIN USER CODE */


/* END USER CODE */

// Main function
void main()
{
	ivec2 pix;	// Current pixel
	vec2 frac;	// Current position in fractal co-ords
	vec2 pos;	// Value after scaling (screen co-ords)

	// Calculate current pixel being worked on from work group
	int j = int(gl_LocalInvocationID.x);
	int k = int(gl_LocalInvocationID.y);

	pix.x = int(gl_WorkGroupID.x * BLOCK_WIDTH) + j;
	pix.y = int(gl_WorkGroupID.y * BLOCK_HEIGHT) + k;

	pix = pix + ivec2(int(renderOffset.x), int(renderOffset.y));

	// Calculate position in fractal co-ords from pixel
	frac.x = ((float(pix.x - (IMAGE_WIDTH / 2)) / float(WINDOW_WIDTH)) * 2);
	frac.y = ((float(pix.y - (IMAGE_HEIGHT / 2)) / float(WINDOW_HEIGHT)) * 2);

	// Calculate screen co-ords from fractal co-ords
	pos.x = ((frac.x * ratio) / zoom);
	pos.y = (frac.y / zoom);

	pos = pos + origin;

	// Run user code
	imageStore(pixels, pix, vec4(compute(pos), 0.0f, 0.0f, 0.0f));
}