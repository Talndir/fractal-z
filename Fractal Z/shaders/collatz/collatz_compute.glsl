uniform int maxIterations;
uniform int limit;

float compute(dvec2 pos)
{
	dvec2 z = pos;
	double l = pow(10, limit);
	//double limit = 1000000;
	int i = 0;

	while ((i < maxIterations) && (complexMagnitude(z) < l))
	{
		z = 0.25 * ((dvec2(2.0, 0.0) + (7 * z)) - complexMultiply(dvec2(2.0, 0.0) + (5 * z), complexCos(PI * z)));

		++i;
	}

	return i + 0.01f;
}