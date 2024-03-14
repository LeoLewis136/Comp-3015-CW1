#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

// Premade for this module includes
#include "helper/torus.h"
#include "helper/scene.h"
#include "helper/objmesh.h" // Model loader

#include <glm/glm.hpp>
#include "helper/ShaderManager.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"

class SceneBasic_Uniform : public Scene
{
private:
    // Camera variables

    GLSLProgram standardShaders;

    std::unique_ptr<ObjMesh> testMesh;
    Torus testTorus;


    void setMatricies();

    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
