#include "MessagingSystem.h"

// Main function
int main() {
    // Create Colliders
    Collider playerCollider("Player");
    Collider wall1("Wall1");
    Collider wall2("Wall2");

    // Create CollisionManager
    CollisionManager collisionManager;

    // Create a player and register it as a listener
    Player player("Player");
    collisionManager.addListener(&player);

    // Detect collisions between the player and walls
    collisionManager.detectCollision(playerCollider, wall1);
    collisionManager.detectCollision(playerCollider, wall2);

    return 0;
}