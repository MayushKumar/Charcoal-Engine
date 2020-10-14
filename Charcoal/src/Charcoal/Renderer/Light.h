#pragma once

#include <glm/glm.hpp>

namespace Charcoal
{
    struct PointLight
    {
        PointLight() {}
        PointLight(const glm::vec3& pos, const glm::vec3& colour, float luminousPower) : Position(pos), Colour(colour), LuminousPower(luminousPower)
        {}

        glm::vec3 Position;
		glm::vec3 Colour;
        float LuminousPower; // Lumens
    };
}
