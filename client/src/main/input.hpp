#ifndef INPUT_HPP
#define INPUT_HPP

#include <vector>

namespace input {

struct input {
    bool up;
    bool down;
    bool left;
    bool right;
    bool aux1;
    bool aux2;
    bool aux3;
    bool aux4;
};

void init();
input poll();
input interpolate(const input& old);
void interpolate_range(std::vector<input> inputs, int begin, int end);

}

#endif // INPUT_HPP
