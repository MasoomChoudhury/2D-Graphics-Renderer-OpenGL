// main.cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <cmath>

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Global transformation parameters (translation, rotation, scale)
glm::vec2 translation(0.0f, 0.0f);
float rotation = 0.0f;
float scale = 1.0f;

// Timing for FPS calculation
double lastTime = 0.0;
int nbFrames = 0;

// CSV log file name
const std::string fpsLogFile = "fps_log.csv";

// Vertex shader source code
const GLchar* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec2 position;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = transform * vec4(position, 0.0, 1.0);\n"
    "}\0";

// Fragment shader source code
const GLchar* fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "uniform vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "    color = vec4(ourColor, 1.0);\n"
    "}\0";

// Function to compile a shader and return its ID
GLuint compileShader(GLenum type, const GLchar* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    // Check compile errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    std::cout << "compileShader called for shader type: " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << std::endl;
    std::cout << "Shader Source Code:\n" << source << std::endl;

    if (!success)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        if (strlen(infoLog) > 0) {
            std::cout << "SHADER::COMPILATION_INFO_LOG:\n" << infoLog << std::endl;
        }
    }
    return shader;
}

// Function to create shader program
GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check linking errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // Shaders can be deleted after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

// Create rectangle data (two triangles)
void createRectangle(GLuint &VAO, GLuint &VBO) {
    GLfloat vertices[] = {
         // First triangle
         -0.5f, -0.5f,
          0.5f, -0.5f,
          0.5f,  0.5f,
         // Second triangle
          0.5f,  0.5f,
         -0.5f,  0.5f,
         -0.5f, -0.5f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
  
    glBindVertexArray(VAO);
  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
  
    glBindVertexArray(0);
}

// Create circle data (triangle fan)
void createCircle(GLuint &VAO, GLuint &VBO, int segments = 50) {
    std::vector<GLfloat> vertices;
    // Center vertex
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    float angleStep = 2.0f * 3.1415926f / segments;
    for (int i = 0; i <= segments; ++i) {
        float angle = i * angleStep;
        vertices.push_back(0.5f * cos(angle));
        vertices.push_back(0.5f * sin(angle));
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
  
    glBindVertexArray(VAO);
  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
  
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
  
    glBindVertexArray(0);
}

// Keyboard input callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Only process when key is pressed or held down
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        // Translation (arrow keys)
        if (key == GLFW_KEY_UP)
            translation.y += 0.05f;
        if (key == GLFW_KEY_DOWN)
            translation.y -= 0.05f;
        if (key == GLFW_KEY_LEFT)
            translation.x -= 0.05f;
        if (key == GLFW_KEY_RIGHT)
            translation.x += 0.05f;
        // Rotation (Q/E keys)
        if (key == GLFW_KEY_Q)
            rotation += glm::radians(5.0f);
        if (key == GLFW_KEY_E)
            rotation -= glm::radians(5.0f);
        // Scaling (Z/X keys)
        if (key == GLFW_KEY_Z)
            scale += 0.05f;
        if (key == GLFW_KEY_X)
            scale = std::max(0.05f, scale - 0.05f);
    }
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
  
    // Setup OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "2D Renderer", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
  
    // Set input callback
    glfwSetKeyCallback(window, key_callback);
  
    // Initialize GLEW (after context creation)
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }
  
    // Define viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);
  
    // Build and compile shader program
    GLuint shaderProgram = createShaderProgram();
  
    // Create rectangle and circle
    GLuint rectVAO, rectVBO;
    createRectangle(rectVAO, rectVBO);
  
    GLuint circleVAO, circleVBO;
    createCircle(circleVAO, circleVBO);
  
    // Open CSV file for appending FPS logs; add header if file is new.
    std::ofstream csvFile;
    csvFile.open(fpsLogFile, std::ios::out | std::ios::app);
    // Optionally write header (only once)
    // csvFile << "Timestamp,FPS\n";
  
    // Timing variables
    lastTime = glfwGetTime();
  
    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Calculate deltaTime and FPS
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0)
        {
            double fps = double(nbFrames) / (currentTime - lastTime);
            // Update window title with FPS
            std::stringstream ss;
            ss << "2D Renderer - FPS: " << fps;
            glfwSetWindowTitle(window, ss.str().c_str());
  
            // Log FPS with a timestamp (here using seconds from start)
            csvFile << currentTime << "," << fps << "\n";
            csvFile.flush();
  
            nbFrames = 0;
            lastTime = currentTime;
        }
  
        // Check and call events
        glfwPollEvents();
  
        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
  
        // Use shader program
        glUseProgram(shaderProgram);
  
        // Build transformation matrix using GLM
        glm::mat4 transform = glm::mat4(1.0f);
        // Scale, then rotate, then translate
        transform = glm::translate(transform, glm::vec3(translation, 0.0f));
        transform = glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(scale, scale, 1.0f));
  
        // Pass transformation matrix to shader
        GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
  
        // Draw rectangle with a blue color
        GLint colorLoc = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform3f(colorLoc, 0.0f, 0.0f, 1.0f);
        glBindVertexArray(rectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
  
        // Draw circle with a red color (translated a bit to avoid overlap)
        glm::mat4 circleTransform = transform;
        circleTransform = glm::translate(circleTransform, glm::vec3(0.8f, 0.0f, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &circleTransform[0][0]);
        glUniform3f(colorLoc, 1.0f, 0.0f, 0.0f);
        glBindVertexArray(circleVAO);
        // Number of vertices is segments+2 (center + segments + closing vertex)
        glDrawArrays(GL_TRIANGLE_FAN, 0, 52);
  
        glBindVertexArray(0);
  
        // Swap buffers
        glfwSwapBuffers(window);
    }
  
    // Cleanup
    glDeleteVertexArrays(1, &rectVAO);
    glDeleteBuffers(1, &rectVBO);
    glDeleteVertexArrays(1, &circleVAO);
    glDeleteBuffers(1, &circleVBO);
    glDeleteProgram(shaderProgram);
  
    csvFile.close();
    glfwDestroyWindow(window);
    glfwTerminate();
  
    return 0;
}
