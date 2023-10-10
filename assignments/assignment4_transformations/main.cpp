#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/ewMath/vec3.h>
#include <ew/procGen.h>

#include <lr/transformations.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Square aspect ratio for now. We will account for this with projection later.
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 720;

//Timer
clock_t start;
float iTime = 0.0;

int NUM_CUBES = 4;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));
	int nums[3]{ 1, 2 , 3 };
	Transform cubeTransformations[4];
	for (int i = 0; i < NUM_CUBES; i++) {
		Transform t;
		t.position.x =   ( - 1 + 2 * (i % 2))/2.0;
		if (i < 2) {
			t.position.y = 1/2.0;
		}
		else {
			t.position.y = -1/2.0;
		}
		cubeTransformations[i] = t;

	}
	
	while (!glfwWindowShouldClose(window)) {
		//Update iTime
		iTime = (std::clock() - start) / (double)(CLOCKS_PER_SEC);

		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (int i = 0; i < NUM_CUBES; i++) {
			//Set uniforms
			shader.use();

			//TODO: Set model matrix uniform
			shader.setMat4("_Model", cubeTransformations[i].getModelMatrix());

			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			for (size_t i = 0; i < NUM_CUBES; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) {

					ImGui::DragFloat3("Position", &cubeTransformations[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransformations[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransformations[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}


			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

