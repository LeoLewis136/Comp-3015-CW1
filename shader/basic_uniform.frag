#version 460

in vec2 TexCoord;
in vec3 Position;
in vec3 Normal;

layout (binding = 0) uniform sampler2D albedoTexture; // GL_TEXTURE0 is object "albedo" texture
layout (binding = 1) uniform sampler2D normalTexture; // GL_TEXTURE1 is object normal texture

layout (location = 0) out vec4 FragColor;

uniform struct LightInfo{
    // Static Lighting
    vec3 Position;
    vec3 Ambient;
    vec3 Specular;

} lights[3];

uniform struct FogInfo{
    float MaxDist; // Distance of 100% fog
    float MinDist; // Start distance for fog
    vec3 FogColour; // The colour of the fog
}Fog;

uniform struct MaterialInfo{
    vec3 Colour;
    vec3 Specular;
    vec3 Ambient;
    float Shininess;
}Material;

float fogCalculation(){
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
    return clamp(fogFactor, 0.0f, 0.8f);
}

vec4 phongLighting(LightInfo _Light, vec3 pos, vec3 n){
    vec4 texColour = texture(albedoTexture, TexCoord);

    if (texColour.a < 0.1){
        discard;
    }

    vec4 diffuse = vec4(0), spec = vec4(0);
    vec4 ambient = vec4(_Light.Ambient, 0.0f) * texColour;

    vec3 s = normalize((vec4(_Light.Position, 1.0f).xyz - pos));
    float sDotN = max(dot(s, n), 0.0f);

    diffuse = texColour * sDotN;

    vec4 v = vec4(normalize(-pos.xyz), 1.0f);
    vec4 r = vec4(reflect(-s, n), 1.0f);
    spec = vec4(_Light.Specular, 0.0f) * pow(max(dot(r, v), 0.0f), Material.Shininess);

    return vec4(_Light.Specular, 1.0) * (diffuse + spec) + ambient;
}

void main() {
    vec3 norm = texture(normalTexture, TexCoord).xyz;
    norm.xy = 2.0 * norm.xy - 1.0;
    norm = normalize(norm + Normal);

    vec4 LightIntensity = vec4(0.0f);
    for (int i = 0; i < 3; i++){
        LightIntensity += phongLighting(lights[i], Position, norm);
    }

    FragColor = mix(vec4(Fog.FogColour, 1.0), LightIntensity, fogCalculation());
}
