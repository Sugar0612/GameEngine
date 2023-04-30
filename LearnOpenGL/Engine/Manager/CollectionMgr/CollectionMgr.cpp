#include "CollectionMgr.h"

namespace MouseDevice {
	float curXpos = 0.f;
	float curYpos = 0.f;
	float cur_lastX = 400.f;
	float cur_lastY = 300.f;
	float sensitivity = 0.2f;

	bool firstMouse = true;
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		curXpos = static_cast<float>(xpos);
		curYpos = static_cast<float>(ypos);
	}

	float _yoffset = 0.f;
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		_yoffset = static_cast<float>(yoffset);
	}
}

namespace LightData {
	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
	glm::vec3 lightColor;
}

CollectionMgr::CollectionMgr()
{
	layer = new WindowLayer(1200, 600);
	texture = new Texture();
	camera = new Camera();

	cubeShader = new Shader("./Shader/VertexSource.vert", "./Shader/FragmentSource.frag");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	texture->LoadImg("../Resources/Texture/container2.png", "PNG");
	texture->LoadImg("../Resources/Texture/container2_specular.png", "PNG");
	texture->LoadImg("../Resources/Texture/awesomeface.png", "PNG");

	cubeShader->BindVAO(0);
	cubeShader->BindVBO(0);
	cubeShader->UseProgram();
	cubeShader->SetUniform1i("material.diffuse", 0);
	cubeShader->SetUniform1i("material.specular", 1);
	cubeShader->SetUniform1i("material.emission", 2);

	lightShader = new Shader("./Shader/LightVertSource.vert", "./Shader/LightFragSource.frag");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	lightShader->BindVAO(0);
	lightShader->BindVBO(0);
	lightShader->UseProgram();

	float sc_width = layer->GetScreenWidth(), sc_height = layer->GetScreenHeight();

	glfwSetCursorPosCallback(layer->GetWindowobj(), MouseDevice::mouse_callback);
	glfwSetScrollCallback(layer->GetWindowobj(), MouseDevice::scroll_callback);
}

void CollectionMgr::Start() {
	while (!glfwWindowShouldClose(layer->GetWindowobj())) {
		proessInput(layer->GetWindowobj());
		_mouse_callback();
		_scroll_callback();
		layer->GameLoop();

		cubeShader->UseProgram();
		// direction light.
		cubeShader->SetUniform3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		cubeShader->SetUniform3f("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
		cubeShader->SetUniform3f("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		cubeShader->SetUniform3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		// Point lights.
		for (int i = 0; i < 4; ++i) {
			std::string name = std::string("pointlights[");
			name.push_back(i + '0');
			name.push_back(']');

			cubeShader->SetUniform3f(std::string(name +".position").c_str(), pointLightPositions[i]);

			cubeShader->SetUniform3f(std::string(name + ".ambient").c_str(), glm::vec3(0.05f, 0.05f, 0.05f));
			cubeShader->SetUniform3f(std::string(name + ".diffuse").c_str(), glm::vec3(0.8f, 0.8f, 0.8f));
			cubeShader->SetUniform3f(std::string(name + ".specular").c_str(), glm::vec3(1.0f, 1.0f, 1.0f));
													  
 			cubeShader->SetUniform1f(std::string(name + ".constant").c_str(), 1.f);
			cubeShader->SetUniform1f(std::string(name + ".linear").c_str(), 0.09f);
			cubeShader->SetUniform1f(std::string(name + ".quadratic").c_str(), 0.032f);
		}

		// Spot light.
		cubeShader->SetUniform3f("spotlight.position", camera->GetCameraPos());
		cubeShader->SetUniform3f("spotlight.direction", camera->GetCameraFront());
		cubeShader->SetUniform3f("spotlight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		cubeShader->SetUniform3f("spotlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		cubeShader->SetUniform3f("spotlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	
		cubeShader->SetUniform1f("spotlight.cutOff", glm::cos(glm::radians(12.5f)));
		cubeShader->SetUniform1f("spotlight.outerCutOff", glm::cos(glm::radians(15.0f)));
		cubeShader->SetUniform1f("spotlight.constant", 1.0f);
		cubeShader->SetUniform1f("spotlight.linear", 0.09f);
		cubeShader->SetUniform1f("spotlight.quadratic", 0.032f);

		cubeShader->SetUniform3f("CameraPos", camera->GetCameraPos());
		cubeShader->SetUniform1f("material.shininess", 32.0f);

		glm::mat4 projection;
		float sc_width = layer->GetScreenWidth(), sc_height = layer->GetScreenHeight();
		projection = glm::perspective(glm::radians(camera->GetFov()), sc_width / sc_height, 0.1f, 100.f);
		cubeShader->SetUniformMatx4("projection", projection);
		cubeShader->SetUniformMatx4("view", camera->GetCameraView());

		glm::mat4 model;
		model = glm::mat4(1.0f);
		cubeShader->SetUniformMatx4("model", model);
		
		texture->GameLoop();

		cubeShader->BindVAO(cubeShader->GetVAO());

		for (int i = 0; i < 10; ++i) {
			glm::mat4 model = glm::mat4(1.0f);;
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			cubeShader->SetUniformMatx4("model", model);
			cubeShader->DrawArrays(36);
		}

		
		lightShader->UseProgram();
		lightShader->SetUniformMatx4("projection", projection);
		lightShader->SetUniformMatx4("view", camera->GetCameraView());
		for (int i = 0; i < 4; ++i) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader->SetUniformMatx4("model", model);
			lightShader->BindVAO(lightShader->GetVAO());
			lightShader->DrawArrays(36);
		}

		layer->SwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//删除之前分配的所有资源
	cubeShader->DeleteShader();
	layer->Terminate();
}

void CollectionMgr::proessInput(GLFWwindow* window) {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glm::vec3 dist{};
	camera->Speed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		dist = camera->GetCameraFront() * camera->Speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		dist = camera->GetCameraFront() * camera->Speed * -1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		dist = glm::normalize(glm::cross(camera->GetCameraFront(), camera->GetCameraUp())) * camera->Speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		dist = glm::normalize(glm::cross(camera->GetCameraFront(), camera->GetCameraUp())) * camera->Speed * -1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		dist = camera->GetCameraUp() * camera->Speed * -1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		dist = camera->GetCameraUp() * camera->Speed;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//printf("mouse front: [%.2f, %.2f, %.2f], mouse position:[%.2f, %.2f, %.2f].\n", camera->GetCameraFront().x, camera->GetCameraFront().y, camera->GetCameraFront().z, camera->GetCameraPos().x, camera->GetCameraPos().y, camera->GetCameraPos().z);
	camera->SetCameraView(dist);
}

void CollectionMgr::_mouse_callback()
{
	float sensitivity = MouseDevice::sensitivity;
	float xpos = MouseDevice::curXpos;
	float ypos = MouseDevice::curYpos;

	if (MouseDevice::firstMouse) {
		MouseDevice::cur_lastX = xpos;
		MouseDevice::cur_lastY = ypos;
		MouseDevice::firstMouse = false;
	}

	float xoffset = xpos - MouseDevice::cur_lastX;
	float yoffset = MouseDevice::cur_lastY - ypos;
	MouseDevice::cur_lastX = xpos;
	MouseDevice::cur_lastY = ypos;
	
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	camera->SetCameraFront(xoffset, yoffset);
}

void CollectionMgr::_scroll_callback()
{
	camera->SetFov(MouseDevice::_yoffset);
	MouseDevice::_yoffset = 0;
}
