// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace Bounce
{
    const sf::Vector2f PADDLE_SIZE(120, 20);
    const int PADDLE_LIVES = 10;
    const float PADDLE_SPEED = 750;

    const float BALL_RADIUS = 10;
    const float BALL_SPEED = 250;
    const float BALL_SPAWN_COOLDOWN_DURATION = 2;

    const float MUSIC_VOLUME = 5;
    const float MUSIC_PITCH = 1;

    const float BOUNCE_SOUND_VOLUME = 10;

    const sf::Color PADDLE_COLOR(sf::Color::Green);
    const sf::Color BALL_COLOR(sf::Color::Cyan);
    const sf::Color STATS_SCORE_TEXT_COLOR(sf::Color::Cyan);
    const sf::Color STATS_HIGH_SCORE_TEXT_COLOR(sf::Color::Yellow);
    const sf::Color STATS_LIVES_TEXT_COLOR(sf::Color::Red);

    const std::string MUSIC_FILENAME = "Music.mp3";
    const std::string BOUNCE_SOUND_FILENAME = "Bonus.mp3";
    const std::string BACKGROUND_TEXTURE_FILENAME = "PurpleBackground.png";

    const std::string_view STATS_HIGH_SCORE_KEY = "Bounce:High Score";
}