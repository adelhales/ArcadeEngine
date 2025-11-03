// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <vector>

struct AnimatedShapeConfig
{
    const sf::Texture* texture;
    std::vector<int> frameCountPerRow;
    float frameDuration;
    bool looping = true;
};

class AnimatedShape : public sf::RectangleShape
{
private:
    AnimatedShapeConfig config_;
    sf::Vector2i currentFrame_;
    sf::Vector2i frameSize_;
    float elapsedTime_;

public:
    void Start(const AnimatedShapeConfig& config);
    bool Update(float deltaTime);

    void SetRow(int rowIndex);
    bool IsFinished() const;
};