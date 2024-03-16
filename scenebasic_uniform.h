#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include <vector>

// Premade for this module includes
#include "helper/torus.h"
#include "helper/scene.h"
#include "helper/objmesh.h" // Model loader

#include <glm/glm.hpp>
#include "helper/ShaderManager.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include "helper/cube.h"
#include "helper/skybox.h"
#include <GLFW/glfw3.h>

using namespace glm;

class SceneBasic_Uniform : public Scene
{
private:
    // Camera variables
    vec3 cameraPosition = vec3(0.0f, 0.0f, 0.0f);
    vec3 cameraForward = vec3(0.0f, 0.0f, 2.0f);
    vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
    float yaw = 0.0f;
    float pitch = 0.0f;

    

    vec2 lastMousePos = vec2(0.0f);

    vec3 movementVec = vec3(0.0f); // The current movement vector
    float lastFrame = 0.0f; // To manage deltaTime;

    GLSLProgram standardShaders;
    GLSLProgram skyboxShaders;

    SkyBox sky;

    std::unique_ptr<ObjMesh> terrain;

    std::vector<std::unique_ptr<ObjMesh>> trees;

    // Textures
    GLuint woodTexture;
    GLuint woodNormal;

    GLuint skyboxTexture;
    GLuint treeTexture;
    GLuint terrainTexture;

    void setMatricies(GLSLProgram& currentShaders);

    void compile();
    void assignTextures(GLuint albedo, GLuint normal);
    void assignCubemap(GLuint albedo);
    void assignTexture(GLuint albedo);
    void assignMaterial(float materalShine);

    void positionModel(glm::vec3 newPosition);


public:
    SceneBasic_Uniform();

    void moveCamera(float t);
    void manageInput(GLFWwindow* myWindow);
    void MouseCallback(GLFWwindow* window, double xpos, double ypos);

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
