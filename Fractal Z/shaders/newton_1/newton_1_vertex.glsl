#version 330 core
layout(location = 0) in vec3 position;

out vec2 pos;

void main()
{
	gl_Position = vec4(position.x, position.y, position.z, 1.0f);
	pos = vec2(position.x, position.y);
}