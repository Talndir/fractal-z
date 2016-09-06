uniform int maxIterations;
uniform float colourScale;
uniform float colourOffset;

vec4 render(float p)
{
	if (p >= float(maxIterations))
		return vec4(0.0f, 0.0f, 0.0f, 1.0f);
	else
		return hsv_to_rgb((p / float(maxIterations)) * colourScale + colourOffset, 1.0f, 1.0f, 1.0f);
}