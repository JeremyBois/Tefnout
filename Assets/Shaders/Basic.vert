#version 330 core

// In
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

// Uniforms
uniform mat4 uProjectionView;

// Out
out vec4 vertexColor;

void main()
{
    // Vertex position
    gl_Position = uProjectionView * vec4(aPos, 1.0);

    // To pixel shader
    vertexColor = aColor;
}
