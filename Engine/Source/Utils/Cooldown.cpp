// Copyright (c) 2025 Adel Hales

#include "Utils/Cooldown.h"

#include <SFML/System/Time.hpp>

Cooldown::Cooldown(float duration) :
    duration_(duration)
{
}

void Cooldown::Start()
{
    timer_.start();
}

void Cooldown::Stop()
{
    timer_.stop();
}

void Cooldown::Restart()
{
    timer_.restart();
}

void Cooldown::Reset()
{
    timer_.reset();
}

void Cooldown::SetDuration(float duration)
{
    duration_ = duration;
}

float Cooldown::GetDuration() const
{
    return duration_;
}

float Cooldown::GetElapsedTime() const
{
    return timer_.getElapsedTime().asSeconds();
}

bool Cooldown::IsRunning() const
{
    return timer_.isRunning();
}

bool Cooldown::IsOver() const
{
    return GetElapsedTime() > duration_;
}