#include "renderer.hpp"

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, const int& line)
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