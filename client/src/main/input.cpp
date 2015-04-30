#include "renderer.hpp"
#include "input.hpp"

#include <GLFW/glfw3.h>

namespace {

bool keypressed[1024];

void keypress_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch(action) {
        case GLFW_PRESS:
            keypressed[key] = true;
            break;
        case GLFW_RELEASE:
            keypressed[key] = false;
            break;
    }
}

}

namespace input {

void init() {
    glfwSetKeyCallback(render::getWindow(), keypress_callback);
}

input poll() {
    input res;
    res.up = keypressed['W'];
    res.down = keypressed['S'];
    res.left = keypressed['A'];
    res.right = keypressed['D'];
    res.aux1 = keypressed[' '];
    return res;
}

input interpolate(const input& old) {
    input res = old;
    res.aux1 = 0;
    res.aux2 = 0;
    res.aux3 = 0;
    res.aux4 = 0;
    return res;
}

void interpolate_range(std::vector<input> inputs, int begin, int end) {
    if (begin > end) return;
    assert (begin > 0);
    assert (inputs.size() > begin);

    for (int i = begin; i <= end; i++) {
        input new_input = interpolate(inputs[i - 1]);
        if (inputs.size() > i) inputs[i] = new_input;
        else inputs.push_back(new_input);
    }
}

}
