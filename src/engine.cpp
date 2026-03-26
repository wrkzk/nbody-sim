#include <cmath>
#include <cstdlib>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <raylib.h>
#include <raymath.h>
#include <vector>

// Struct representing an arbitrary body
typedef struct Body {
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    float radius;
} Body;

// Octree node
typedef struct Node {
    
    // Stores pointers to all sub-nodes of the octree
    Node *ne, *nw, *sw, *se;
    Node *ne2, *nw2, *sw2, *se2;

    // Optionally stores pointer to body
    Body *body;

    // Store the width of the region of space the node represents
    float width;

} Node;

// Time step and gravitational constants, softening factor
const float dt = 0.016;
const float G = 1.0f; // Increasing G increases simulation speed
const float softening = 1.0f;

// Window width and height
const int width = 1500;
const int height = 1000;

/* Iterates through the world vector and constructs the Octree that represents all bodies in 3D space
 *      params: world (vector of all bodies)
 *      returns: pointer to root node of octree
 */
Node *constructOctree(std::vector<Body> world, glm::vec3 midpoint, float width) {
    
    // Base case 1: world is empty
    if (world.size() == 0) {
        Node *emptyNode = (Node *) malloc(sizeof(Node));
        emptyNode->width = width;
        return emptyNode;
    }
    
    // Base case 2: world contains 1 body
    else if (world.size() == 1) {
        Node *node = (Node *) malloc(sizeof(Node));
        node->body = &world[1];
        node->width = width;
        return node;
    }
    
    // Else: split into 8 vectors depending on position and call recursively on each
    else {
        
        // Separate points out into quadrants
        std::vector<Body> ne, nw, sw, se;
        std::vector<Body> ne2, nw2, sw2, se2;

        while (world.size() > 0) {
            Body curr = world.back();

            // Case for if point is in rear 'layer'
                // Determine which of the 4 quadrants it goes into
                

            // Case for if point is in top 'layer'
                // Determine which of the 4 quadrants it goes into

        }

        Node *node = (Node *) malloc(sizeof(Node));

        node->ne = constructOctree(ne, width / 2);
        node->nw = constructOctree(nw, width / 2);
        node->sw = constructOctree(sw, width / 2);
        node->se = constructOctree(se, width / 2);

        node->ne2 = constructOctree(ne, width / 2);
        node->nw2 = constructOctree(nw, width / 2);
        node->sw2 = constructOctree(sw, width / 2);
        node->se2 = constructOctree(se, width / 2);

        return node;
    }
}

// Main loop
int main() {
    InitWindow(width, height, "N-Body Physics Simulator");

    // Three bodies with interesting initial conditions respresenting the 3-body problem
    Body b0 = { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 5000.0f, 2.0f };
    Body b1 = { glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -15.0f), 10.0f, 0.8f };
    Body b2 = { glm::vec3(-10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 15.0f), 10.0f, 0.8f };
    Body b3 = { glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(15.0f, 0.0f, 0.0f), 10.0f, 0.8f };
    Body b4 = { glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(-15.0f, 0.0f, 0.0f), 10.0f, 0.8f };

    Body b6 = { glm::vec3(7.071f, 0.0f, 7.071f), glm::vec3(-14.14f, 0.0f, 14.14f), 10.0f, 0.8f };
    Body b7 = { glm::vec3(7.071f, 0.0f, -7.071f), glm::vec3(14.14f, 0.0f, 14.14f), 10.0f, 0.8f };
    Body b8 = { glm::vec3(-7.071f, 0.0f, 7.071f), glm::vec3(-14.14f, 0.0f, -14.14f), 10.0f, 0.8f };
    Body b9 = { glm::vec3(-7.071f, 0.0f, -7.071f), glm::vec3(14.14f, 0.0f, -14.14f), 10.0f, 0.8f };


    // All the bodies are contained in the world vector
    std::vector<Body> world = {b0, b1, b2, b3, b4, b6, b7, b8, b9};

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
        
        // --- Physics Logic (Barnes-Hut)
        
        
        glm::vec3 origin = {0.0f, 0.0f, 0.0f};

        // Determine the size of the root node cube
        float maxDist, minDist = 0.0f;
        glm::vec3 maxVec, minVec;        

        for (int i = 0; i < world.size(); i++) {
        }
        
        // Create the octree
        Node *octree = constructOctree(world, origin, 1000.0f);
        
        // Calculate forces on the current body

        
        // --- Physics Logic (Naive )---

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

        DrawGrid(100, 5);

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
