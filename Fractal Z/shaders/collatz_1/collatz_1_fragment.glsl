#version 440 core

in vec4 pos;

out vec4 colour;

uniform float ratio;
uniform float zoom;
uniform vec2 origin;

const float PI = 3.1415926535897932384626433832795;
const float E = 2.7182818284590452353602875;

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

float cosh(float x)
{
	return (pow(E, x) + pow(E, -x)) / 2 ;
}

float sinh(float x)
{
	return (pow(E, x) - pow(E, -x)) / 2 ;
}

complex cCos(complex a)
{
	complex result;
	result.r = cos(a.r) * cosh(a.i);
	result.i = - (sin(a.r) * sinh(a.i));
	return result;
}

complex cSin(complex a)
{
	complex result;
	result.r = sin(a.r) * cosh(a.i);
	result.i = cos(a.r) * sinh(a.i);
	return result;
}


void main()
{
	/*
	vec2 c;
	c.x = ((pos.x * ratio * 20) / zoom) + origin.x;
	c.y = ((pos.y * 20) / zoom) + origin.y;
	vec2 z = vec2(0.0f, 0.0f);
	int i = 0;
	int maxIterations = int(floor(200 * pow(1.05, log2(zoom))));

	while ((i < maxIterations ) && (((z.x * z.x) + (z.y * z.y)) < 100))	
	{
		vec2 oldz = z;
		z = 5 * oldz;
		z += vec2(2, 0);
		z *= cos(oldz * PI);
		z = (vec2(2, 0) + (7 * oldz)) - z;
		z *= 0.25;
		z = 0.25 * (vec2(2, 0) + vec2(z.x * 7, z.y * 7) - ((vec2(2, 0) + vec2(z.x * 5, z.y * 5)) * cos(vec2(z.x * PI, z.y)));

		++i;
	}
	*/

	complex z;
	z.r = ((pos.x * ratio * 2) / zoom) + origin.x;
	z.i = ((pos.y * 2) / zoom) + origin.y;
	//z.r = 0;
	//z.i = 0;
	float d = 1000000;
	int i = 0;
	int maxIterations = int(floor(50 * pow(1.05, log2(zoom))));

	while ((i < maxIterations) && (abs(cAbsSqr(z)) <= d))
	{
		complex a = cCos(cMultReal(z, PI));
		complex b = cMultReal(z, 5);
		complex c;
		c.r = 2;
		c.i = 0;
		complex d = cMultReal(z, 7);

		z = cMultReal(cSub(cAdd(c, d), cMult(cAdd(c, b), a)), 0.25);
		//z = cMultReal(h, 0.25);
		++i;
	}

	if (i == maxIterations)	
		colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	else
	{
		float ifloat = i;
		float hue = ifloat / maxIterations;
		colour = hsv_to_rgb(hue, 1.0f, 1.0f, 1.0f);
	}
}