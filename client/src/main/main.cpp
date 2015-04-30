#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <pthread.h>

#include <cstdlib>
#include <cassert>
#include <algorithm>

#include <vector>
#include <list>

#include "log.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "state.hpp"
#include "ipc.hpp"

using std::vector;
using std::list;
using glm::vec2;
using glm::vec4;

int thisPlayer;
int otherPlayer;

vec4 colors[2] = { vec4(1, 0, 0, 1), vec4(0, 1, 0, 1) };

vector<input::input> inputs[2];
vector<state::state> states[2];

int in_fd;
int out_fd;

void init() {
    render::init(3, 3, 4, "Test", false);

    glfwSwapInterval(1);

    input::init();

    const state::state red_initial(vec2(-0.5, 0.2), vec2(-0.4, 0), vec2(-0.6, 0), true);
    const state::state green_initial(vec2(0.5, 0.2), vec2(0.6, 0), vec2(0.4, 0), true);

    inputs[0].push_back(input::input());
    inputs[1].push_back(input::input());

    states[0].push_back(red_initial);
    states[1].push_back(green_initial);

    //green_triangle = solidTriangle::add(vec2(-0.5, 0.2), vec2(-0.4, 0), vec2(-0.6, 0), vec4(0, 1, 0, 1));
    //red_triangle = solidTriangle::add(vec2(0.5, 0.2), vec2(0.6, 0), vec2(0.4, 0), vec4(1, 0, 0, 1));
}

void* send_thread(void* param) {
    ipc::packet packet = *((ipc::packet*) param);
    usleep(200 * 1000);
    ipc::send(packet, out_fd);
    return NULL;
}

void gameLoop() {
    double delta;
    double lastFrame = glfwGetTime();
    int id = 1;
    do {
        double time = glfwGetTime();
        delta = time - lastFrame;
        lastFrame = time;

        input::input user_input = input::poll();
        inputs[thisPlayer].push_back(user_input);
        inputs[otherPlayer].push_back(input::interpolate(inputs[otherPlayer][id - 1]));

        ipc::packet out_packet(id, user_input);
        pthread_t thread;
        pthread_create(&thread, NULL, send_thread, (void*) &out_packet);
        vector<ipc::packet> recieved = ipc::recieve(in_fd);
        int lowerest = id;
        for (int i = 0; i < recieved.size(); i++) {
            ipc::packet p = recieved[i];
            inputs[otherPlayer][p.id] = p.in;
            input::interpolate_range(inputs[otherPlayer], p.id + 1, id);
            lowerest = (p.id < lowerest) ? p.id : lowerest;
        }

        state::update_range(states[0], inputs[0], lowerest, id);
        state::update_range(states[1], inputs[1], lowerest, id);

        list<solidTriangle::Data*> triangles;
        solidTriangle::Data a = state::state_to_renderer(states[0][id], colors[0]);
        solidTriangle::Data b = state::state_to_renderer(states[1][id], colors[1]);
        triangles.push_back(&a);
        triangles.push_back(&b);
        solidTriangle::remakeBuffer(triangles);

        //usleep(16 * 1000);

        // Andrew Chen: On my computer,
        // glfwSwapBuffers() automatically blocks until next screen update,
        render::tick();

        if(CHECK_GL())
            return;

        //printf("FPS: %f\n", 1.0 / fps::delta);

        id++;
    } while (render::shouldClose());
}

void cleanUp() {
    render::clean();
}

int main(int argc, char* argv[]) {
    // program player out_pipe in_pipe
    // player: 0=red, 1=green

    assert(argc == 3 + 1);
    assert(argv[1][0] == '0' || argv[1][0] == '1');
    thisPlayer = argv[1][0] - '0';
    otherPlayer = (thisPlayer + 1) % 2;

    in_fd = open(argv[3], O_RDONLY | O_NONBLOCK);
    out_fd = open(argv[2], O_WRONLY);

    init();
    gameLoop();
    cleanUp();
}
