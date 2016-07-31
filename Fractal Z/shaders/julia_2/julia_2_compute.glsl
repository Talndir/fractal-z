#version 440 core
#extension GL_ARB_compute_variable_group_size : enable
#extension GL_ARB_gpu_shader_fp64 : enable

layout(local_size_variable) in;

layout (binding = 0, r16i) uniform iimage2D pixels;

uniform float ratio;
uniform float zoom;
uniform vec2 origin;
uniform vec2 c;
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
	ivec2 pix;
	vec2 pos;

	int j = int(gl_LocalInvocationID.x);
	int k = int(gl_LocalInvocationID.y);

	pix.x = int(gl_WorkGroupID.x * BLOCK_WIDTH) + j;
	pix.y = int(gl_WorkGroupID.y * BLOCK_HEIGHT) + k;

	pix = pix + renderOffset;

	pos.x = ((float(pix.x - (IMAGE_WIDTH / 2)) / float(WINDOW_WIDTH)) * 2);
	pos.y = ((float(pix.y - (IMAGE_HEIGHT / 2)) / float(WINDOW_HEIGHT)) * 2);
	
	vec2 z;
	z.x = ((pos.x * ratio * 2) / zoom);
	z.y = ((pos.y * 2) / zoom);
	z = z + origin;

	int i = 0;
	float rsqr = z.x * z.x;
	float isqr = z.y * z.y;
	float tempr = 0.0f;

	while ((i < maxIterations) && ((rsqr + isqr) < 4))
	{
		tempr = z.x;
		z.x = (rsqr - isqr) + c.x;
		z.y = (2 * tempr * z.y) + c.y;
		rsqr = z.x * z.x;
		isqr = z.y * z.y;

		++i;
	}

	pix = pix + offset;

	if (pix.x >= IMAGE_WIDTH)
		pix.x -= IMAGE_WIDTH;
	if (pix.y >= IMAGE_HEIGHT)
		pix.y -= IMAGE_HEIGHT;

	//float result = float(i - (log(log(sqrt(z.x*z.x + z.y*z.y))) / log(2.0)));
	//int final = int(result * normaliser);
	//int final = int(i * normaliser);

	imageStore(pixels, pix, ivec4(i, 0, 0, 0));
}