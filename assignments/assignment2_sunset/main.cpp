#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <lr/shader.h>

unsigned int createVAO(float* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[12] = {
	//x    y    z		
	-0.8 , -0.8 , 0.0, 	//Bottom left
	 0.8 , -0.8 , 0.0, 	//Bottom right
	 0.8 ,  0.8 , 0.0, 	//Top right
	-0.8 ,  0.8 , 0.0 	//Top left
};

unsigned int indices[6] = {
	0 , 2 , 3 , //Triangle 1
	0 , 1 , 2   //Triangle 2
};




float speed = 1.5;
float skyCol[3]{ 0.1, 0.2, 0.3 };
float altSkyCol[3]{1.0, 0.7, 0.7 };

float sunCol[3]{ 1.0, 0.2, 0.3 };
float circleRadius = 1.0;

float buildingCol[3]{ 0.5, 0.5, 0.5 };

bool showImGUIDemoWindow = true;

//Timer
clock_t start;
float iTime = 0.0;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	lr::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();


	unsigned int vao = createVAO(vertices, 4, indices, 6);
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		//Update iTime
		iTime = (std::clock() - start) / (double)(CLOCKS_PER_SEC);
		shader.setFloat("iTime", iTime);
		shader.setFloat("speed", speed);

		shader.setVec3("skyCol", skyCol[0], skyCol[1], skyCol[2]);
		shader.setVec3("altSkyCol", altSkyCol[0], altSkyCol[1], altSkyCol[2]);


		shader.setVec3("sunCol", sunCol[0], sunCol[1], sunCol[2]);
		shader.setFloat("circleRadius", circleRadius);

		shader.setVec3("buildingCol", buildingCol[0], buildingCol[1], buildingCol[2]);

		bool showImGUIDemoWindow = true;

		//Polygon Mode
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		//shader.setVec3("_Color", triangleColor[0], triangleColor[1], triangleColor[2]);
		//shader.setFloat("_Brightness", triangleBrightness);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			//My varriables
			ImGui::SliderFloat("Speed", &speed, 1.0f, 2.0f);
			ImGui::ColorEdit3("skyColor", skyCol);
			ImGui::ColorEdit3("altSkyColor", altSkyCol);
			ImGui::ColorEdit3("sunColor", sunCol);
			ImGui::SliderFloat("sunSize", &circleRadius, 0.5f, 1.5f);
			ImGui::ColorEdit3("BuildingColor", buildingCol);

			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}


//Was having some issues so used a youtube video for reference
//https://www.youtube.com/watch?v=MXNMC1YAxVQ

unsigned int createVAO(float* vertexData, int numVertices, unsigned int* indicesData, int numIndices) 
{
	//vao
	unsigned int vao;
	glGenBuffers(1, &vao);
	glBindBuffer(GL_ARRAY_BUFFER, vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, vertexData, GL_STATIC_DRAW);

	//element buffer
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}