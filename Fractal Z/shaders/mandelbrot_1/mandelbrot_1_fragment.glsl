#version 440 core
//#extension GL_ARB_gpu_shader_fp64 : enable

in vec2 pos;

out vec4 colour;

uniform float ratio;
uniform float zoom;
uniform vec2 origin;

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

struct complex
{
	float r;
	float i;
};

complex cAdd(complex a, complex b)
{
	complex result;
	result.r = a.r + b.r;
	result.i = a.i + b.i;
	return result;
}

complex cSub(complex a, complex b)
{
	complex result;
	result.r = a.r + b.r;
	result.i = a.i + b.i;
	return result;
}

complex cMultReal(complex a, float d)
{
	complex result;
	result.r = a.r * d;
	result.i = a.i * d;
	return result;
}

complex cMult(complex a, complex b)
{
	complex result;
	result.r = (a.r * b.r) - (a.i * b.i);
	result.i = (a.r * b.i) + (a.i * b.r);
	return result;
}

complex cDivReal(complex a, float d)
{
	complex result;
	result.r = a.r / d;
	result.i = a.i / d;
	return result;
}

complex cDiv(complex a, complex b)
{
	complex result;
	complex rationaliser;
	rationaliser.r = (b.r);
	rationaliser.i = (b.i);
	float denominator = (b.r * b.r) + (b.i * b.i);
	result = cDivReal(cMult(a, rationaliser), denominator);
	return result;
}

float cAbs(complex a)
{
	return sqrt((a.r * a.r) + (a.i * a.i));
}

float cAbsSqr(complex a)
{
	return (a.r * a.r) + (a.i * a.i);
}


void main()
{
	/*
	vec2 c;
	c.x = ((pos.x * ratio * 2) / zoom) + origin.x;
	c.y = ((pos.y * 2) / zoom) + origin.y;
	vec2 z = vec2(0.0f, 0.0f);
	int i = 0;
	int maxIterations = int(floor(200 * pow(1.05, log2(zoom))));
	float rsqr = 0.0f;
	float isqr = 0.0f;
	float tempr = 0.0f;

	while ((i < maxIterations ) && ((rsqr + isqr) < 4))
	{
		tempr = z.x;
		z.x = (rsqr - isqr) + c.x;
		z.y = (2 * tempr * z.y) + c.y;
		rsqr = z.x * z.x;
		isqr = z.y * z.y;

		++i;
	}
	*/
	
	complex c;
	c.r = ((pos.x * ratio * 2) / zoom) + origin.x;
	c.i = ((pos.y * 2) / zoom) + origin.y;
	complex z;
	z.r = 0.0;
	z.i = 0.0;
	int i = 0;
	int maxIterations = int(floor(200 * pow(1.05, log2(zoom))));
	float rsqr = 0.0f;
	float isqr = 0.0f;
	float tempr = 0.0f;
	float escapeSquared = 4.0f;

	/*
	while ((i < maxIterations ) && ((rsqr + isqr) < 4))
	{
		tempr = z.r;
		z.r = (rsqr - isqr) + c.r;
		z.i = (2 * tempr * z.i) + c.i;
		rsqr = z.r * z.r;
		isqr = z.i * z.i;
		++i;
	}
	*/

	while ((i < maxIterations) && (cAbsSqr(z) < escapeSquared))
	{
		z = cAdd(cMult(z, z), c);
		++i;
	}
	
	if (i == maxIterations)	
		colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	else
	{
		float ifloat = i;
		//float hue = ifloat / maxIterations;
		float hue = (i - (log(log(cAbs(z))) /  log(2.0)) ) / maxIterations;
		colour = hsv_to_rgb(hue, 1.0f, 1.0f, 1.0f);
	}
}