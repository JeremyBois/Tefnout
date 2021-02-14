#version 330 core

// In
in vec3 vertexColor;

// Out
out vec4 fragColor;


void main()
{
    // Multiplication
    fragColor = vec4(vertexColor * 0.5 + 0.5, 1.0);
}
