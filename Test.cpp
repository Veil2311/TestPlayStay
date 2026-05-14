#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <vector>

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

class ShaderProgram {
public:
	ShaderProgram(const char* vertex_source, const char* fragment_source) {
		GLuint vertex = Compile(GL_VERTEX_SHADER, vertex_source);
		GLuint fragment = Compile(GL_FRAGMENT_SHADER, fragment_source);

		program_ = glCreateProgram();
		glAttachShader(program_, vertex);
		glAttachShader(program_, fragment);
		glLinkProgram(program_);

		int success = 0;
		glGetProgramiv(program_, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512] = {0};
			glGetProgramInfoLog(program_, sizeof(infoLog), nullptr, infoLog);
			std::cerr << "Program link failed: " << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	~ShaderProgram() {
		if (program_ != 0) {
			glDeleteProgram(program_);
		}
	}

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;

	ShaderProgram(ShaderProgram&& other) noexcept {
		program_ = other.program_;
		other.program_ = 0;
	}
	ShaderProgram& operator=(ShaderProgram&& other) noexcept {
		if (this != &other) {
			if (program_ != 0) {
				glDeleteProgram(program_);
			}
			program_ = other.program_;
			other.program_ = 0;
		}
		return *this;
	}

	void Use() const { glUseProgram(program_); }
	GLuint Id() const { return program_; }

private:
	GLuint program_ = 0;

	GLuint Compile(GLenum type, const char* source) {
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
};

class TriangleMesh {
public:
	TriangleMesh() {
		const float vertices[] = {
			// positions        // colors
			 0.0f,  0.5f, 0.0f,  1.0f, 0.2f, 0.2f,
			 0.5f, -0.5f, 0.0f,  0.2f, 1.0f, 0.2f,
			-0.5f, -0.5f, 0.0f,  0.2f, 0.2f, 1.0f
		};

		glGenVertexArrays(1, &vao_);
		glGenBuffers(1, &vbo_);

		glBindVertexArray(vao_);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	~TriangleMesh() {
		if (vbo_ != 0) {
			glDeleteBuffers(1, &vbo_);
		}
		if (vao_ != 0) {
			glDeleteVertexArrays(1, &vao_);
		}
	}

	TriangleMesh(const TriangleMesh&) = delete;
	TriangleMesh& operator=(const TriangleMesh&) = delete;

	TriangleMesh(TriangleMesh&& other) noexcept {
		vao_ = other.vao_;
		vbo_ = other.vbo_;
		other.vao_ = 0;
		other.vbo_ = 0;
	}
	TriangleMesh& operator=(TriangleMesh&& other) noexcept {
		if (this != &other) {
			if (vbo_ != 0) {
				glDeleteBuffers(1, &vbo_);
			}
			if (vao_ != 0) {
				glDeleteVertexArrays(1, &vao_);
			}
			vao_ = other.vao_;
			vbo_ = other.vbo_;
			other.vao_ = 0;
			other.vbo_ = 0;
		}
		return *this;
	}

	void Draw() const {
		glBindVertexArray(vao_);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

private:
	GLuint vao_ = 0;
	GLuint vbo_ = 0;
};

class GlfwWindow {
public:
	GlfwWindow(int width, int height, const char* title) {
		if (!glfwInit()) {
			std::cerr << "Failed to initialize GLFW." << std::endl;
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (!window_) {
			std::cerr << "Failed to create GLFW window." << std::endl;
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(window_);
		glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);
	}

	~GlfwWindow() {
		if (window_) {
			glfwDestroyWindow(window_);
		}
		glfwTerminate();
	}

	GlfwWindow(const GlfwWindow&) = delete;
	GlfwWindow& operator=(const GlfwWindow&) = delete;

	GLFWwindow* Get() const { return window_; }
	bool IsValid() const { return window_ != nullptr; }

private:
	GLFWwindow* window_ = nullptr;
};

class Application {
public:
	int Run() {
		GlfwWindow window(800, 600, "OpenGL Triangle");
		if (!window.IsValid()) {
			return 1;
		}

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
			std::cerr << "Failed to initialize GLAD." << std::endl;
			return 1;
		}

		ShaderProgram program(kVertexShader, kFragmentShader);
		TriangleMesh mesh;

		while (!glfwWindowShouldClose(window.Get())) {
			if (glfwGetKey(window.Get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window.Get(), GLFW_TRUE);
			}

			glClearColor(0.08f, 0.1f, 0.14f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			program.Use();
			mesh.Draw();

			glfwSwapBuffers(window.Get());
			glfwPollEvents();
		}

		return 0;
	}
};
}  // namespace

int main() {
	Application app;
	return app.Run();
}
