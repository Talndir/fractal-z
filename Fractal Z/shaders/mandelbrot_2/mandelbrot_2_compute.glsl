#version 440 core
#extension GL_ARB_compute_variable_group_size : enable	// Get group size from main program instead of in here
#extension GL_ARB_gpu_shader_fp64 : enable				// 64-bit floating-point (higher accuracy = deeper zoom)

/* List of available built-in constants
in uvec3 gl_NumWorkGroups;
in uvec3 gl_WorkGroupID;
in uvec3 gl_LocalInvocationID;
in uvec3 gl_GlobalInvocationID;
in uint  gl_LocalInvocationIndex;
*/

// Receive group size from main program
layout(local_size_variable) in;

// Image from image load/store
layout (binding = 0, r32f) uniform image2D pixels;

// Uniforms
uniform float ratio;
uniform float zoom;
uniform vec2 origin;
uniform int maxIterations;
uniform ivec2 renderOffset;
uniform ivec2 offset;

uniform int WINDOW_WIDTH;
uniform int WINDOW_HEIGHT;
uniform int BLOCK_WIDTH;
uniform int BLOCK_HEIGHT;
uniform int IMAGE_WIDTH;
uniform int IMAGE_HEIGHT;

uniform float normaliser;

void main()
{
	ivec2 pix;	// Current pixel
	vec2 pos;	// Current position in fractal co-ords
	vec2 c;		// Value of 'c'

	// Calculate current pixel being worked on from work group
	int j = int(gl_LocalInvocationID.x);
	int k = int(gl_LocalInvocationID.y);

	pix.x = int(gl_WorkGroupID.x * BLOCK_WIDTH) + j;
	pix.y = int(gl_WorkGroupID.y * BLOCK_HEIGHT) + k;

	pix = pix + renderOffset;

	// Calculate position in fractal co-ords from pixel
	pos.x = ((float(pix.x - (IMAGE_WIDTH / 2)) / float(WINDOW_WIDTH)) * 2);
	pos.y = ((float(pix.y - (IMAGE_HEIGHT / 2)) / float(WINDOW_HEIGHT)) * 2);

	c.x = ((pos.x * ratio * 2) / zoom);
	c.y = ((pos.y * 2) / zoom);

	c = c + origin;

	// Initialising variables
	ivec2 r = pix;
	vec2 z = vec2(0.0f, 0.0f);
	int i = 0;
	float rsqr = 0.0f;
	float isqr = 0.0f;

	// Main loop
	while ((i < maxIterations) && ((rsqr + isqr) < 4))	// Ends when max iterations exceeded or 'z' out of bounds
	{
		// Effecient algorithm for computing 'z = z^2 + c'
		z.y = z.x * z.y;
		z.y += z.y + c.y;
		z.x = (rsqr - isqr) + c.x;
		rsqr = z.x * z.x;
		isqr = z.y * z.y;

		++i;
	}

	// Add on pixel offset to save to correct place
	pix = pix + offset;

	if (pix.x >= IMAGE_WIDTH)
		pix.x -= IMAGE_WIDTH;
	if (pix.y >= IMAGE_HEIGHT)
		pix.y -= IMAGE_HEIGHT;

	float result = float(i - (log(log(float(sqrt(z.x*z.x + z.y*z.y)))) / log(2.0)));
	int final = int(floor(float(result + i) * normaliser));

	if (i == maxIterations)
		final = 32767;
	if (i == maxIterations)
		++i;

	// Store final value in the image
	//imageStore(pixels, pix, ivec4(final, 0, 0, 0));
	imageStore(pixels, pix, vec4(i + result, 0.f, 0.f, 0.f));
}