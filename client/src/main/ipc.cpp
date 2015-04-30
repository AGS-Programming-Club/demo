#include "ipc.hpp"

#include <cassert>

#include <unistd.h>
#include <sys/file.h>

#include <vector>

using std::vector;

namespace ipc {

    void send(packet p, int fd) {
        flock(fd, LOCK_EX);

        write(fd, &p, sizeof(packet));

        flock(fd, LOCK_UN);
    }


    vector<packet> recieve(int fd) {
        assert(fcntl(fd, F_GETFL, 0) & O_NONBLOCK);

        flock(fd, LOCK_EX);

        vector<packet> res;

        packet buf;
        while (read(fd, &buf, sizeof(packet)) == sizeof(packet)) {
            res.push_back(buf);
        }

        flock(fd, LOCK_UN);

        return res;
    }
}
