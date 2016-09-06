uniform int maxIterations;

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

// This is the function being used
// f(z) = z^4 - 1
dvec2 calcFx(dvec2 a)
{
	a = complexMultiply(a, a);	// z^2
	a = complexMultiply(a, a);	// z^2^2 = z^4
	a.x -= 1;					// z^4 - 1

	return a;
}

// This is the derivative of the function being used
// f'(z) = 4z^3
dvec2 calcFpx(dvec2 a)
{
	dvec2 old = a;					// Save old z
	a = complexMultiply(a, a);		// z^2
	a = complexMultiply(a, old);	// z^2 * z = z^3
	a = 4 * a;						// z^3 * 4

	return a;
}

// Compute
float compute(dvec2 pos)
{
	// Roots of f(z)
	dvec2 roots[4];
	roots[0] = dvec2(1.0f, 0.0f);	// 1
	roots[1] = dvec2(-1.0f, 0.0f);	// -1
	roots[2] = dvec2(0.0f, 1.0f);	// i
	roots[3] = dvec2(0.0f, -1.0f);	// -i

	// Initiaise variables
	dvec2 c = pos;
	dvec2 fx = vec2(0.0f, 0.0f);
	dvec2 fpx = vec2(0.0f, 0.0f);

	dvec2 norm = vec2(0.0f, 0.0f);
	int root = -1;
	double tolerance = 0.001f;
	double dist = 0.0f;
	int iterations = 0;

	// Previous closest to each root, used in smooth colouring
	double prev[4];
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

	// Calculate shade (smooth colouring)
	float shade = 0.0f;

	if (root != -1)
	{
		float logi = (log(float(tolerance)) - log(float(prev[root]))) / (log(float(dist)) - log(float(prev[root])));
		shade = ((iterations + logi) / float(maxIterations)) * 1.5f;
		shade = 1.0f - shade;
	}

	// Combine shade and iteration count in a single variable so they can be extracted later
	shade = floor(shade * 100) * 10;
	++root;

	return float(root + shade);
}