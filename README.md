# Arrakis

This project is a small game featuring a few ideas I wanted to try out.


It consists of:

* A game server in C++ with the game state.
* Web clients that communicate with the server through WebSockets.


It uses:

* EntityX, an Entity-Component-System framework.
* websocketspp, a C++ websockets library.


## Requirements

To build the project, you will need:

* CMake 3.1+
* C++14 compiler (tested with gcc 5.2.1)


## Build instructions

    git clone --recursive http://github.com/blito/arrakis.git


First, build EntityX (which should be automatically downloaded if you used `--recursive`).

Then,


    cd arrakis
    mkdir build && cd build
    cmake ..  # setup `ENTITY_INCLUDE_DIR`, `ENTITY_LIBRARY` and `ENTITY_LIBRARY_DEBUG` if necessary
    cmake --build .


To run,

    ./arrakis