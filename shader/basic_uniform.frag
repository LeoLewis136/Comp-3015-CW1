#version 460

in vec2 TexCoord;
in vec3 LightDir;
in vec3 ViewDir;
in vec3 CalculatedVertexNormal;

layout (binding = 0) uniform sampler2D woodTexture;
layout (binding = 1) uniform sampler2D woodNormal;

layout (location = 0)out vec4 FragColor;

uniform struct LightInfo{
    // Static Lighting
    vec3 Position;
    vec3 Ambient;
    vec3 Specular;

} Light;

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

// float fogCalculation(){
//     float dist = abs(Position.z);
//     float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
//     return clamp(fogFactor, 0.0f, 1.0f);
// }

vec3 phongLighting(LightInfo Light, vec3 n){
    vec3 texColour = texture(woodTexture, TexCoord).rgb;

    vec3 diffuse = vec3(0), spec = vec3(0);
    vec3 ambient = Light.Ambient * texColour;

    vec3 s = normalize(LightDir);
    float sDotN = max(dot(s, n), 0.0f);

    diffuse = texColour * sDotN;

    spec = Material.Specular * pow(max(dot(reflect(s, n), ViewDir), 0.0f), Material.Shininess);

    return Light.Specular * (diffuse + spec) + ambient;
}

void main() {
    // vec3 LightIntensity = vec3(0.0f);
    // for (int i = 0; i < 3; i++){
    //     LightIntensity += phongLighting(lights[i], Position, Normal);
    // }
    
    
    vec3 norm = CalculatedVertexNormal;
    norm.xy = 2.0 * norm.xy - 1.0;

    FragColor = vec4(norm * 0.5 + 0.5, 0.0);
}
