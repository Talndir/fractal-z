
/* END USER CODE */

// Main function
void main()
{
	ivec2 pix;	// Current pixel
	vec2 frac;	// Current position in fractal co-ords
	dvec2 pos;	// Value after scaling (screen co-ords)

	// Calculate current pixel being worked on from work group
	int j = int(gl_LocalInvocationID.x);
	int k = int(gl_LocalInvocationID.y);

	pix.x = int(gl_WorkGroupID.x * BLOCK_WIDTH) + j;
	pix.y = int(gl_WorkGroupID.y * BLOCK_HEIGHT) + k;

	pix = pix + ivec2(int(renderOffset.x), int(renderOffset.y));

	// Calculate position in fractal co-ords from pixel
	frac.x = ((float(pix.x - (IMAGE_WIDTH / 2)) / float(WINDOW_WIDTH)) * 2);
	frac.y = ((float(pix.y - (IMAGE_HEIGHT / 2)) / float(WINDOW_HEIGHT)) * 2);

	// Calculate screen co-ords from fractal co-ords
	pos.x = ((frac.x * ratio) / zoom);
	pos.y = (frac.y / zoom);

	pos = pos + origin;

	// Add on pixel offset to save to correct place
	pix = pix + ivec2(int(offset.x), int(offset.y));

	if (pix.x >= IMAGE_WIDTH)
		pix.x -= IMAGE_WIDTH;
	if (pix.y >= IMAGE_HEIGHT)
		pix.y -= IMAGE_HEIGHT;

	// Run user code
	imageStore(pixels, pix, vec4(compute(pos), 0.0f, 0.0f, 0.0f));
}