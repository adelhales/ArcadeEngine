// Copyright (c) 2025 Adel Hales

#include "Clicker.h"

using namespace Clicker;

Game::Game(EngineContext& context) :
    Scene(context)
{
    InitTarget();
    InitStats();
}

void Game::InitTarget()
{
    target.shape.setTexture(ctx.resources.FetchTexture(TARGET_TEXTURE_FILENAME));
    target.shape.setRadius(TARGET_RADIUS);
    target.shape.setOrigin(target.shape.getGeometricCenter());

    target.teleportCooldown.SetDuration(TARGET_TELEPORT_COOLDOWN_DURATION);
}

void Game::InitStats()
{
    stats.scoreText.setFillColor(STATS_SCORE_TEXT_COLOR);
    stats.scoreText.setCharacterSize(STATS_TEXT_SIZE);
    stats.scoreText.setPosition({5, 5});

    stats.finalCooldown.SetDuration(STATS_FINAL_COOLDOWN_DURATION);

    stats.finalCooldownText.setFillColor(STATS_FINAL_COOLDOWN_TEXT_COLOR);
    stats.finalCooldownText.setCharacterSize(STATS_TEXT_SIZE);
    stats.finalCooldownText.setPosition({5, 50});

    stats.bestTimeText.setFillColor(STATS_BEST_TIME_TEXT_COLOR);
    stats.bestTimeText.setCharacterSize(STATS_TEXT_SIZE);
    stats.bestTimeText.setPosition({5, 95});
}

void Game::Start()
{
    ctx.cursor.SetSpeed(CURSOR_SPEED);

    BindInputs();

    StartTarget();
    StartStats();
}

void Game::BindInputs()
{
    ctx.input.Bind(Click, Input::Mouse{sf::Mouse::Button::Left});
    ctx.input.Bind(Click, Input::Gamepad{GamepadButton::South});
}

void Game::StartTarget()
{
    target.shape.setFillColor(TARGET_COLOR);
    target.shape.setPosition(gConfig.windowSize / 2.f);
}

void Game::StartStats()
{
    stats.score = 0;
    stats.scoreText.setString("Score: 0/" + std::to_string(STATS_SCORE_GOAL));

    stats.finalCooldown.Reset();

    float totalTime = stats.finalCooldown.GetDuration();
    stats.finalCooldownText.setString("Time Left (s): " + std::to_string((int)totalTime));

    stats.bestTime = ctx.save.Get<float>(STATS_BEST_TIME_KEY, totalTime);
    stats.bestTimeText.setString(std::format("Best Time: {:.2f}", stats.bestTime));
}

void Game::Update()
{
    if (ctx.input.Pressed(Click) && IsTargetHovered())
    {
        EventTargetClicked();
    }

    if (stats.finalCooldown.IsRunning())
    {
        UpdateTarget();
        UpdateStats();
    }
}

void Game::UpdateTarget()
{
    if (target.teleportCooldown.IsOver())
    {
        EventTargetTeleport();
        target.teleportCooldown.Restart();
    }

    UpdateTargetColor();
}

void Game::UpdateTargetColor()
{
    float progress = target.teleportCooldown.GetElapsedTime() / target.teleportCooldown.GetDuration();

    sf::Color color = target.shape.getFillColor();
    color.a = std::uint8_t(255 * (1 - std::min(progress, 1.f)));

    target.shape.setFillColor(color);
}

void Game::UpdateStats()
{
    float timeLeft = stats.finalCooldown.GetDuration() - stats.finalCooldown.GetElapsedTime();
    stats.finalCooldownText.setString(std::format("Time Left (s): {:.1f}", timeLeft));

    if (stats.finalCooldown.IsOver())
    {
        LOG_INFO("You Lose! Score: {}", stats.score);
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::EventTargetClicked()
{
    EventTargetTeleport();
    target.teleportCooldown.Restart();

    if (stats.score == 0)
    {
        stats.finalCooldown.Start();
    }

    EventStatsScoreIncrease();
}

void Game::EventTargetTeleport()
{
    sf::Vector2f halfSize = target.shape.getGlobalBounds().size / 2.f;
    target.shape.setPosition(ctx.random.Position(halfSize, gConfig.windowSize - halfSize));
}

void Game::EventStatsScoreIncrease()
{
    stats.score++;
    stats.scoreText.setString(std::format("Score: {}/{}", stats.score, STATS_SCORE_GOAL));

    if (stats.score == STATS_SCORE_GOAL)
    {
        float elapsedTime = stats.finalCooldown.GetElapsedTime();
        ctx.save.Set(STATS_BEST_TIME_KEY, std::min(elapsedTime, stats.bestTime));

        LOG_INFO("You Win! Time: {}", elapsedTime);
        ctx.scenes.RestartCurrentScene();
    }
}

bool Game::IsTargetHovered() const
{
    return Contains(target.shape, ctx.cursor.GetPosition());
}

void Game::Render() const
{
    ctx.renderer.Draw(target.shape);

    ctx.renderer.Draw(stats.scoreText);
    ctx.renderer.Draw(stats.finalCooldownText);
    ctx.renderer.Draw(stats.bestTimeText);
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        target.teleportCooldown.Stop();
        stats.finalCooldown.Stop();
    }
    else
    {
        target.teleportCooldown.Start();
        stats.finalCooldown.Start();
    }
}