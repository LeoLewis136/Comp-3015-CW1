#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

out vec3 Position;
out vec3 LightDir;
out vec3 ViewDir;
out vec2 TexCoord;

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

// Projection
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

vec3 calculateLightDir(vec3 Position, mat3 toObjectLocal){
    vec3 totalLightDir = vec3(0.0);
    for (int i = 0; i < lights.length(); i++){
        vec3 lightDirN = normalize(toObjectLocal * (lights[i].Position.xyz - vec3(Position)));
        totalLightDir += lightDirN;
    }

    return totalLightDir / lights.length();
}

void main()
{
    vec3 norm = normalize(NormalMatrix * VertexNormal);
    vec3 tangent = normalize(NormalMatrix * vec3(VertexTangent));
    vec3 binormal = normalize(cross(norm, tangent)) * VertexTangent.w;

    mat3 toObjectLocal = mat3(
        tangent.x, binormal.x, norm.x,
        tangent.y, binormal.y, norm.y,
        tangent.z, binormal.z, norm.z
    );

    TexCoord = VertexTexCoord;

    Position = vec3(ModelViewMatrix) * VertexPosition;

    LightDir = calculateLightDir(Position, toObjectLocal);
    ViewDir = toObjectLocal * normalize(-Position);

    gl_Position = MVP * vec4(VertexPosition, 1.0f);
}
