#version 330 core

in vec4 pos;

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

vec2 complexMultiply(vec2 a, vec2 b)
{
	float r = (a.x * b.x) - (a.y * b.y);
	float i = (a.x * b.y) + (a.y * b.x);
	return vec2(r, i);
}

vec2 complexDivide(vec2 a, vec2 b)
{
	vec2 rat = vec2(b.x, -b.y);
	vec2 denominator = complexMultiply(rat, b);
	float divisor = denominator.x;
	a = complexMultiply(a, rat);
	vec2 result = vec2(a.x / divisor, a.y / divisor);
	return result;
}

// f(z) = z^4 - 1
vec2 calcFx(vec2 a)
{
	a = complexMultiply(a, a);
	a = complexMultiply(a, a);
	a.x -= 1;
	return a;
}

// f'(z) = 4z^3
vec2 calcFpx(vec2 a)
{
	vec2 old = a;
	a = complexMultiply(a, a);
	a = complexMultiply(a, old);
	a = 4 * a;
	return a;
}

void main()
{
	vec2 roots[4];
	roots[0] = vec2(1.0f, 0.0f);
	roots[1] = vec2(-1.0f, 0.0f);
	roots[2] = vec2(0.0f, 1.0f);
	roots[3] = vec2(0.0f, -1.0f);

	int maxIterations = 50;

	vec2 a;
	a.x = ((pos.x * ratio * 2) / zoom) + origin.x;
	a.y = ((pos.y * 2) / zoom) + origin.y;

	vec2 fx = vec2(0.0f, 0.0f);
	vec2 fpx = vec2(0.0f, 0.0f);

	vec2 norm = vec2(0.0f, 0.0f);
	int root = -1;
	float tolerance = 0.001f;
	float dist = 0.0f;
	float prevDist = tolerance;
	int iterations = 0;

	for (int i = 0; i < maxIterations; ++i)
	{
		fx = calcFx(a);
		fpx = calcFpx(a);
		a = a - complexDivide(fx, fpx);

		for (int r = 0; r < 4; ++r)
		{
			norm = a - roots[r];
			dist = (norm.x * norm.x) + (norm.y * norm.y);

			if (dist < tolerance)
			{
				root = r;
				break;
			}
		}

		++iterations;

		if (root != -1)
			break;

		prevDist = dist;
	}

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
}

/*
dist - tolerance = distance to tolerance;
prevdist - tolerance = prev distance to tolerance;
0 < dist < tolerance;
0 < prevdist < infinity;
*/