#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace {
const char* kVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

void main() {
	gl_Position = vec4(aPos, 1.0);
	vColor = aColor;
}
)";

const char* kFragmentShader = R"(
#version 330 core
in vec3 vColor;
out vec4 FragColor;

void main() {
	FragColor = vec4(vColor, 1.0);
}
)";

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	(void)window;
	glViewport(0, 0, width, height);
}

GLuint CompileShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512] = {0};
		glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "Shader compile failed: " << infoLog << std::endl;
	}

	return shader;
}

GLuint CreateShaderProgram() {
	GLuint vertex = CompileShader(GL_VERTEX_SHADER, kVertexShader);
	GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, kFragmentShader);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512] = {0};
		glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "Program link failed: " << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return program;
}
}  // namespace

int main() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW." << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create GLFW window." << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cerr << "Failed to initialize GLAD." << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	float vertices[] = {
		// positions        // colors
		 0.0f,  0.5f, 0.0f,  1.0f, 0.2f, 0.2f,
		 0.5f, -0.5f, 0.0f,  0.2f, 1.0f, 0.2f,
		-0.5f, -0.5f, 0.0f,  0.2f, 0.2f, 1.0f
	};

	GLuint vao = 0;
	GLuint vbo = 0;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint program = CreateShaderProgram();

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		glClearColor(0.08f, 0.1f, 0.14f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
