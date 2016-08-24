uniform int maxIterations;

float compute(vec2 pos)
{
	vec2 z = vec2(0.0f, 0.0f);
	vec2 c = pos;
	float rsqr = 0.0f;
	float isqr = 0.0f;
	int i = 0;

	while ((i < maxIterations) && (isqr + rsqr < 4.0f))
	{
		z.y = z.x * z.y;
		z.y += z.y + c.y;
		z.x = rsqr - isqr + c.x;
		rsqr = z.x * z.x;
		isqr = z.y * z.y;

		++i;
	}

	float result = float(i + 1 - (log(log(float(sqrt(rsqr + isqr)))) / log(2.0)));

	return result + 0.01f;
}