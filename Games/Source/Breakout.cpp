// Copyright (c) 2025 Adel Hales

#include "Breakout.h"

#include <magic_enum/magic_enum.hpp>

using namespace Breakout;

Game::Game(EngineContext& context) :
    Scene(context)
{
    InitPlayer();
}

void Game::InitPlayer()
{
    player.shape.setTexture(ctx.resources.FetchTexture(PLAYER_TEXTURE_FILENAME));
    player.shape.setFillColor(PLAYER_COLOR);
    player.shape.setSize(gConfig.windowSize.componentWiseMul({0.12f, 0.03f}));
    player.shape.setOrigin(player.shape.getGeometricCenter());
}

void Game::Start()
{
    ctx.cursor.SetVisible(false);

    balls.clear();
    bonuses.clear();

    StartPlayer();
    StartBricks();

    EventBallSpawn();
}

void Game::StartPlayer()
{
    player.shape.setPosition(gConfig.windowSize.componentWiseMul({0.50f, 0.90f}));
    player.shape.setScale({1, 1});

    player.magnetic = false;

    player.stats.score = 0;
    player.stats.scoreText.setString("Score: 0");
    player.stats.scoreText.setPosition({5, 750});

    player.stats.lives = PLAYER_LIVES;
    player.stats.livesText.setString("Lives: " + std::to_string(player.stats.lives));
    player.stats.livesText.setPosition({680, 750});
}

void Game::StartBricks()
{
    bricks.clear();
    bricks.reserve(BRICK_MAP_SIZE.y * BRICK_MAP_SIZE.x);

    for (int i = 0; i < BRICK_MAP_SIZE.y; i++)
    {
        for (int j = 0; j < BRICK_MAP_SIZE.x; j++)
        {
            auto& brick = bricks.emplace_back();

            StartBrick(brick, {j, i});
        }
    }
}

void Game::StartBrick(Brick& brick, sf::Vector2i tile)
{
    brick.lives = ctx.random.Int(1, BRICK_LIVES_MAX);

    brick.shape.setFillColor(BRICK_COLORS[brick.lives - 1]);
    brick.shape.setOutlineColor(sf::Color::Black);
    brick.shape.setOutlineThickness(-1);
    brick.shape.setSize({gConfig.windowSize.x / BRICK_MAP_SIZE.x,
                         gConfig.windowSize.y / (BRICK_MAP_SIZE.y * 4.5f)});
    brick.shape.setPosition(sf::Vector2f(tile).componentWiseMul(brick.shape.getSize()));
}

void Game::Update()
{
    UpdatePlayer();
    UpdateBalls();
    UpdateBonuses();

    HandleCollisions();
}

void Game::UpdatePlayer()
{
    float positionY = player.shape.getPosition().y;
    player.shape.setPosition({ctx.cursor.GetPosition().x, positionY});

    sf::FloatRect bounds = player.shape.getGlobalBounds();
    if (IsOutsideWindowLeft(player.shape))
    {
        player.shape.setPosition({bounds.size.x / 2, positionY});
    }
    else if (IsOutsideWindowRight(player.shape))
    {
        player.shape.setPosition({gConfig.windowSize.x - bounds.size.x / 2, positionY});
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
    if (player.magnetic && ball.direction.y > 0 && IsBallNearPlayer(ball))
    {
        sf::Vector2f attraction = player.shape.getPosition() - ball.shape.getPosition();
        ball.direction = (ball.direction + attraction.normalized()).normalized();
    }

    ball.shape.move(ball.direction * ball.speed * ctx.time.GetDeltaTime());
}

void Game::UpdateBonuses()
{
    for (const auto& bonus : bonuses)
    {
        if (bonus.endCooldown.IsOver())
        {
            EventBonusDisable(bonus);
        }
    }

    UpdateBonusesCleanup();
}

void Game::UpdateBonusesCleanup()
{
    std::size_t erased = std::erase_if(bonuses, [](const Bonus& bonus) {
        return bonus.endCooldown.IsOver();
    });

    if (erased > 0)
    {
        for (std::size_t i = 0; i < bonuses.size(); i++)
        {
            bonuses[i].text.setPosition({gConfig.windowSize.x * 0.75f,
                                         gConfig.windowSize.y * 0.375f + BONUS_TEXT_OFFSET_Y * i});
        }
    }
}

void Game::EventBallSpawn()
{
    auto& ball = balls.emplace_back();

    ball.shape.setFillColor(BALL_COLOR);
    ball.shape.setRadius(BALL_RADIUS);
    ball.shape.setOrigin(ball.shape.getGeometricCenter());

    ball.alive = true;

    EventBallReset(ball);
}

void Game::EventBallReset(Ball& ball)
{
    sf::Vector2f direction(1, ctx.random.Angle(-BALL_ANGLE_MAX, BALL_ANGLE_MAX));
    ball.direction = {direction.y, -direction.x};
    ball.speed = BALL_SPEED;

    sf::Vector2f offset(0, player.shape.getGlobalBounds().size.y);
    ball.shape.setPosition(player.shape.getPosition() - offset);

    ball.fire = false;
    ball.invincible = false;
}

void Game::EventBonusEnable()
{
    std::vector<BonusType> types = GetBonusTypesAllowed();

    if (types.empty())
    {
        return;
    }

    Bonus bonus;
    bonus.type = types[ctx.random.Int(0, (int)types.size() - 1)];
    bonus.endCooldown.SetDuration(BONUS_COOLDOWN_DURATION);

    switch (bonus.type)
    {
        case ExpandPaddle: player.shape.setScale({1.5f, 1}); break;
        case ShrinkPaddle: player.shape.setScale({0.5f, 1}); break;
        case MagnetPaddle: player.magnetic = true; break;
        case AccelerateBall: for (Ball& ball : balls) { ball.speed = BALL_SPEED * 1.25f; } break;
        case DecelerateBall: for (Ball& ball : balls) { ball.speed = BALL_SPEED * 0.75f; } break;
        case MultiBall: EventBallSpawn(); break;
        case FireBall: for (Ball& ball : balls) { ball.fire = true; ball.shape.setFillColor(BALL_FIRE_COLOR); } break;
        case InvincibleBall: for (Ball& ball : balls) { ball.invincible = true; } break;
        default: break;
    }

    bonus.text.setCharacterSize(BONUS_TEXT_SIZE);
    bonus.text.setString(std::string(magic_enum::enum_name(bonus.type)));
    bonus.text.setPosition({gConfig.windowSize.x * 0.75f, 
                            gConfig.windowSize.y * 0.375f + BONUS_TEXT_OFFSET_Y * bonuses.size()});

    bonuses.emplace_back(bonus);
}

void Game::EventBonusDisable(const Bonus& bonus)
{
    switch (bonus.type)
    {
        case ExpandPaddle:
        case ShrinkPaddle: player.shape.setScale({1, 1}); break;
        case MagnetPaddle: player.magnetic = false; break;
        case AccelerateBall:
        case DecelerateBall: for (Ball& ball : balls) { ball.speed = BALL_SPEED; } break;
        case MultiBall: break;
        case FireBall: for (Ball& ball : balls) { ball.fire = false; ball.shape.setFillColor(BALL_COLOR); } break;
        case InvincibleBall: for (Ball& ball : balls) { ball.invincible = false; } break;
        default: break;
    }
}

void Game::HandleCollisions()
{
    HandleCollisionsBallsMap();

    std::size_t lastBricksCount = bricks.size();

    for (auto& ball : balls)
    {
        HandleCollisionsBallBricks(ball);
    }

    if (bricks.empty())
    {
        LOG_INFO("You Win!");
        ctx.scenes.RestartCurrentScene();
        return;
    }

    std::size_t newBonuses = lastBricksCount - bricks.size();

    for (std::size_t i = 0; i < newBonuses && (bonuses.size() < BonusType::Count); i++)
    {
        EventBonusEnable();
    }
}

void Game::HandleCollisionsBallsMap()
{
    for (auto& ball : balls)
    {
        HandleCollisionsBallMap(ball);
    }

    std::erase_if(balls, [](const Ball& ball) { return !ball.alive; });
}

void Game::HandleCollisionsBallMap(Ball& ball)
{
    if (ball.direction.y > 0 && Intersects(ball.shape, player.shape))
    {
        ResolveCollisionBallPlayer(ball);
    }

    if ((ball.direction.x < 0 && IsOutsideWindowLeft(ball.shape)) ||
        (ball.direction.x > 0 && IsOutsideWindowRight(ball.shape)))
    {
        ball.direction.x *= -1;
    }

    if (ball.direction.y < 0 && IsOutsideWindowTop(ball.shape))
    {
        ball.direction.y *= -1;
    }

    if (ball.direction.y > 0 && ball.shape.getPosition().y > player.shape.getPosition().y)
    {
        ResolveCollisionBallDown(ball);
    }
}

void Game::ResolveCollisionBallPlayer(Ball& ball)
{
    float ballRelativePositionX = ball.shape.getPosition().x - player.shape.getPosition().x;
    float impactFactorX = ballRelativePositionX / (player.shape.getGlobalBounds().size.x / 2);
    float ballHitPositionX = std::clamp(impactFactorX, -1.f, 1.f);

    float angle = BALL_ANGLE_MAX.asRadians() * ballHitPositionX;
    ball.direction = {std::sin(angle), -std::cos(angle)};
}

void Game::ResolveCollisionBallDown(Ball& ball)
{
    if (ball.invincible)
    {
        ball.direction.y *= -1;
        return;
    }

    player.stats.lives--;
    player.stats.livesText.setString("Lives: " + std::to_string(player.stats.lives));

    if (player.stats.lives == 0)
    {
        LOG_INFO("You Lose!");
        ctx.scenes.RestartCurrentScene();
    }
    else if (balls.size() == 1)
    {
        EventBallReset(ball);
    }
    else
    {
        ball.alive = false;
    }
}

void Game::HandleCollisionsBallBricks(Ball& ball)
{
    for (auto& brick : bricks)
    {
        if (Intersects(ball.shape, brick.shape))
        {
            ResolveCollisionBallBrick(ball, brick);
            break;
        }
    }

    std::erase_if(bricks, [](const Brick& brick) { return brick.lives == 0; });
}

void Game::ResolveCollisionBallBrick(Ball& ball, Brick& brick)
{
    if (ball.fire)
    {
        brick.lives = 0;
    }
    else
    {
        brick.lives--;
    }

    if (brick.lives > 0)
    {
        brick.shape.setFillColor(BRICK_COLORS[brick.lives - 1]);
    }
    else
    {
        player.stats.score++;
        player.stats.scoreText.setString("Score: " + std::to_string(player.stats.score));
    }

    ResolveCollisionBallBrickBounce(ball, brick);
}

void Game::ResolveCollisionBallBrickBounce(Ball& ball, Brick& brick)
{
    if (auto intersection = Intersects(ball.shape, brick.shape))
    {
        if (intersection->size.x < intersection->size.y)
        {
            ball.direction.x *= -1;
            float correction = intersection->size.x / 2 + 1;
            ball.shape.move({(ball.direction.x > 0) ? correction : -correction, 0});
        }
        else
        {
            ball.direction.y *= -1;
            float correction = intersection->size.y / 2 + 1;
            ball.shape.move({0, (ball.direction.y > 0) ? correction : -correction});
        }
    }
}

std::vector<BonusType> Game::GetBonusTypesAllowed() const
{
    std::vector<BonusType> types;

    for (int i = 0; i < BonusType::Count; i++)
    {
        BonusType candidate = BonusType(i);

        bool allowed = std::ranges::none_of(bonuses, [&](const Bonus& bonus) {
            return bonus.type == candidate || !IsBonusCompatible(bonus.type, candidate);
        });

        if (allowed)
        {
            types.emplace_back(candidate);
        }
    }

    return types;
}

bool Game::IsBonusCompatible(BonusType a, BonusType b) const
{
    if ((a == ExpandPaddle && b == ShrinkPaddle) ||
        (a == ShrinkPaddle && b == ExpandPaddle))
    {
        return false;
    }

    if ((a == AccelerateBall && b == DecelerateBall) ||
        (a == DecelerateBall && b == AccelerateBall))
    {
        return false;
    }

    return true;
}

bool Game::IsBallNearPlayer(const Ball& ball) const
{
    float distanceX = std::abs(ball.shape.getPosition().x - player.shape.getPosition().x);

    bool nearPositionX = distanceX < BALL_MAGNET_RANGE;
    bool nearPositionY = ball.shape.getPosition().y > player.shape.getPosition().y - BALL_MAGNET_RANGE;

    return nearPositionX && nearPositionY;
}

void Game::Render() const
{
    for (const auto& brick : bricks)
    {
        ctx.renderer.Draw(brick.shape);
    }

    for (const auto& ball : balls)
    {
        ctx.renderer.Draw(ball.shape);
    }

    ctx.renderer.Draw(player.shape);

    for (const auto& bonus : bonuses)
    {
        ctx.renderer.Draw(bonus.text);
    }

    ctx.renderer.Draw(player.stats.scoreText);
    ctx.renderer.Draw(player.stats.livesText);
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        for (auto& bonus : bonuses)
        {
            bonus.endCooldown.Stop();
        }
    }
    else
    {
        for (auto& bonus : bonuses)
        {
            bonus.endCooldown.Start();
        }
    }
}