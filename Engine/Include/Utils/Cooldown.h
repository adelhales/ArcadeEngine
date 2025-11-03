// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/System/Clock.hpp>

class Cooldown
{
private:
    sf::Clock timer_;
    float duration_;

public:
    Cooldown(float duration = 0.f);

    void Start();
    void Stop();

    void Restart();
    void Reset();

    void SetDuration(float duration);
    float GetDuration() const;

    float GetElapsedTime() const;

    bool IsRunning() const;
    bool IsOver() const;
};