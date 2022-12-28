#pragma once

#include <raylib.h>

//@Imposter vector3 is declared in raylib.h
/*
struct Vector3 {
	float x,y,z;
};
*/

struct World {
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    Vector3 ballPosition = { 0.0f, 1.0f, 0.0f };
    Vector3 ballVelocity = { 1.0f, 1.0f, 0.0f };
    int points = 0;
};
