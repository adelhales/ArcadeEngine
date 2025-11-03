// Copyright (c) 2025 Adel Hales

#include "Pong.h"

using namespace Pong;

Game::Game(EngineContext& context) :
    Scene(context)
{
    InitPlayerLeft(player1);
    InitPlayerRight(player2);
    InitBall();
}

void Game::InitPlayerLeft(Player& player)
{
    InitPlayerBase(player);

    player.joystickId = 0;
    player.shape.setFillColor(PLAYER_1_COLOR);
    player.stats.scoreText.setFillColor(PLAYER_1_SCORE_COLOR);
}

void Game::InitPlayerRight(Player& player)
{
    InitPlayerBase(player);

    player.joystickId = 1;
    player.shape.setFillColor(PLAYER_2_COLOR);
    player.stats.scoreText.setFillColor(PLAYER_2_SCORE_COLOR);
}

void Game::InitPlayerBase(Player& player)
{
    player.shape.setSize(PLAYER_SIZE);
    player.shape.setOrigin(player.shape.getGeometricCenter());

    player.speed = PLAYER_SPEED;
}

void Game::InitBall()
{
    ball.shape.setFillColor(BALL_COLOR);
    ball.shape.setRadius(BALL_RADIUS);
    ball.shape.setOrigin(ball.shape.getGeometricCenter());
}

void Game::BindInputs()
{
    ctx.input.Bind(Player1MoveUp,   Input::Keyboard{sf::Keyboard::Scan::W});
    ctx.input.Bind(Player1MoveDown, Input::Keyboard{sf::Keyboard::Scan::S});

    ctx.input.Bind(Player2MoveUp,   Input::Keyboard{sf::Keyboard::Scan::Up});
    ctx.input.Bind(Player2MoveDown, Input::Keyboard{sf::Keyboard::Scan::Down});
}

void Game::Start()
{
    ctx.cursor.SetVisible(false);
    
    BindInputs();

    StartPlayerLeft(player1);
    StartPlayerRight(player2);

    EventBallReset();
}

void Game::StartPlayerLeft(Player& player)
{
    StartPlayerBase(player);

    player.shape.setPosition({player.shape.getSize().x / 2, gConfig.windowSize.y / 2});

    player.stats.scoreText.setPosition({75, 25});
}

void Game::StartPlayerRight(Player& player)
{
    StartPlayerBase(player);

    player.shape.setPosition({gConfig.windowSize.x - player.shape.getSize().x / 2,
                              gConfig.windowSize.y / 2});

    float offsetX = player.stats.scoreText.getGlobalBounds().size.x;
    player.stats.scoreText.setPosition({gConfig.windowSize.x - offsetX - 75, 25});
}

void Game::StartPlayerBase(Player& player)
{
    player.stats.score = 0;
    player.stats.scoreText.setString("Score: 0");
}

void Game::Update()
{
    UpdatePlayer(player1, Player1MoveUp, Player1MoveDown);

    if (TWO_PLAYERS)
    {
        UpdatePlayer(player2, Player2MoveUp, Player2MoveDown);
    }
    else
    {
        UpdatePlayerAI(player2);
    }

    UpdateBall();

    HandleCollisions();
}

void Game::UpdatePlayer(Player& player, Action up, Action down)
{
    sf::Vector2f lastPosition = player.shape.getPosition();

    float joystickDirectionY = sf::Joystick::getAxisPosition(player.joystickId, sf::Joystick::Axis::Y) / 100;

    if (std::abs(joystickDirectionY) > gConfig.joystickDeadzone)
    {
        player.shape.move({0, joystickDirectionY * player.speed * ctx.time.GetDeltaTime()});
    }
    else
    {
        if (ctx.input.Pressed(up))
        {
            player.shape.move({0, -player.speed * ctx.time.GetDeltaTime()});
        }
        if (ctx.input.Pressed(down))
        {
            player.shape.move({0, player.speed * ctx.time.GetDeltaTime()});
        }
    }

    if (IsOutsideWindowTop(player.shape) || IsOutsideWindowBottom(player.shape))
    {
        player.shape.setPosition(lastPosition);
    }
}

void Game::UpdatePlayerAI(Player& player)
{
    sf::Vector2f lastPosition = player.shape.getPosition();

    float distanceY = ball.shape.getPosition().y - player.shape.getPosition().y;
    float desiredVelocityY = distanceY * PLAYER_AI_SPEED_FACTOR;
    float velocityY = std::clamp(desiredVelocityY, -player.speed, player.speed);

    player.shape.move({0, velocityY * ctx.time.GetDeltaTime()});

    if (IsOutsideWindowTop(player.shape) || IsOutsideWindowBottom(player.shape))
    {
        player.shape.setPosition(lastPosition);
    }
}

void Game::UpdateBall()
{
    ball.shape.move(ball.direction * ball.speed * ctx.time.GetDeltaTime());
}

void Game::EventPlayerWin(Player& player)
{
    player.stats.score++;
    player.stats.scoreText.setString("Score: " + std::to_string(player.stats.score));

    EventBallReset();
}

void Game::EventBallReset()
{
    ball.shape.setPosition(gConfig.windowSize / 2.f);

    ball.direction = {1, ctx.random.Angle(-BALL_ANGLE_MAX, BALL_ANGLE_MAX)};
    ball.direction.x *= ctx.random.Bool() ? 1 : -1;

    ball.speed = BALL_SPEED;
}

void Game::HandleCollisions()
{
    HandleCollisionsPlayersBall();
    HandleCollisionsBallMap();

    if (player1.stats.score == STATS_SCORE_GOAL)
    {
        LOG_INFO("You Win!");
        ctx.scenes.RestartCurrentScene();
    }
    else if (player2.stats.score == STATS_SCORE_GOAL)
    {
        LOG_INFO("You Lose!");
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::HandleCollisionsPlayersBall()
{
    if (ball.direction.x < 0 && Intersects(player1.shape, ball.shape))
    {
        ResolveCollisionPlayerBall(player1);
    }
    else if (ball.direction.x > 0 && Intersects(player2.shape, ball.shape))
    {
        ResolveCollisionPlayerBall(player2);
    }
}

void Game::ResolveCollisionPlayerBall(const Player& player)
{
    float ballRelativePositionY = ball.shape.getPosition().y - player.shape.getPosition().y;
    float ballHitPositionY = ballRelativePositionY / (player.shape.getSize().y / 2);

    sf::Vector2f direction(-ball.direction.x, std::sin(BALL_ANGLE_MAX.asRadians()) * ballHitPositionY);
    ball.direction = direction.normalized();

    ball.speed = std::min(ball.speed * BALL_SPEED_BONUS_FACTOR, BALL_SPEED_MAX);
}

void Game::HandleCollisionsBallMap()
{
    if ((ball.direction.y < 0 && IsOutsideWindowTop(ball.shape)) ||
        (ball.direction.y > 0 && IsOutsideWindowBottom(ball.shape)))
    {
        ball.direction.y *= -1;
    }

    if (IsOutsideWindowLeft(ball.shape))
    {
        EventPlayerWin(player2);
    }
    else if (IsOutsideWindowRight(ball.shape))
    {
        EventPlayerWin(player1);
    }
}

void Game::Render() const
{
    RenderMap();

    ctx.renderer.Draw(player1.shape);
    ctx.renderer.Draw(player2.shape);

    ctx.renderer.Draw(ball.shape);

    ctx.renderer.Draw(player1.stats.scoreText);
    ctx.renderer.Draw(player2.stats.scoreText);
}

void Game::RenderMap() const
{
    sf::RectangleShape lineSegment(MAP_LINE_SEGMENT_SIZE);
    lineSegment.setFillColor(MAP_LINE_SEGMENT_COLOR);
    lineSegment.setOrigin(lineSegment.getGeometricCenter());

    for (float y = MAP_LINE_SEGMENT_SIZE.y; y < gConfig.windowSize.y; y += MAP_LINE_SEGMENT_SIZE.y * 2)
    {
        lineSegment.setPosition({gConfig.windowSize.x / 2, y});
        ctx.renderer.Draw(lineSegment);
    }
}