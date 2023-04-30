#include "WindowLayer.h"

WindowLayer::WindowLayer(const GLuint width, const GLuint height) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(width, height, "LayerWindow", nullptr, nullptr);
	if (window == nullptr) {
		//std::cerr << "Failed to initialize GLFW." << std::endl;
		return;
	}
	glfwMakeContextCurrent(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext(NULL);
	ImGuiIO& io = ImGui::GetIO();
	/*ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);*/

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	glew = new GLPtrMgr();

	// viewport
	int v_width, v_height;
	glfwGetFramebufferSize(window, &v_width, &v_height);
	this->ScreenWidth = v_width;
	this->ScreenHeight = v_height;
	glViewport(0, 0, v_width, v_height);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void WindowLayer::GameLoop() {
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	//ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwPollEvents();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void WindowLayer::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void WindowLayer::Terminate()
{
	glfwTerminate();
}

GLFWwindow* WindowLayer::GetWindowobj()
{
	return window;
}
