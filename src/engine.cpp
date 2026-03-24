#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <raylib.h>
#include <raymath.h>
#include <vector>

// Struct representing an arbitrary body
struct Body {
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    float radius;
};

// Time step and gravitational constants
const float dt = 0.00005;
const float G = 5000000.0f;
const float softening = 1.0f;

// Window width and height
const int width = 1500;
const int height = 1000;

// Main loop
int main() {
    InitWindow(width, height, "N-Body Physics Simulator");

    // Three bodies with interesting initial conditions respresenting the 3-body problem
    Body b0 = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 20.0f, 2.0f };
    Body b1 = { glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -3000.0f), 10.0f, 0.8f };
    Body b2 = { glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 3000.0f), 10.0f, 0.8f };
    Body b3 = { glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(3000.0f, 0.0f, 0.0f), 10.0f, 0.8f };
    Body b4 = { glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(-3000.0f, 0.0f, 0.0f), 10.0f, 0.8f };
    Body b5 = { glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(3000.0f, 0.0f, 0.0f), 10.0f, 0.8f };


    // All the bodies are contained in the world vector
    std::vector<Body> world = {b0, b1, b2, b3, b4};

    // Set up raylib camera
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 20.0f, 0.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    Vector2 prevMousePos = GetMousePosition();
    camera.projection = CAMERA_PERSPECTIVE;

    float rotAngle = 45;
    float tiltAngle = 45;
    float camDist = 70;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        // ------- Raylib camera panning and zoom -------
        
        // Calculate new camera zoom amount
        float wheelDelta = GetMouseWheelMove();
        camDist = camDist + wheelDelta;
        if (camDist <= 0) {
            camDist = 0.01f;
        }

        // Panning logic
        Vector3 camPos = {0, 0, camDist};
        Vector2 curMousePos = GetMousePosition();
        Vector2 mouseDelta = curMousePos - prevMousePos;
        prevMousePos = curMousePos;

        
        rotAngle -= mouseDelta.x * 0.25f;
        tiltAngle += mouseDelta.y * 0.25f;

        if (tiltAngle > 89)
            tiltAngle = 89;

        if (tiltAngle < 1)
            tiltAngle = 1;

        Matrix tiltMatrix = MatrixRotateX(-tiltAngle * DEG2RAD);
        Matrix rotMatrix = MatrixRotateY(rotAngle * DEG2RAD);
        Matrix rot = MatrixMultiply(tiltMatrix, rotMatrix);

        camera.position = Vector3Transform(camPos, rot);


        // ----------------------------------------------


        // --- Physics Logic ---

        for (int i = 0; i < world.size(); i++) {

                // Total net force is stored for each body
            glm::vec3 net_force = { 0, 0, 0 };
                
                // Calculate the force for each other body in the world vector
            for (int j = 0; j < world.size(); j++) {
                if (i == j) continue;

                    // Newton's Law of Universal Gravitation
                float distance = glm::distance(world[i].position, world[j].position);
                float force = G * ((world[i].mass * world[j].mass) /
                        (distance * distance) + (softening * softening));
                    
                    // Find directional unit vector between centers of masses and add total force
                glm::vec3 dir = glm::normalize(world[j].position - world[i].position);
                net_force += dir * force;
            }
                
            // Use F = ma to calculate the acceleration vector for the current body
            // Then multiply acceleration by the timestep and increment the bodies velocity
            glm::vec3 accel = net_force / world[i].mass;
            world[i].velocity += accel * dt;
        }
        
        // Now that velocity has been updated for all of the bodies, update their positions
        for (int i = 0; i < world.size(); i++) {
            world[i].position += world[i].velocity * dt;
        }
            

        // ---- Drawing Logic ----
        
        // Call raylib functions that initialize canvas for drawing (port to OpenGL eventually)
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        DrawGrid(50, 1);

        // Draw each of the bodies at the correct position with a specified radius
        for (int i = 0; i < world.size(); i++) {
            Vector3 center = { world[i].position.x, world[i].position.y, world[i].position.z };
            DrawSphere(center, world[i].radius, RED);
        }

        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
