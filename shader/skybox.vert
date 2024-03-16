#version 460

layout (location = 0) in vec3 VertexPosition;

out vec3 Vec;
out vec3 Position;

uniform mat4 MVP;
uniform mat4 ModelViewMatrix;

void main(){
    Position = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;
    Vec = VertexPosition;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}