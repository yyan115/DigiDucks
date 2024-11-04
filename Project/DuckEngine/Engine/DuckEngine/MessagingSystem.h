#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// CollisionListener Interface
class CollisionListener {
public:
    virtual void onCollision(const std::string& colliderName) = 0;
    virtual ~CollisionListener() = default;
};

// Collider Class
class Collider {
public:
    explicit Collider(const std::string& name) : name(name) {}

    std::string getName() const { return name; }

private:
    std::string name;
};

// CollisionManager Class
class CollisionManager {
public:

    void addListener(CollisionListener* listener) {
        listeners.push_back(listener);
    }

    void detectCollision(const Collider& player, const Collider& wall) {
        std::cout << "Collision detected between " << player.getName() << " and " << wall.getName() << "\n";
        for (auto* listener : listeners) {
            listener->onCollision(wall.getName());
        }
    }

private:
    std::vector<CollisionListener*> listeners;
};

class Player : public CollisionListener {
public:
    explicit Player(const std::string& name) : name(name) {}

    void onCollision(const std::string& colliderName) override {
        std::cout << name << " reacts to collision with " << colliderName << "\n";
    }

private:
    std::string name;
};

class Walls : public CollisionListener {
public:
    explicit Wall(const std::string& name) : name(name) {}

    void onCollision(const std::string& colliderName) override {
        std::cout << name << " reacts to collision with " << colliderName << "\n";
    }   

private:
    std::string name;
};
