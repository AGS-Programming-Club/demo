#ifndef PROGRAMLOADER_HPP
#define PROGRAMLOADER_HPP

#include <string>
#include <GL/glew.h>

namespace shader {
    GLuint create(GLuint type, std::string path);
}

namespace program {
    GLuint create(GLuint* shaders, int length = 2);
}

#endif // PROGRAMLOADER_HPP
