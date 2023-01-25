#include <raylib.h>
#include <rlgl.h>
#include <rose.h>
#include <rose/event.h>
#include <rose/hash.h>
#include <roseio.h>
#include <imgui.h>
#include <components/components.h>
#include <components/padevents.h>
#include <components/roseoptions.h>
#include <components/workspace.h>

#define IMPL_SERIALIZER
#include <components/components_ser.h>
#include <serializer/imguiserializer.h>
#include <algorithm>

#include "world.h"
#include "world.serializer.h"

#include <rose/world.h>

#define CUTE_C2_IMPLEMENTATION
#include "cute/cute_c2.h"

Camera3D camera = { 0 };
float ballSpeed = .15f;
float ballRadius = .5f;
Texture2D cubeTexture;

static Vector3 operator*(Vector3 lhs, float rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

static Vector3 operator+(Vector3 lhs, Vector3 rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

static Vector3 operator-(Vector3 lhs, Vector3 rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

static c2v operator+(c2v lhs, c2v rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

static c2v operator-(c2v lhs, c2v rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

static Vector3 & operator+=(Vector3 & lhs, Vector3 rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

static c2v toc2v(Vector3 v) {
    c2v v2;
    v2.x = v.x;
    v2.y = v.y;
    return v2;
}

bool equalish(Vector3 lhs, Vector3 rhs) {
    float dx = lhs.x - rhs.x;
    float dy = lhs.y - rhs.y;
    float dz = lhs.z - rhs.z;
    if(dx < 0) dx = -dx;
    if(dy < 0) dy = -dy;
    if(dz < 0) dz = -dz;

    return dx < 0.001f && dy < 0.001f && dz < 0.001f;
}

void processPadEvent(const PadEvent & pad, World & world) ;


bool add_new_stone (World & world) {
    float x = -13;
    float y = 23;
    float xyi = 1;
    int i = 0;

    int rows_max = 15;
    int row = 0;

    Color colors[] = {RED,BLUE,YELLOW, ORANGE,GREEN};

    Stone stone { {x, y, 0} };
    for(;;) {
        if(i < world.stones.size()) {
            if(equalish(world.stones[i].position, stone.position)) {
                i++;
                stone.position.x += xyi;
                if(stone.position.x > 13) {
                    stone.position.x = -13;
                    stone.position.y -= xyi;
                    row++;
                    if(row == rows_max) break;
                    stone.color = colors[row % 5];
                }
                continue;
            }
        }
        break;
    }

    if(row == rows_max) return false;

    world.stones.push_back(stone);

    std::sort(world.stones.begin(), world.stones.end(), [](auto & lhs, auto & rhs) {
        if(lhs.position.y == rhs.position.y) {
            return lhs.position.x < rhs.position.x;
        }
        return lhs.position.y > rhs.position.y;
    });

    return true;
};

void new_game(World & world) {
    world.stones.clear();
    for(int i = 0; i != 15*27; ++i) {
        bool added = add_new_stone(world);
        rose::unused(added);
    }
}

ROSE_EXPORT void postload() {
    World & world = rose::world::get<World>();

    // Define the camera to look into our 3d world
    camera.position = { 0.0f, 10.0f, 40.0f }; // Camera position
    camera.target = { 0.0f, 10.0f, 0.0f };      // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type
    SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

    // Create a small texture in memory
    Image image = GenImageGradientRadial(64, 64, .22f, WHITE, {200,200,200,255});
    
    // Load the texture from image data
    cubeTexture = LoadTextureFromImage(image);

    SetTextureFilter(cubeTexture, TEXTURE_FILTER_POINT);

    // Unload the image data to free memory
    UnloadImage(image);

    rose::io::json::read(world, rose::io::Folder::Working, "game_state.json");
}

ROSE_EXPORT void predestroy() {
    World & world = rose::world::get<World>();
    rose::io::json::write(world, rose::io::Folder::Working, "game_state.json");
}

void collisionCheck(World & world) {
    c2Circle pBall;
    pBall.p = toc2v(world.ballPosition);
    pBall.r = ballRadius;

    for(auto & stone : world.stones) {
        c2v p = toc2v(stone.position);

        c2AABB pStone;
        pStone.max = p + toc2v( stone.size * .5 );
        pStone.min = p - toc2v( stone.size * .5 );

        if(c2CircletoAABB(pBall, pStone)) {
            stone.state = StoneState::Dead;

            float dx = fabsf(p.x - pBall.p.x);
            float dy = fabsf(p.y - pBall.p.y);

            if(dy > dx)
                world.ballVelocity.y *= -1;
            else
                world.ballVelocity.x *= -1;
            break;
        }
    }

    world.stones.erase(std::remove_if(world.stones.begin(), world.stones.end(), [](auto & stone) {
        return stone.state == StoneState::Dead;
    }), world.stones.end());
}

void reset_ball(World & world) {
    world.ballPosition = world.cubePosition + Vector3 {0.0f, 1.0f, 0.0f};
    float vx = rose::nextf(world.random) * 2 - 1;
    world.ballVelocity = Vector3 {vx, 1.0, 0};

    static Color colors[] {
      YELLOW     ,
      GOLD       ,
      ORANGE     ,
      PINK       ,
      RED        ,
      MAROON     ,
      GREEN      ,
      DARKGREEN  ,
      SKYBLUE    ,
      DARKBLUE   ,
      PURPLE     ,
      DARKPURPLE ,
    };

    int i = rose::next_range(world.random, 0, std::size(colors));
    world.ballColor = colors[i];
}

ROSE_EXPORT void update() {
    World & world = rose::world::get<World>();
    if(world.state == WorldState::Paused) return;

    collisionCheck(world);

    world.cubePosition.x += world.currentStick.x;
    world.ballPosition += world.ballVelocity * ballSpeed;

    if(world.cubePosition.x < -12) world.cubePosition.x = -12;
    if(world.cubePosition.x > 12)  world.cubePosition.x =  12;

    if(world.ballPosition.x < -13) world.ballVelocity.x *= -1;
    if(world.ballPosition.x >  13) world.ballVelocity.x *= -1;
    if(world.ballPosition.y >  23) world.ballVelocity.y *= -1;
    
    if(world.ballPosition.y < 0) {
        world.points -= 1;

        reset_ball(world);
    }
    else if(world.ballPosition.y < 1 && world.ballVelocity.y < 0) {
        if(world.ballPosition.x > world.cubePosition.x - 2 && world.ballPosition.x < world.cubePosition.x + 2) {
            world.ballVelocity.y *= -1;
            Vector3 vec_dif = world.ballPosition - world.cubePosition;
            world.ballVelocity.x += vec_dif.x * .25f;
        }
    }
}

void DrawCubeWiresOutline(Vector3 position, float width, float height, float length, Color color) {
    DrawCubeTexture(cubeTexture, position, width, height, length, color);
}

ROSE_EXPORT void draw() {    
    World & world = rose::world::get<World>();
 
    static float new_build = 7;
    ImGui::LabelText("Build Time", "%s %s", __TIME__, new_build > 0 ? "NEW BUILD!" : "");
    new_build -= .1f;

    if(ImGui::Button("New Stone")) {
        bool added = add_new_stone(world);
        rose::unused(added);
    }

    if(ImGui::Button("Clear Stone")) {
        world.stones.clear();
    }

    if(ImGui::Button("Quicksafe")) {
        rose::io::json::write(world, rose::io::Folder::Working, "quicksafe.json");
    }

    if(ImGui::Button("Quickload")) {
        rose::io::json::read(world, rose::io::Folder::Working, "Quicksafe.json");
    }

    if(ImGui::Button("New Game")) {
        world.points = 0;
        world.random = rose::hash_from_clock();
        world.state = WorldState::Running;
        reset_ball(world);
        new_game(world);
    }
    ImGui::DragFloat("Cam Distance", &camera.position.z, .1f, 1, 100);

    ImGui::Separator();

    ImguiSerializer serializer;
    rose::ecs::serialize(world, serializer);

    //BeginDrawing();
    {
        //ClearBackground(DARKGRAY);

        //camera.position = { 0.0f, 10.0f, 35.5f }; // Camera position
        camera.target = { 0.0f, 10.0f, 0.0f };      // Camera looking at point
        camera.up = { 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
        ImGui::DragFloat("Cam fov ", &camera.fovy);
        camera.projection = CAMERA_PERSPECTIVE;                   // Camera mode type
        SetCameraMode(camera, CAMERA_FREE); // Set a free camera mode

        BeginMode3D(camera);
        {
            //Paddle
            world.cubePosition.z = 2;
            DrawCubeWiresOutline(world.cubePosition, 3.0f, 1.0f, 1.0f, GREEN);

            //Ball
            world.ballPosition.z = 2;
            DrawSphere(world.ballPosition, ballRadius, world.ballColor);

            //Stones
            for(auto & stone : world.stones) {
                DrawCubeWiresOutline(stone.position, stone.size.x, stone.size.y, stone.size.z, stone.color);
            }
            
            //Borders
            DrawCubeWiresOutline({-14.0f,   .0f, .0f,}, 1.0f, 100.0f, 1.0f, GRAY);
            DrawCubeWiresOutline({ 14.0f,   .0f, .0f,}, 1.0f, 100.0f, 1.0f, GRAY);
            DrawCubeWiresOutline({   .0f, 24.0f, .0f,}, 29.0f, 1.0f, 1.0f, GRAY);

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
    //EndDrawing();
}

ROSE_EXPORT void event(const rose::Event & ev) {
    if(auto pad = ev.get<PadEvent>()) {
        World & world = rose::world::get<World>();
        
        PadEventButton changed_button = pad->buttons ^ world.previous_pad_event.buttons;

        if((pad->buttons & PadEventButton::OptionRight) && (pad->buttons & changed_button)) {
            switch (world.state) {
                case WorldState::Paused: world.state = WorldState::Running; break;
                case WorldState::Running: world.state = WorldState::Paused; break;
                default: break;
            }
        }

        world.currentStick.x = pad->stick_lx;
        world.currentStick.y = pad->stick_ly;
        world.previous_pad_event = *pad;
    }
}

#include <rose/engine.hpp>