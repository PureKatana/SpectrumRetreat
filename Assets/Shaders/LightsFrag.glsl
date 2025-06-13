#version 330 core

struct DirLight {
    vec3  direction;
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;
};

struct PointLight {
    vec3  position;
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3  position;
    vec3  direction;
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

#define MAX_DIR    10
#define MAX_POINT  30
#define MAX_SPOT   10

uniform int        numDir;
uniform DirLight   dirLights[MAX_DIR];

uniform int        numPoint;
uniform PointLight pointLights[MAX_POINT];

uniform int        numSpot;
uniform SpotLight  spotLights[MAX_SPOT];

in vec3 FragPos;
in vec3 Normal;
in vec2 UV;

out vec4 FragColor;

uniform vec3  uViewPos;
uniform vec4  uMatDiffuse;
uniform vec4  uMatSpecular;
uniform float uMatShininess;

uniform sampler2D u_Texture;
uniform float u_opacity;

vec4 CalcDirLight(DirLight light, vec3 N, vec3 V, vec4 texColor)
{
    vec3 L = normalize(light.direction);
    float diff = max(dot(N, L), 0.0);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), uMatShininess);

    vec4 ambient  = light.ambient  * texColor;
    vec4 diffuse  = light.diffuse  * diff * texColor;
    vec4 specular = light.specular * spec * uMatSpecular;
    return ambient + diffuse + specular;
}

vec4 CalcPointLight(PointLight light, vec3 N, vec3 V, vec4 texColor)
{
    vec3 L = normalize(light.position - FragPos);
    float diff = max(dot(N, L), 0.0);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), uMatShininess);

    float dist = length(light.position - FragPos);
    float att  = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec4 ambient  = light.ambient  * texColor;
    vec4 diffuse  = light.diffuse  * diff * texColor;
    vec4 specular = light.specular * spec * uMatSpecular;
    return (ambient + diffuse + specular) * att;
}

vec4 CalcSpotLight(SpotLight light, vec3 N, vec3 V, vec4 texColor)
{
    vec3 L = normalize(light.position - FragPos);

    float cosTheta = dot(L, normalize(light.direction));
    float phi      = acos(cosTheta);  // phi in radians

    float inner = light.cutOff;       // still in radians
    float outer = light.outerCutOff;

    float intensity = clamp((phi - outer) / (inner - outer), 0.0, 1.0);

    float diff = max(dot(N, L), 0.0);
    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), uMatShininess);

    float dist = length(light.position - FragPos);
    float att  = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec4 ambient  = light.ambient  * texColor;
    vec4 diffuse  = light.diffuse  * diff * texColor;
    vec4 specular = light.specular * spec * uMatSpecular;
    return (ambient + diffuse + specular) * att * intensity;
}

void main()
{
    vec3 V = normalize(uViewPos - FragPos);

    vec4 texColor = texture(u_Texture, UV) * uMatDiffuse;

    vec4 color = vec4(0.0);

    for (int i = 0; i < numDir; ++i)
        color += CalcDirLight(dirLights[i], Normal, V, texColor);

    for (int i = 0; i < numPoint; ++i)
        color += CalcPointLight(pointLights[i], Normal, V, texColor);

    for (int i = 0; i < numSpot; ++i)
        color += CalcSpotLight(spotLights[i], Normal, V, texColor);
        
        color.a = texColor.a;

    FragColor = vec4(color.rgb, color.a * u_opacity);
}