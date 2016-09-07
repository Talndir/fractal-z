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
uniform dvec2 origin;
uniform float ratio;
uniform float zoom;

// Standard functions

#define PI 3.1415926535897932384626433832795

// Double exp
double exp(double a)
{
	return exp(float(a));
}

// Double cosine
double cos(double a)
{
	return cos(float(a));
}

// Double sine
double sin(double a)
{
	return sin(float(a));
}

// Hyperbolic cosine
double cosh(double a)
{
	return (exp(a) + exp(-a)) / 2.0;
}

// Hyperbolic sine
double sinh(double a)
{
	return (exp(a) - exp(-a)) / 2.0;
}

// Multiply two complex numbers
dvec2 complexMultiply(dvec2 a, dvec2 b)
{
	double r = (a.x * b.x) - (a.y * b.y);	// Compute real part
	double i = (a.x * b.y) + (a.y * b.x);	// Cmopute imaginary part

	return dvec2(r, i);
}

// Divide two complex numbers (rationalise denominator, divide)
dvec2 complexDivide(dvec2 a, dvec2 b)
{
	dvec2 rat = dvec2(b.x, -b.y);							// Conjugate of denominator
	dvec2 denominator = complexMultiply(rat, b);			// Multiply denominator by conjugate
	double divisor = denominator.x;
	a = complexMultiply(a, rat);
	dvec2 result = dvec2(a.x / divisor, a.y / divisor);		// Divide

	return result;
}

// Get magnitude squared of a complex number (Pythagoras)
double complexMagnitudeSquared(dvec2 a)
{
	return (a.x * a.x) + (a.y * a.y);
}

// Get magnitude of a complex number
double complexMagnitude(dvec2 a)
{
	return sqrt(complexMagnitudeSquared(a));
}

// Square complex number
dvec2 complexSquare(dvec2 a)
{
	return dvec2((a.x * a.x) - (a.y * a.y), 2 * a.x * a.y);
}

// Complex cosine
dvec2 complexCos(dvec2 a)
{
	return dvec2(cos(a.x) * cosh(a.y), -1 * sin(a.x) * sinh(a.y));
}

// Complex sine
dvec2 complexSin(dvec2 a)
{
	return dvec2(sin(a.x) * cosh(a.y), cos(a.x) * sinh(a.y));
}

// Complex hyperbolic cosine
dvec2 complexCosh(dvec2 a)
{
	return dvec2(cosh(a.x) * cos(a.y), sinh(a.x) * sin(a.y));
}

// Complex hyperbolic sine
dvec2 complexSinh(dvec2 a)
{
	return dvec2(sinh(a.x) * cos(a.y), cosh(a.x) * sin(a.y));
}

/* BEGIN USER CODE */


/* END USER CODE */

// Main function
void main()
{
	ivec2 pix;	// Current pixel
	vec2 frac;	// Current position in fractal co-ords
	dvec2 pos;	// Value after scaling (screen co-ords)

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

	// Add on pixel offset to save to correct place
	pix = pix + ivec2(int(offset.x), int(offset.y));

	if (pix.x >= IMAGE_WIDTH)
		pix.x -= IMAGE_WIDTH;
	if (pix.y >= IMAGE_HEIGHT)
		pix.y -= IMAGE_HEIGHT;

	// Run user code
	imageStore(pixels, pix, vec4(compute(pos), 0.0f, 0.0f, 0.0f));
}