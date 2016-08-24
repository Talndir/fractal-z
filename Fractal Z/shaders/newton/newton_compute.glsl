uniform int maxIterations;

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

// Compute
float compute(vec2 pos)
{
	// Roots of f(z)
	vec2 roots[4];
	roots[0] = vec2(1.0f, 0.0f);	// 1
	roots[1] = vec2(-1.0f, 0.0f);	// -1
	roots[2] = vec2(0.0f, 1.0f);	// i
	roots[3] = vec2(0.0f, -1.0f);	// -i

	// Initiaise variables
	vec2 c = pos;
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

	return float(root + shade);
}