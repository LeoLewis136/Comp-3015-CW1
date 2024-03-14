#version 460

in vec4 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (binding = 0) uniform sampler2D woodTexture;

out vec4 FragColor;

uniform struct LightInfo{
    // Static Lighting
    vec4 Position;
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
    return clamp(fogFactor, 0.0f, 1.0f);
}

vec3 phongLighting(LightInfo Light, vec4 pos, vec3 n){
    vec3 texColour = texture(woodTexture, TexCoord).rgb;

    vec3 diffuse = vec3(0), spec = vec3(0);
    vec3 ambient = Light.Ambient * texColour;

    vec3 s = normalize(vec3(Light.Position - pos));
    float sDotN = max(dot(s, n), 0.0f);

    diffuse = texColour * sDotN;

    spec = vec3(0.0f);
    if (sDotN > 0.0f){
        vec3 v = normalize(-pos.xyz);
        vec3 r = reflect(-s, n);
        spec = Material.Specular * pow(max(dot(r, v), 0.0f), Material.Shininess);
    }

    return Light.Specular * (diffuse + spec) + ambient;
}

void main() {
    vec3 LightIntensity = vec3(0.0f);
    for (int i = 0; i < 3; i++){
        LightIntensity += phongLighting(lights[i], Position, Normal);
    }
    
    

    FragColor = vec4(mix(Fog.FogColour, LightIntensity, fogCalculation()), 1.0);
}
