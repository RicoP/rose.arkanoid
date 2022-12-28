#include <raylib.h>
#include <rlgl.h>
#include <rose.h>
#include <rose/event.h>
#include <roseio.h>
#include <imgui.h>
#include <components/padevents.h>
#include <components/components.h>
#include <components/components_ser.h>

static PadEventButton buttons = PadEventButton::NONE;

Camera3D camera = { 0 };

Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
Vector3 ballPosition = { 0.0f, 1.0f, 0.0f };
Vector3 ballVelocity = { 1.0f, 1.0f, 0.0f };

float ballSpeed = .3f;
int points = 0;

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
}

ROSE_EXPORT void predestroy() {
}

void update() {
    cubePosition.x += rose::input::stick().x;
    ballPosition += ballVelocity * ballSpeed;

    #define VEC_FMT(v) "(%g, %g, %g)", v.x, v.y, v.z
    ImGui::LabelText("Cube Pos: ", VEC_FMT(cubePosition));
    ImGui::LabelText("Ball Pos: ", VEC_FMT(ballPosition));
    #undef VEC_FMT
    ImGui::DragFloat("Ball Speed", &ballSpeed, .05f, .01f, 1);

    if(cubePosition.x < -11.5) cubePosition.x = -11.5;
    if(cubePosition.x > 11.5) cubePosition.x = 11.5;

    if(ballPosition.x < -13) ballVelocity.x *= -1;
    if(ballPosition.x > 13) ballVelocity.x *= -1;
    if(ballPosition.y > 20) ballVelocity.y *= -1;
    
    if(ballPosition.y < 0) {
        points -= 1;

        ballPosition = cubePosition + Vector3 {0.0f, 1.0f, 0.0f};
        ballVelocity = Vector3 {1,1.0};
    }
    else if(ballPosition.y < 1 && ballVelocity.y < 0) {
        if(ballPosition.x > cubePosition.x - 1.5f && ballPosition.x < cubePosition.x + 1.5f) {
            ballVelocity.y *= -1;
        }
    }
    //ballVelocity.y *= -1;


}

void DrawCubeWiresOutline(Vector3 position, float width, float height, float length, Color colorA, Color colorB)
{
    DrawCube(position, width,height, length, colorA);
    DrawCubeWires(position, width+.01f,height+.01f, length+.01f, colorB);
}

ROSE_EXPORT void draw() {
    //ImGui::Button("Hello");
    //if(!!(buttons & PadEventButton::North)) ImGui::Button("Y");
    //if(!!(buttons & PadEventButton::West)) ImGui::Button("X");
    //if(!!(buttons & PadEventButton::South)) ImGui::Button("A");
    //if(!!(buttons & PadEventButton::East)) ImGui::Button("B");

    update();


    BeginDrawing();
    {
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        {
            DrawCubeWiresOutline(cubePosition, 3.0f, 1.0f, 1.0f, YELLOW, MAROON);

            DrawSphere(ballPosition, .5f, RED);
            
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
    buttons = PadEventButton::NONE;
    if(auto pad = ev.get<PadEvent>()) {
        buttons |= pad->buttons;
    }
}

ROSE_EXPORT void ui() {
}