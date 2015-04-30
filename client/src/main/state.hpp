#ifndef STATE_HPP
#define STATE_HPP

#include <glm/glm.hpp>

#include <vector>

#include "renderer.hpp"
#include "input.hpp"

namespace state {

struct state {
    bool show;
    glm::vec2 a, b, c;
    state(glm::vec2 a, glm::vec2 b, glm::vec2 c, bool show) : a(a), b(b), c(c), show(show) {}
};

state update(state& prev, input::input& in);
void update_range(std::vector<state>& states, std::vector<input::input>& inputs, int begin, int end);
solidTriangle::Data state_to_renderer(state& s, glm::vec4 color);

}

#endif // STATE_HPP
