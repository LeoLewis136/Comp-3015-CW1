#version 460

in vec2 TexCoord;
in vec3 Position;
in vec3 Normal;

in vec3 ViewPos; // The position of the camera

layout (binding = 0) uniform sampler2D albedoTexture; // GL_TEXTURE0 is object "albedo" texture
layout (binding = 1) uniform sampler2D normalTexture; // GL_TEXTURE1 is object normal texture
layout (binding = 5) uniform sampler2D RenderTex; // Second pass render texture

uniform float Weights[5];
uniform int Pass; // The current rendering pass 

layout (location = 0) out vec4 FragColor;


uniform struct LightInfo{
    // Static Lighting
    vec3 Position;
    vec3 Ambient;
    vec3 Specular;
    vec3 Diffuse;

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
    return clamp(fogFactor, 0.3f, 1.0f);
}

// pos = fragment pos in world space, n = fragent normal, _Light = current light object
vec4 phongLighting(LightInfo _Light, vec3 pos, vec3 n){
    vec4 texColour = texture(albedoTexture, TexCoord);

    if (texColour.a < 0.1){
        discard;
    }

    // -- Ambient --
    vec3 ambient = _Light.Ambient * vec3(texColour);

    // -- Diffuse --
    vec3 lightDir = normalize(_Light.Position - pos);
    vec3 normal = normalize(n);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * _Light.Diffuse;

    // Specular reflections
    vec3 viewDir = normalize(ViewPos - pos);
    
    // Blinn Phong
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 spec = pow(max(dot(normal, halfwayDir), 0.0f), Material.Shininess) * _Light.Specular;

    return vec4(ambient + (diffuse + spec) + texColour.rgb, 1.0);
    


    // vec4 diffuse = vec4(0), spec = vec4(0);
    // vec4 ambient = vec4(_Light.Ambient, 0.0f) * texColour;

    // vec3 s = normalize(((vec4(_Light.Position, 1.0f)).xyz - pos));
    // float sDotN = max(dot(s, n), 0.0f);

    // diffuse = texColour * sDotN;

    // vec4 v = vec4(normalize(ViewPos - pos.xyz), 1.0f);
    // vec4 r = vec4(reflect(-s, n), 1.0f);
    // spec = vec4(_Light.Specular, 1.0f) * pow(max(dot(v, r), 0.0f), Material.Shininess);

    // return vec4(_Light.Specular, 1.0) * (diffuse + spec) + ambient;
}

vec4 pass1(){
    vec3 norm = texture(normalTexture, TexCoord).xyz;
    //norm.xy = 2.0 * norm.xy - 1.0;
    norm = normalize(norm + Normal);

    vec4 LightIntensity = vec4(0.0f);
    for (int i = 0; i < 3; i++){
        LightIntensity += phongLighting(lights[i], Position, norm);
    }
    return mix(vec4(Fog.FogColour, 1.0), (LightIntensity / lights.length()), fogCalculation());
}

vec4 pass3(){
    ivec2 pix = ivec2(gl_FragCoord.xy);

    vec4 sum = texelFetch(RenderTex, pix, 0) * Weights[0];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(1,0)) * Weights[1];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(-1,0)) * Weights[1];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(2,0)) * Weights[2];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(-2,0)) * Weights[2];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(3,0)) * Weights[3];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(-3,0)) * Weights[3];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(4,0)) * Weights[4];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(-4,0)) * Weights[4];

    return sum;
}

vec4 pass2(){
    ivec2 pix = ivec2(gl_FragCoord.xy);

    vec4 sum = texelFetch(RenderTex, pix, 0) * Weights[0];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0,1)) * Weights[1];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0,-1)) * Weights[1];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0,2)) * Weights[2];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0,-2)) * Weights[2];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0,3)) * Weights[3];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0,-3)) * Weights[3];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0,4)) * Weights[4];
    sum += texelFetchOffset(RenderTex, pix, 0, ivec2(0,-4)) * Weights[4];

    return sum;
}

void main() {
    if (Pass == 1) { FragColor = pass1(); }
    else if (Pass == 2) { FragColor = pass2(); }
    else if (Pass == 3) { FragColor = pass3(); }
}
