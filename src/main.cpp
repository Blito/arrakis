#include <entityx/entityx.h>

#include <core/game.h>

constexpr int default_server_port = 9002;

int main(int argc, const char * argv[])
{
    using namespace arrakis::core;

    Game game(argc > 1? std::atoi(argv[1]) : default_server_port);
    game.run();

    return 0;
}
