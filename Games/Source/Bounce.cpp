// Copyright (c) 2025 Adel Hales

#include "Bounce.h"

using namespace Bounce;

Game::Game(EngineContext& context) :
    Scene(context),
    ballSpawnCooldown(BALL_SPAWN_COOLDOWN_DURATION),
    bounceSound(*ctx.resources.FetchSound(BOUNCE_SOUND_FILENAME)),
    music(*ctx.resources.FetchMusic(MUSIC_FILENAME))
{
    InitPaddle();
    InitStats();
    InitBackground();
    InitBounceSound();
    InitMusic();
}

void Game::InitPaddle()
{
    paddle.shape.setFillColor(PADDLE_COLOR);
    paddle.shape.setSize(PADDLE_SIZE);
    paddle.shape.setOrigin(paddle.shape.getGeometricCenter());

    paddle.speed = PADDLE_SPEED;
}

void Game::InitStats()
{
    stats.scoreText.setFillColor(STATS_SCORE_TEXT_COLOR);
    stats.scoreText.setPosition({10, 10});

    stats.highScoreText.setFillColor(STATS_HIGH_SCORE_TEXT_COLOR);
    stats.highScoreText.setPosition({10, 60});

    stats.livesText.setFillColor(STATS_LIVES_TEXT_COLOR);
    stats.livesText.setPosition({10, 110});
}

void Game::InitBackground()
{
    background.setTexture(ctx.resources.FetchTexture(BACKGROUND_TEXTURE_FILENAME));
    background.setSize(gConfig.windowSize);
}

void Game::InitBounceSound()
{
    bounceSound.setVolume(BOUNCE_SOUND_VOLUME);
}

void Game::InitMusic()
{
    music.setVolume(MUSIC_VOLUME);
    music.setPitch(MUSIC_PITCH);
    music.setLooping(true);
}

void Game::Start()
{
    ctx.cursor.SetVisible(false);

    BindInputs();

    balls.clear();

    StartPaddle();
    StartStats();
    StartMusic();

    ballSpawnCooldown.Restart();
}

void Game::BindInputs()
{
    ctx.input.Bind(MoveLeft, Input::Keyboard{sf::Keyboard::Scan::Left});
    ctx.input.Bind(MoveLeft, Input::Axis{sf::Joystick::Axis::X, -0.25f});

    ctx.input.Bind(MoveRight, Input::Keyboard{sf::Keyboard::Scan::Right});
    ctx.input.Bind(MoveRight, Input::Axis{sf::Joystick::Axis::X, 0.25f});
}

void Game::StartPaddle()
{
    paddle.shape.setPosition(gConfig.windowSize.componentWiseMul({0.50f, 0.90f}));
}

void Game::StartStats()
{
    stats.score = 0;
    stats.scoreText.setString("Score: 0");

    stats.highScore = ctx.save.Get<int>(STATS_HIGH_SCORE_KEY);
    stats.highScoreText.setString("High: " + std::to_string(stats.highScore));

    stats.lives = PADDLE_LIVES;
    stats.livesText.setString("Lives: " + std::to_string(stats.lives));
}

void Game::StartMusic()
{
    music.play();
}

void Game::Update()
{
    if (ballSpawnCooldown.IsOver())
    {
        EventBallSpawn();
        ballSpawnCooldown.Restart();
    }

    UpdatePaddle();
    UpdateBalls();

    HandleCollisions();
}

void Game::UpdatePaddle()
{
    sf::Vector2f lastPosition = paddle.shape.getPosition();

    if (ctx.input.Pressed(MoveLeft))
    {
        paddle.shape.move({-paddle.speed * ctx.time.GetDeltaTime(), 0});
    }
    if (ctx.input.Pressed(MoveRight))
    {
        paddle.shape.move({paddle.speed * ctx.time.GetDeltaTime(), 0});
    }

    if (IsOutsideWindowLeft(paddle.shape) || IsOutsideWindowRight(paddle.shape))
    {
        paddle.shape.setPosition(lastPosition);
    }
}

void Game::UpdateBalls()
{
    for (auto& ball : balls)
    {
        UpdateBall(ball);
    }
}

void Game::UpdateBall(Ball& ball)
{
    sf::Vector2f lastPosition = ball.shape.getPosition();

    ball.shape.move(ball.direction * ball.speed * ctx.time.GetDeltaTime());

    if (IsOutsideWindowLeft(ball.shape) || IsOutsideWindowRight(ball.shape))
    {
        ball.direction.x *= -1;
        ball.shape.setPosition(lastPosition);
    }

    if (IsOutsideWindowTop(ball.shape))
    {
        ball.direction.y *= -1;
        ball.shape.setPosition(lastPosition);
    }
}

void Game::EventBallSpawn()
{
    auto& ball = balls.emplace_back();

    ball.shape.setFillColor(BALL_COLOR);
    ball.shape.setRadius(BALL_RADIUS);
    ball.shape.setOrigin(ball.shape.getGeometricCenter());
    ball.shape.setPosition(gConfig.windowSize.componentWiseMul({0.50f, 0.25f}));

    ball.direction = {1, ctx.random.Angle(sf::Angle::Zero, sf::degrees(360))};
    ball.speed = BALL_SPEED;
}

void Game::EventBallsMissed(int ballsMissed)
{
    stats.lives -= ballsMissed;
    stats.livesText.setString("Lives: " + std::to_string(stats.lives));

    if (stats.lives <= 0)
    {
        ctx.save.Set(STATS_HIGH_SCORE_KEY, std::max(stats.score, stats.highScore));

        LOG_INFO("You Lose! Score: {}", stats.score);
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::HandleCollisions()
{
    HandleCollisionsPaddleBalls();
    HandleCollisionsBallsMap();
}

void Game::HandleCollisionsPaddleBalls()
{
    for (auto& ball : balls)
    {
        if (ball.direction.y > 0 && Intersects(ball.shape, paddle.shape))
        {
            ResolveCollisionPaddleBall(ball);
        }
    }
}

void Game::ResolveCollisionPaddleBall(Ball& ball)
{
    ball.direction.y *= -1;
    bounceSound.play();

    stats.score++;
    stats.scoreText.setString("Score: " + std::to_string(stats.score));
}

void Game::HandleCollisionsBallsMap()
{
    int ballsMissed = (int)std::erase_if(balls, [](const Ball& ball) {
        return IsOutsideWindowBottom(ball.shape);
    });

    if (ballsMissed > 0)
    {
        EventBallsMissed(ballsMissed);
    }
}

void Game::Render() const
{
    ctx.renderer.Draw(background);

    ctx.renderer.Draw(paddle.shape);

    for (const auto& ball : balls)
    {
        ctx.renderer.Draw(ball.shape);
    }

    ctx.renderer.Draw(stats.scoreText);
    ctx.renderer.Draw(stats.highScoreText);
    ctx.renderer.Draw(stats.livesText);
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        ballSpawnCooldown.Stop();
        music.pause();
        bounceSound.stop();
    }
    else
    {
        ballSpawnCooldown.Start();
        music.play();
    }
}

void Game::OnCleanup()
{
    music.stop();
    bounceSound.stop();
}