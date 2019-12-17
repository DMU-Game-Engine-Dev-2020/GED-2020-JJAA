#version 440 core
			
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_vertexColour;

out vec3 fragmentColour;

layout (std140) uniform Matrices
{
	mat4 u_viewProjection;
};

uniform mat4 u_model;

void main()
{
	fragmentColour = a_vertexColour;
	gl_Position = u_viewProjection * u_model * vec4(a_vertexPosition, 1.0);
}