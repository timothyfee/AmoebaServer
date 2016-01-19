#include "GameLib.hpp"

int main(int argc, char *argv[])
{
    int maxConnections = argc > 1 ? toInt(argv[1]) : 10;
    int port = argc > 2 ? toInt(argv[2]) : 8080;
    cout << "Maximum allowed connections: " << maxConnections << endl;
    cout << "Working on port: " << port << endl;

    AmoebaNetworkServerController networkController(maxConnections, port);
    networkController.start();

    GameThread game(networkController.incomingMailer, networkController.outgoingMailer, networkController.connectionPool);
    game.run(Thread::infiniteLoop, 15);
}
