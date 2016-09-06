uniform int maxIterations;
uniform int power;

float compute(dvec2 pos)
{
	dvec2 z = vec2(0.0f, 0.0f);
	dvec2 original = z;
	dvec2 c = pos;
	int i = 0;

	while ((i < maxIterations) && (complexMagnitudeSquared(z) < 4.0))
	{
		original = z;

		for (int j = 1; j < power; ++j)
			z = complexMultiply(z, original);

		z = z + c;

		++i;
	}

	float result = float(i + 1 - (log(log(float(complexMagnitude(z)))) / log(2.0)));

	return result + 0.01f;
}