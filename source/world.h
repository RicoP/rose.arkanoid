#pragma once

#include <raylib.h>
#include <vector>

//@Imposter Color is declared in raylib.h
/*
struct Color {
     char r;
     char g;
     char b;
     char a;
};
*/

struct Stone {
    Vector3 position = { 0.0f, 0.0f, 0.0f };
    Color color = RED;
};


//@Imposter vector3 is declared in raylib.h
/*
struct Vector3 {
	float x,y,z;
};
*/

enum class WorldState {
    NewGame,
    Running,
};

struct World {
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    Vector3 ballPosition = { 0.0f, 1.0f, 0.0f };
    Vector3 ballVelocity = { 1.0f, 1.0f, 0.0f };
    int points = 0;

    WorldState state = WorldState::NewGame;
    std::vector<Stone> stones;
};
