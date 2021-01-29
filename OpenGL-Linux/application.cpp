#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdint>
#include <cassert>
#include <string>
#include <sstream>
#include <fstream>

#define GLCall(x)   GLClearError();\
                    x;\
                    assert(GLLogCall(#x, __FILE__, __LINE__));

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, const int& line)
{
    while (GLenum error = glGetError())
    {
        std::cout   << "[OpenGL Error] (0x"
                    << std::hex << error 
                    << std::dec << ") " 
                    << function << ' '
                    << file << ':'
                    << line
                    << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource parseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

    std::stringstream vs, fs;
    std::string line;
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
            (bool)type ? fs << line << '\n' : vs << line << '\n';
    }
    return {vs.str(), fs.str()};
}

static uint32_t compileShader(uint32_t type, const std::string& source)
{
    uint32_t id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1u, &src, nullptr);
    glCompileShader(id);

    int32_t result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        int32_t length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader:" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        delete[] message;
        return 0;
    }

    return id;
}

static uint32_t createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    uint32_t program = glCreateProgram();
    uint32_t vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{   
    GLFWwindow *window;

    if (!glfwInit())
        return EXIT_FAILURE;

    window = glfwCreateWindow(1280, 720, "Hello OpenGL", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return EXIT_FAILURE;
    }

    //std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = 
    {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    uint32_t indicies[] = 
    {
        0, 1, 2,
        2, 3, 0
    };

    uint32_t bufferID;
    GLCall(glGenBuffers(1, &bufferID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6*2*sizeof(float), positions, GL_STATIC_DRAW));
    
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0));
    

    uint32_t ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(uint32_t), indicies, GL_STATIC_DRAW));

    ShaderProgramSource source = parseShader("../res/basic.glsl");

    uint32_t shader = createShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));
    
    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return EXIT_SUCCESS;
}