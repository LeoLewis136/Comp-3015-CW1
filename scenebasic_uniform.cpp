#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

// Helper includes
#include "helper/glutils.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/vector_float3.hpp>

using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : testTorus(0.7f, 0.3f, 30, 30){}

void SceneBasic_Uniform::initScene()
{
	compile();

	model = mat4(1.0f);
	view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);
	
	model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(15.0f), vec3(0.0f, 1.0f, 0.0f));

	

	// ---- Point light setup ---- //
	for (int i = 0; i < 3; i++) {
		std::stringstream name;
		float x, z;

		name << "lights[" << i << "].Position";
		x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
		z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);

		standardShaders.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
	}

	// Scene light values
	// Specular
	standardShaders.setUniform("lights[0].Specular", vec3(0.0f, 0.0f, 0.8f)); // LD
	standardShaders.setUniform("lights[1].Specular", vec3(0.0f, 0.8f, 0.0f)); // LD
	standardShaders.setUniform("lights[2].Specular", vec3(1.0f, 0.0f, 0.0f)); // LD

	// Ambient
	standardShaders.setUniform("lights[0].Ambient", vec3(0.0f, 0.0f, 0.2f)); // LA
	standardShaders.setUniform("lights[1].Ambient", vec3(0.0f, 0.2f, 0.0f)); // LA
	standardShaders.setUniform("lights[2].Ambient", vec3(0.2f, 0.0f, 0.0f)); // LA

	// Fog Setup
	standardShaders.setUniform("Fog.FogColour", vec3(0.2, 0.2, 0.2));
	standardShaders.setUniform("Fog.MinDist", 0.0f);
	standardShaders.setUniform("Fog.MaxDist", 2.0f);

	// Standard material values
	standardShaders.setUniform("Material.Colour", vec3(0.2f, 0.55f, 0.9f)); // KD
	standardShaders.setUniform("Material.Specular", vec3(0.8f, 0.8f, 0.8f)); // KS 
	standardShaders.setUniform("Material.Ambient", vec3(0.2f, 0.55f, 0.9f)); // KA 
	standardShaders.setUniform("Material.Shininess", 100.0f); // Shininess


	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
	
	testMesh = ObjMesh::load("media/port.obj", true);

}

// * Unused * 
void SceneBasic_Uniform::compile()
{

	// Compiling a standard shader used for basic lit 3d objects
	try {
		standardShaders.compileShader("shader/basic_uniform.vert");
		standardShaders.compileShader("shader/basic_uniform.frag");

		standardShaders.link();
		standardShaders.use();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}


void SceneBasic_Uniform::setMatricies() {
	mat4 mv = view * model;
	standardShaders.setUniform("ModelViewMatrix", mv);
	standardShaders.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	standardShaders.setUniform("MVP", projection * mv);
}

// Game update function
void SceneBasic_Uniform::update( float t )
{
	float temp = t + 10;
}

void SceneBasic_Uniform::render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	setMatricies();

	testMesh->render();
	//testTorus.render();
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	

	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}
