#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

out vec2 TexCoord;
out vec3 LightDir;
out vec3 ViewDir;
out vec3 CalculatedVertexNormal;

uniform struct LightInfo{
    // Static Lighting
    vec3 Position;
    vec3 Ambient;
    vec3 Specular;

} Light;

// Projection
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
    vec3 norm = normalize(NormalMatrix * VertexNormal);
    
    vec3 tangent = normalize(NormalMatrix * vec3(VertexTangent));
    vec3 binormial = normalize(cross(norm, tangent)) * VertexTangent.w;
    vec3 position = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;

    mat3 toObjectLocal = mat3(
        tangent.x, binormial.x, norm.x,
        tangent.y, binormial.y, norm.y,
        tangent.z, binormial.z, norm.z
    );
    norm = normalize(toObjectLocal * norm);

    

    TexCoord = VertexTexCoord;
    LightDir = normalize(toObjectLocal * (Light.Position.xyz - position));
    ViewDir = normalize(toObjectLocal * normalize(-position));
    CalculatedVertexNormal = normalize(NormalMatrix * VertexNormal);

    gl_Position = MVP * vec4(VertexPosition, 1.0f);
}
