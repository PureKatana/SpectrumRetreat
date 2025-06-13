#version 330 core

// Inputs from vertex buffer
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

// Uniform
uniform mat4 uMVP;

// Outputs to fragment shader
out vec3 vNormal;
out vec2 vUV;

void main()
{
    gl_Position = uMVP * vec4(aPosition, 1.0);
    vNormal = aNormal;
    vUV = aUV;
}