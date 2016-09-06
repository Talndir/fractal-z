uniform dvec2 c;

float compute(dvec2 pos)
{
	dvec2 z = pos;
	double rsqr = z.x * z.x;
	double isqr = z.y * z.y;
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