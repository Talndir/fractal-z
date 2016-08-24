/* TEMPLATE COMPUTE SHADER */
#version 440 core

// Work group size
layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

// Image from image load/store
uniform image2D pixels;

// Standard uniforms
uniform int WINDOW_WIDTH;
uniform int WINDOW_HEIGHT;
uniform int BLOCK_WIDTH;
uniform int BLOCK_HEIGHT;
uniform int IMAGE_WIDTH;
uniform int IMAGE_HEIGHT;
uniform vec2 renderOffset;
uniform vec2 offset;
uniform vec2 origin;
uniform float ratio;
uniform float zoom;

// Standard functions



/* BEGIN USER CODE */
