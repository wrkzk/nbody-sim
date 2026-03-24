#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <raylib.h>
#include <vector>

// Struct representing an arbitrary body
struct Body {
    glm::vec2 position;
    glm::vec2 velocity;
    float mass;
    float radius;
};

// Time step and gravitational constants
const float dt = 0.00005;
const float G = 500000.0f;

// Window width and height
const int width = 1500;
const int height = 1000;

// Main loop
int main() {
    InitWindow(width, height, "N-Body Physics Simulator");

    // Center of the window
    glm::vec2 center = {width / 2, height / 2};
    
    // Initial velocity vectors     
    glm::vec2 v1 = {0.466203685f, 0.43236573f};
    glm::vec2 v2 = v1;
    glm::vec2 v3 = -2.0f * v1;
    
    // Three bodies with interesting initial conditions respresenting the 3-body problem
    Body b1 = { center + glm::vec2(0.9700436f, -0.24308753f) * 100.0f, v1 * 100.0f, 1.0f, 10.0f };
    Body b2 = { center + glm::vec2(-0.9700436f, 0.24308753f) * 100.0f, v2 * 100.0f, 4.0f, 10.0f };
    Body b3 = { center, v3 * 100.0f, 1.0f, 10.0f };

    // All the bodies are contained in the world vector
    std::vector<Body> world = {b1, b2, b3};

    float accumulator = 0.0f;
    while (!WindowShouldClose())
    {
        accumulator += GetFrameTime();
        while (accumulator >= dt) {
            for (int i = 0; i < world.size(); i++) {

                // Total net force is stored for each body
                glm::vec2 net_force = { 0, 0 };
                
                // Calculate the force for each other body in the world vector
                for (int j = 0; j < world.size(); j++) {
                    if (i == j) continue;

                    // Newton's Law of Universal Gravitation
                    float distance = glm::distance(world[i].position, world[j].position);
                    float force = G * ((world[i].mass * world[j].mass) / (distance * distance));
                    
                    // Find directional unit vector between centers of masses and add total force
                    glm::vec2 dir = glm::normalize(world[j].position - world[i].position);
                    net_force += dir * force;
                }
                
                // Use F = ma to calculate the acceleration vector for the current body
                // Then multiply acceleration by the timestep and increment the bodies velocity
                glm::vec2 accel = net_force / world[i].mass;
                world[i].velocity += accel * dt;
            }
            
            // Now that velocity has been updated for all of the bodies, update their positions
            for (int i = 0; i < world.size(); i++) {
                world[i].position += world[i].velocity * dt;
            }
            
            // Update at a consitent framerate
            accumulator -= dt;
        }
        
        // Call raylib functions that initialize canvas for drawing (port to OpenGL eventually)
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Draw each of the bodies at the correct position with a specified radius
        for (int i = 0; i < world.size(); i++) {
            DrawCircle(world[i].position.x, world[i].position.y, world[i].radius, RED);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
