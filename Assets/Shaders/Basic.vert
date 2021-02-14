#version 330 core

// In
layout (location = 0) in vec3 aPos;

// Out
out vec3 vertexColor;

void main()
{
    // Vertex position
    gl_Position = vec4(aPos, 1.0);

    // To pixel shader
    vertexColor = gl_Position.xyz;
}
