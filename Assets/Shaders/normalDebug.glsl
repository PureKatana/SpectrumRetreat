#version 330 core
in vec3 Normal;
out vec4 FragColor;
void main() {
    vec3 n = normalize(Normal);
    // remap from [-1,1] to [0,1]
    FragColor = vec4(n * 0.5 + 0.5, 1.0);
}