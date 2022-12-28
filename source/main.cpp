#include <raylib.h>
#include <rlgl.h>
#include <rose.h>
#include <rose/event.h>
#include <rose/hash.h>
#include <roseio.h>
#include <imgui.h>
#include <components/padevents.h>
#include <components/components.h>
#include <components/components_ser.h>
#include <serializer/imguiserializer.h>

#include "world.h"
#define IMPL_SERIALIZER
#include "world.serializer.h"

World world;
Camera3D camera = { 0 };
float ballSpeed = .3f;

static Vector3 operator*(Vector3 lhs, float rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

static Vector3 operator+(Vector3 lhs, Vector3  rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

static Vector3 operator-(Vector3 lhs, Vector3  rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

static Vector3 & operator+=(Vector3 & lhs, Vector3 rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}


ROSE_EXPORT void postload() {
    // Define the camera to look into our 3d world
    camera.position = { 0.0f, 10.0f, 30.0f }; // Camera position
    camera.target = { 0.0f, 10.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type
    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    rose::io::json::read<World>(world, rose::io::Folder::Working, "game_state.json");
}

ROSE_EXPORT void predestroy() {
    rose::io::json::write(world, rose::io::Folder::Working, "game_state.json");
}

void update() {
    world.cubePosition.x += rose::input::stick().x;
    world.ballPosition += world.ballVelocity * ballSpeed;

    if(world.cubePosition.x < -11.5) world.cubePosition.x = -11.5;
    if(world.cubePosition.x > 11.5) world.cubePosition.x = 11.5;

    if(world.ballPosition.x < -13) world.ballVelocity.x *= -1;
    if(world.ballPosition.x > 13) world.ballVelocity.x *= -1;
    if(world.ballPosition.y > 20) world.ballVelocity.y *= -1;
    
    if(world.ballPosition.y < 0) {
        world.points -= 1;

        world.ballPosition = world.cubePosition + Vector3 {0.0f, 1.0f, 0.0f};
        auto rand = rose::hash_from_clock();
        float vx = rose::nextf(rand) * 2 - 1;
        world.ballVelocity = Vector3 {vx, 1.0, 0};
    }
    else if(world.ballPosition.y < 1 && world.ballVelocity.y < 0) {
        if(world.ballPosition.x > world.cubePosition.x - 2 && world.ballPosition.x < world.cubePosition.x + 2) {
            world.ballVelocity.y *= -1;
            Vector3 vec_dif = world.ballPosition - world.cubePosition;
            world.ballVelocity.x += vec_dif.x * .25f;
        }
    }

    ImguiSerializer serializer;
    rose::ecs::serialize(world, serializer);
}

void DrawCubeWiresOutline(Vector3 position, float width, float height, float length, Color colorA, Color colorB)
{
    DrawCube(position, width,height, length, colorA);
    DrawCubeWires(position, width+.01f,height+.01f, length+.01f, colorB);
}

ROSE_EXPORT void draw() {
    update();

    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        {
            DrawCubeWiresOutline(world.cubePosition, 3.0f, 1.0f, 1.0f, YELLOW, MAROON);

            DrawSphere(world.ballPosition, .5f, RED);
            
            DrawCubeWiresOutline({-13.5f, .0f, .0f,}, 1.0f, 100.0f, 1.0f, GRAY, BLACK);
            DrawCubeWiresOutline({13.5f, .0f, .0f,}, 1.0f, 100.0f, 1.0f, GRAY, BLACK);
            DrawCubeWiresOutline({.0f, 20.0f, .0f,}, 100.0f, 1.0f, 1.0f, GRAY, BLACK);

            DrawGrid(10, 1.0f);
        }
        EndMode3D();

        DrawRectangle( 10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines( 10, 10, 320, 133, BLUE);

        DrawText("Free camera default controls:", 20, 20, 10, BLACK);
        DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
        DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
        DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
        DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10, DARKGRAY);
        DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);
    }
    EndDrawing();

}

ROSE_EXPORT void event(const rose::Event & ev) {
}

ROSE_EXPORT void ui() {
}


#include <rose/engine.hpp>