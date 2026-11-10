#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab/lab04/transform3D.h"

class Physics
{
public:
    Physics();
    ~Physics();

    void ApplyGravity(float deltaT);
    void ApplyFriction(float friction);

    bool CheckCollision();
    bool CheckBBCollision();

    bool hitsBoundingBox;


private:



};

