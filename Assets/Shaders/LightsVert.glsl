#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

uniform mat4 uMVP;
uniform mat4 uModel;
uniform mat4 uNormalMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

void main()
{
    // world‐space position
    FragPos = vec3(uModel * vec4(aPosition, 1.0));
    // transform normal via the full 4×4 inverse‐transpose, treating it as a direction
    Normal  = normalize((uNormalMatrix * vec4(aNormal, 0.0)).xyz);
    // pass through UVs
    UV      = aUV;
    // finally, clip‐space position
    gl_Position = uMVP * vec4(aPosition, 1.0);
}