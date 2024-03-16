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
#include "helper/texture.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/vector_float3.hpp>

using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : sky(100) {}

void SceneBasic_Uniform::initScene()
{
	compile();

	model = mat4(1.0f);
	view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);

	
	/* --- Unused --- multiple lights in scene
	To be reimplemented when normal mapping is complete */
	standardShaders.use();
	// ---- Point light setup ---- //
	standardShaders.setUniform("lights[0].Position", vec3(5.0f, 5.0f, 5.0f));
	standardShaders.setUniform("lights[0].Position", vec3(-5.0f, 5.0f, 5.0f));
	standardShaders.setUniform("lights[0].Position", vec3(-5.0f, 5.0f, -5.0f));

	// Scene light values
	// Specular
	standardShaders.setUniform("lights[0].Specular", vec3(0.2f, 0.0f, 0.0f)); // LD
	standardShaders.setUniform("lights[1].Specular", vec3(0.0f, 0.2f, 0.0f)); // LD
	standardShaders.setUniform("lights[2].Specular", vec3(0.0f, 0.0f, 0.2f)); // LD

	// Ambient
	standardShaders.setUniform("lights[0].Ambient", vec3(0.2f, 0.0f, 0.0f)); // LA
	standardShaders.setUniform("lights[1].Ambient", vec3(0.0f, 0.2f, 0.0f)); // LA
	standardShaders.setUniform("lights[2].Ambient", vec3(0.0f, 0.0f, 0.2f)); // LA
	
	// --- Temporary --- Single light source uniform setup
	/*standardShaders.setUniform("Light.Position", vec3(5.0f, 5.0f, 5.0f));
	standardShaders.setUniform("Light.Specular", vec3(1.0f, 1.0f, 1.0f));
	standardShaders.setUniform("Light.Ambient", vec3(0.2f));*/

	// Fog Setup
	standardShaders.setUniform("Fog.FogColour", vec3(0.2, 0.2, 0.2));
	standardShaders.setUniform("Fog.MinDist", 10.0f);
	standardShaders.setUniform("Fog.MaxDist", 30.0f);

	skyboxShaders.use();
	skyboxShaders.setUniform("Fog.FogColour", vec3(0.2, 0.2, 0.2));
	skyboxShaders.setUniform("Fog.MinDist", 10.0f);
	skyboxShaders.setUniform("Fog.MaxDist", 30.0f);



	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
	
	terrain = ObjMesh::load("media/terrain.obj", true);

	for (int i = 0; i < 10; i++) {
		trees.push_back(ObjMesh::load("media/tree.obj", true));
	}

	woodTexture = Texture::loadTexture("media/texture/WoodTexture.jpg");
	woodNormal = Texture::loadTexture("media/texture/WoodNormal.jpg");
	skyboxTexture = Texture::loadCubeMap("media/texture/Skybox/vz_sinister", ".png");
	treeTexture = Texture::loadTexture("media/texture/Tree.png");
	terrainTexture = Texture::loadTexture("media/texture/Grass.png");
}

// * Unused * 
void SceneBasic_Uniform::compile()
{

	// Compiling a standard shader used for basic lit 3d objects
	try {
		standardShaders.compileShader("shader/basic_uniform.vert");
		standardShaders.compileShader("shader/basic_uniform.frag");
		standardShaders.link();


		skyboxShaders.compileShader("shader/skybox.vert");
		skyboxShaders.compileShader("shader/skybox.frag");
		skyboxShaders.link();
	}
	catch (GLSLProgramException& e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}


void SceneBasic_Uniform::setMatricies(GLSLProgram& currentShaders) {
	mat4 mv = view * model;
	currentShaders.setUniform("ModelViewMatrix", mv);
	currentShaders.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	currentShaders.setUniform("MVP", projection * mv);
}

// Game update function
void SceneBasic_Uniform::update( float t )
{
	float delta = t - lastFrame;
	lastFrame = t;

	moveCamera(t);
}

void SceneBasic_Uniform::render()
{
	model = mat4(1.0f);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	skyboxShaders.use(); 
	positionModel(cameraPosition);
	setMatricies(skyboxShaders); 
	// Skybox rendering
	assignCubemap(skyboxTexture); 
	sky.render(); 

	standardShaders.use();
	positionModel(vec3(0.0f));
	setMatricies(standardShaders);
	assignMaterial(2.0f);
	// Standard obejct rendering
	assignTexture(terrainTexture);
	terrain->render();

	assignTexture(treeTexture);
	for (int i = 0; i < trees.size(); i++) {
		trees[i]->render();
		model = glm::translate(model, vec3(2.0f, 0.0f, -4.0f));
		setMatricies(standardShaders);
	}
	
	//testTorus.render();
}

void SceneBasic_Uniform::manageInput(GLFWwindow* myWindow) {
	movementVec = vec3(0.0f);

	if (glfwGetKey(myWindow, GLFW_KEY_W) == GLFW_PRESS) {
		movementVec.z = 1; }
	if (glfwGetKey(myWindow, GLFW_KEY_S) == GLFW_PRESS) {
		movementVec.z = -1; }

	if (glfwGetKey(myWindow, GLFW_KEY_A) == GLFW_PRESS) {
		movementVec.x = 1; }
	if (glfwGetKey(myWindow, GLFW_KEY_D) == GLFW_PRESS) {
		movementVec.x = -1; }
}

void SceneBasic_Uniform::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	//Sets values for change in position since last frame to current frame
	float xOffset = (float)xpos - lastMousePos.x;
	float yOffset = lastMousePos.y - (float)ypos;

	//Sets last positions to current positions for next frame
	lastMousePos.x = (float)xpos;
	lastMousePos.y = (float)ypos;

	//Moderates the change in position based on sensitivity value
	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	//Adjusts yaw & pitch values against changes in positions
	yaw += xOffset;
	pitch += yOffset;

	//Prevents turning up & down beyond 90 degrees to look backwards
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	//Modification of direction vector based on mouse turning
	vec3 direction;
	direction.x = cos(radians(yaw)) * cos(radians(pitch));
	direction.y = sin(radians(pitch));
	direction.z = sin(radians(yaw)) * cos(radians(pitch));
	cameraForward = normalize(direction);

	// Calculate the new up vector based on the current forward and world up vector
	vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = normalize(cross(worldUp, cameraForward));
	cameraUp = normalize(cross(cameraForward, right));
}

void SceneBasic_Uniform::moveCamera(float t) {
	if (movementVec.z != 0) { cameraPosition += (cameraForward * movementVec.z) / t; }
	if (movementVec.x != 0) { cameraPosition -= normalize(cross((cameraForward * movementVec.x), cameraUp)) / t; }

	model = mat4(1.0f);
	view = glm::lookAt(cameraPosition, cameraPosition + cameraForward, cameraUp);
}

void SceneBasic_Uniform::positionModel(vec3 newPosition) {
	model = mat4(1.0f);
	model = translate(model, newPosition);
}

void SceneBasic_Uniform::assignMaterial(float shininess) {
	standardShaders.setUniform("Material.Shininess", shininess); // Shininess
}

void SceneBasic_Uniform::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	

	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::assignTextures(GLuint albedo, GLuint normal) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal);
}

void SceneBasic_Uniform::assignCubemap(GLuint albedo) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, albedo);
}

void SceneBasic_Uniform::assignTexture(GLuint albedo) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo);
}