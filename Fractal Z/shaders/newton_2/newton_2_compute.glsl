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
layout (binding = 0, r16i) uniform iimage2D pixels;

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

// Multiply two complex numbers
vec2 complexMultiply(vec2 a, vec2 b)
{
	float r = (a.x * b.x) - (a.y * b.y);	// Compute real part
	float i = (a.x * b.y) + (a.y * b.x);	// Cmopute imaginary part

	return vec2(r, i);
}

// Divide two complex numbers (rationalise denominator, divide)
vec2 complexDivide(vec2 a, vec2 b)
{
	vec2 rat = vec2(b.x, -b.y);							// Conjugate of denominator
	vec2 denominator = complexMultiply(rat, b);			// Multiply denominator by conjugate
	float divisor = denominator.x;
	a = complexMultiply(a, rat);
	vec2 result = vec2(a.x / divisor, a.y / divisor);	// Divide

	return result;
}

// This is the function being used
// f(z) = z^4 - 1
vec2 calcFx(vec2 a)
{
	a = complexMultiply(a, a);	// z^2
	a = complexMultiply(a, a);	// z^2^2 = z^4
	a.x -= 1;					// z^4 - 1

	return a;
}

// This is the derivative of the function being used
// f'(z) = 4z^3
vec2 calcFpx(vec2 a)
{
	vec2 old = a;					// Save old z
	a = complexMultiply(a, a);		// z^2
	a = complexMultiply(a, old);	// z^2 * z = z^3
	a = 4 * a;						// z^3 * 4

	return a;
}

void main()
{
	// Roots of f(z)
	vec2 roots[4];
	roots[0] = vec2(1.0f, 0.0f);	// 1
	roots[1] = vec2(-1.0f, 0.0f);	// -1
	roots[2] = vec2(0.0f, 1.0f);	// i
	roots[3] = vec2(0.0f, -1.0f);	// -i

	// Maximum iterations, can be much lower than Mandelbrot and Julia
	int maxIterations = 50;
	
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

	// Initiaise variable
	vec2 fx = vec2(0.0f, 0.0f);
	vec2 fpx = vec2(0.0f, 0.0f);

	vec2 norm = vec2(0.0f, 0.0f);
	int root = -1;
	float tolerance = 0.001f;
	float dist = 0.0f;
	int iterations = 0;

	// Previous closest to each root, used in smooth colouring
	float prev[4];
	prev[0] = tolerance;
	prev[1] = tolerance;
	prev[2] = tolerance;
	prev[3] = tolerance;

	// Main loop
	for (int i = 0; i < maxIterations; ++i)
	{
		fx = calcFx(c);					// Calculate f(z)
		fpx = calcFpx(c);				// Calculate f'(z)
		c = c - complexDivide(fx, fpx);	// Calculate Newton-Raphson iteration (z = z - f(z) / f'(z))



		// Loop through each root
		for (int r = 0; r < 4; ++r)
		{
			norm = c - roots[r];
			dist = (norm.x * norm.x) + (norm.y * norm.y);

			// If close to a root, break
			if (dist < tolerance)
			{
				root = r;
				break;
			}

			// Save distance to root
			prev[r] = dist;
		}

		++iterations;

		// Break again (break above only breaks to here)
		if (root != -1)
			break;
	}

	/*
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

	float logi = (log(tolerance) - log(prevDist)) / (log(dist) - log(prevDist));
	float shade = ((iterations + logi) / maxIterations) * 1.5f;
	shade = 1.0f - shade;
	colour = vec4(colour.x * shade, colour.y * shade, colour.z * shade, 1.0f);
	*/

	// Add on pixel offset to save to correct place
	pix = pix + offset;

	if (pix.x >= IMAGE_WIDTH)
		pix.x -= IMAGE_WIDTH;
	if (pix.y >= IMAGE_HEIGHT)
		pix.y -= IMAGE_HEIGHT;

	// Calculate shade (smooth colouring)
	float shade = 0.0f;

	if (root != -1)
	{
		float logi = (log(tolerance) - log(prev[root])) / (log(dist) - log(prev[root]));
		shade = ((iterations + logi) / maxIterations) * 1.5f;
		shade = 1.0f - shade;
	}

	// Combine shade and iteration count in a single variable so they can be extracted later
	shade = floor(shade * 100) * 10;
	++root;

	// Store final value in the image
	imageStore(pixels, pix, ivec4(shade + root, 0, 0, 0));
}

/*
dist - tolerance = distance to tolerance;
prevdist - tolerance = prev distance to tolerance;
0 < dist < tolerance;
0 < prevdist < infinity;
*/