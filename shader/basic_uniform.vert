#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

layout (location = 3) in vec4 VertexTangent;

out vec2 TexCoord;
out vec3 ViewDir;
out vec3 FragPos;

// Projection
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

// vec3 calculateLightDir(vec3 _Position, mat3 toObejctLocal){
//     vec3 totalLightDir = vec3(0.0);
//     for (int i = 0; i < lights.length(); i++){
//         vec3 lightDir = normalize(toObejctLocal * (lights[i].Position.xyz - _Position));
//         totalLightDir += lightDir;
//     }
//     return normalize(totalLightDir);
// }

void main()
{
    vec3 norm = normalize(NormalMatrix * VertexNormal);
    vec3 tangent = normalize(NormalMatrix * vec3(VertexTangent));
    vec3 binormial = normalize(cross(norm, tangent)) * VertexTangent.w;
    vec4 position = ModelViewMatrix * vec4(VertexPosition, 1.0);

    mat3 toObjectLocal = mat3(
        tangent.x, binormial.x, norm.x,
        tangent.y, binormial.y, norm.y,
        tangent.z, binormial.z, norm.z
    );

    ViewDir = toObjectLocal * normalize(vec3(-position));

    TexCoord = VertexTexCoord;
    FragPos = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));

    gl_Position = MVP * vec4(VertexPosition, 1.0f);
}
