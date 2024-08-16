#include "server.hpp"

int main(int argc, char *argv[]){
    if(argc != 2){
        std::cerr << "Usage:\n\t" << argv[0] << " port" << std::endl;
    }
    uint16_t port = atoi(argv[1]);

    ServerTcp svr(port);
    svr.init();
    svr.loop();
    return 0;
}