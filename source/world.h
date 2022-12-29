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



namespace rose {
  namespace ecs {
    void        serialize(Color &o, ISerializer &s) {
        unsigned long long l = 0;
        l ^= o.a;
        l <<= 8;
        l ^= o.b;
        l <<= 8;
        l ^= o.g;
        l <<= 8;
        l ^= o.r;
        
        ::rose::ecs::serialize(l, s);
    }
    void      deserialize(Color &o, IDeserializer &s) {
        unsigned long long l = 0;
        ::rose::ecs::deserialize(l, s);

        o.r = (unsigned char)(l & 0xFF);
        l >>= 8;
        o.g = (unsigned char)(l & 0xFF);
        l >>= 8;
        o.b = (unsigned char)(l & 0xFF);
        l >>= 8;
        o.a = (unsigned char)(l & 0xFF);
    }
  }
}


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


namespace rose {
  namespace ecs {
    void        serialize(Vector3 &o, ISerializer &s) {
        float v[3] = {o.x, o.y, o.z};
        ::rose::ecs::serialize(v, s);
    }
    void      deserialize(Vector3 &o, IDeserializer &s) {
        float v[3] = {0,0,0};
        ::rose::ecs::deserialize(v, s);
        o.x = v[0];
        o.y = v[1];
        o.z = v[2];
    }
  }
}

enum class WorldState {
    NewGame,
    Running,
    Paused,
};

struct World {
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    Vector3 ballPosition = { 0.0f, 1.0f, 0.0f };
    Vector3 ballVelocity = { 1.0f, 1.0f, 0.0f };
    int points = 0;

    WorldState state = WorldState::NewGame;
    std::vector<Stone> stones;
};
