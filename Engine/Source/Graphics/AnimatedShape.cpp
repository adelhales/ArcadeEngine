// Copyright (c) 2025 Adel Hales

#include "Graphics/AnimatedShape.h"

#include <algorithm>

void AnimatedShape::Start(const AnimatedShapeConfig& config)
{
    config_ = config;
    setTexture(config_.texture);

    currentFrame_ = {};
    elapsedTime_ = 0;

    const int cols = *std::ranges::max_element(config_.frameCountPerRow);
    const int rows = (int)config_.frameCountPerRow.size();

    frameSize_ = sf::Vector2i(config_.texture->getSize()).componentWiseDiv({cols, rows});
    setTextureRect({{}, frameSize_});
}

bool AnimatedShape::Update(float deltaTime)
{
    if (IsFinished())
    {
        return false;
    }

    elapsedTime_ += deltaTime;
    if (elapsedTime_ >= config_.frameDuration)
    {
        currentFrame_.x = (currentFrame_.x + 1) % config_.frameCountPerRow[currentFrame_.y];
        setTextureRect({currentFrame_.componentWiseMul(frameSize_), frameSize_});

        elapsedTime_ -= config_.frameDuration;
        return true;
    }

    return false;
}

void AnimatedShape::SetRow(int rowIndex)
{
    if (currentFrame_.y != rowIndex)
    {
        currentFrame_ = {0, rowIndex};
        setTextureRect({currentFrame_.componentWiseMul(frameSize_), frameSize_});

        elapsedTime_ = 0;
    }
}

bool AnimatedShape::IsFinished() const
{
    return !config_.looping && currentFrame_.x == config_.frameCountPerRow[currentFrame_.y] - 1;
}