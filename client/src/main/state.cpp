#include <cassert>

#include "state.hpp"

using glm::vec2;
using glm::vec4;

using std::vector;

namespace state {
    state update(state& prev, input::input& in) {
        state res = prev;
        vec2* to_mod[3] = { &res.a, &res.b, &res.c };
        for (int j = 0; j < 3; j++) {
            to_mod[j]->x += (in.right - in.left) * 0.01;
            to_mod[j]->y += (in.up - in.down) * 0.01;
        }
        res.show ^= in.aux1;
        return res;
    }

    void update_range(vector<state>& states, vector<input::input>& inputs, int begin, int end) {
        if (begin > end) return;
        assert (begin > 0);
        assert (states.size() >= begin);
        assert (inputs.size() > end);

        for (int i = begin; i <= end; i++) {
            state new_state = update(states[i - 1], inputs[i]);
            if (states.size() > i) states[i] = new_state;
            else states.push_back(new_state);
        }
    }

    solidTriangle::Data state_to_renderer(state& s, glm::vec4 color) {
        solidTriangle::Data res;
        if (s.show) {
            res.a = s.a;
            res.b = s.b;
            res.c = s.c;
        } else {
            res.a = res.b = res.c = vec2(0.0, 0.0);
        }
        res.colour = color;
        return res;
    }
}
