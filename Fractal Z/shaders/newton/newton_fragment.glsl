vec4 render(float p)
{
	int root = (int(p) % 10) - 1;
	float shade = float((p - root) + 1) / 1000.f;

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

	return vec4(colour.x * shade, colour.y * shade, colour.z * shade, 1.0f);
}