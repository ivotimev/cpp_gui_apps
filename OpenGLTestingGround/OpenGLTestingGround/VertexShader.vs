#version 330 

layout(location = 0) in vec3 position;

void main()
{
	gl_Position = vec4(position, 1.0f);
	//vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);
}