#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include "GL/glew.h"


#define GLCall(x)   GLClearError();\
                    x;\
                    assert(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogCall(const char* function, const char* file, const int& line);

#endif
