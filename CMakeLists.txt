cmake_minimum_required(VERSION 4.1)
project(net8)

set(CMAKE_CXX_STANDARD 26)

add_executable(net8 net8_server/main.cpp
        net8_server/Server.cpp
        net8_server/Server.h
        net8_server/Game.cpp
        net8_server/Game.h
        net8_server/Deck.cpp
        net8_server/Deck.h
        net8_server/Card.cpp
        net8_server/Card.h
        net8_server/Player.cpp
        net8_server/Player.h
        net8_server/Pile.cpp
        net8_server/Pile.h
        net8_server/Rooms.cpp
        net8_server/Rooms.h
        net8_server/Players.cpp
        net8_server/Players.h
        net8_server/Playground.cpp
        net8_server/Playground.h)
