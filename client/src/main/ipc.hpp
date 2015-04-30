#ifndef IPC_HPP
#define IPC_HPP

#include "input.hpp"

#include <vector>

namespace ipc {

struct packet {
    int id;
    input::input in;
    packet() : id(0), in() {}
    packet(int id, input::input in) : id(id), in(in) {}
};

void send(packet p, int fd);
std::vector<packet> recieve(int fd);

}

#endif // IPC_HPP
