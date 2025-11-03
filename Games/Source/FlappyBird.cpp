// Copyright (c) 2025 Adel Hales

#include "FlappyBird.h"

#include <numeric>

using namespace FlappyBird;

Game::Game(EngineContext& context) :
    Scene(context)
{
    InitGeneration();
    InitBackground();
}

void Game::InitGeneration()
{
    InitBirds();
    InitStats();
}

void Game::InitBackground()
{
    background.setTexture(ctx.resources.FetchTexture(BACKGROUND_TEXTURE_FILENAME));
    background.setFillColor(BACKGROUND_COLOR);
    background.setSize(gConfig.windowSize);
}

void Game::InitBirds()
{
    for (auto& bird : generation.birds)
    {
        InitBird(bird);
    }
}

void Game::InitBird(Bird& bird)
{
    bird.shape.setTexture(ctx.resources.FetchTexture(BIRD_TEXTURE_FILENAME));
    bird.shape.setFillColor(ctx.random.Color(sf::Color::Black, sf::Color::White));
    bird.shape.setSize(gConfig.windowSize.componentWiseMul({0.05f, 0.05f}));
    bird.shape.setOrigin(bird.shape.getGeometricCenter());
    bird.shape.setPosition(gConfig.windowSize / 2.f);
}

void Game::InitStats()
{
    generation.indexText.setFillColor(STATS_TEXT_COLOR);
    generation.indexText.setOutlineThickness(2);
    generation.indexText.setString("Generation: 0");
    generation.indexText.setPosition({10, 10});

    generation.birdCountText.setFillColor(STATS_TEXT_COLOR);
    generation.birdCountText.setOutlineThickness(2);
    generation.birdCountText.setString("Birds: " + std::to_string(BIRD_COUNT));
    sf::Vector2f offsetBirdCount(0, generation.indexText.getGlobalBounds().size.y * 2);
    generation.birdCountText.setPosition(generation.indexText.getPosition() + offsetBirdCount);

    generation.scoreText.setFillColor(STATS_TEXT_COLOR);
    generation.scoreText.setOutlineThickness(2);
    generation.scoreText.setString("Score: 0");
    sf::Vector2f offsetScore(0, generation.birdCountText.getGlobalBounds().size.y * 2);
    generation.scoreText.setPosition(generation.birdCountText.getPosition() + offsetScore);
}

void Game::Start()
{
    obstacles.clear();

    StartGeneration();

    EventObstacleSpawn();

    clock.restart();
}

void Game::StartGeneration()
{
    StartBirds();
    StartStats();
}

void Game::StartBirds()
{
    for (auto& bird : generation.birds)
    {
        StartBird(bird);
    }
}

void Game::StartBird(Bird& bird)
{
    EventRestartBird(bird);
    std::ranges::generate(bird.weights, [&] { return ctx.random.Float(-1, 1); });
}

void Game::StartStats()
{
    generation.index = 0;
    generation.indexText.setString("Generation: 0");

    generation.birdCount = BIRD_COUNT;
    generation.birdCountText.setString("Birds: " + std::to_string(generation.birdCount));

    generation.score = 0;
    generation.scoreText.setString("Score: 0");
}

void Game::Update()
{
    UpdateGeneration();
    UpdateObstacles();

    if (ShouldObstacleSpawn())
    {
        EventObstacleSpawn();
    }

    HandleCollisions();
}

void Game::UpdateGeneration()
{
    bool scored = false;

    for (auto& bird : generation.birds)
    {
        if (!bird.alive)
        {
            continue;
        }

        int previousTargetObstacleIndex = bird.targetObstacleIndex;

        UpdateBird(bird);

        if (bird.targetObstacleIndex > previousTargetObstacleIndex)
        {
            scored = true;
        }
    }

    if (scored)
    {
        generation.score++;
        generation.scoreText.setString("Score: " + std::to_string(generation.score));
    }
}

void Game::UpdateBird(Bird& bird)
{
    UpdateBirdTargetObstacle(bird);

    if (ShouldBirdJump(bird))
    {
        bird.velocity.y = -BIRD_JUMP_FORCE;
    }

    UpdateBirdMove(bird);
    UpdateBirdRotation(bird);
}

void Game::UpdateBirdTargetObstacle(Bird& bird)
{
    for (std::size_t i = 0; i < obstacles.size(); i++)
    {
        float birdLeftX = bird.shape.getPosition().x - bird.shape.getSize().x / 2;
        float obstacleRightX = obstacles[i].top.getPosition().x + obstacles[i].top.getSize().x / 2;

        if (birdLeftX < obstacleRightX)
        {
            bird.targetObstacleIndex = (int)i;
            break;
        }
    }
}

void Game::UpdateBirdMove(Bird& bird)
{
    bird.velocity.y += BIRD_GRAVITY * ctx.time.GetDeltaTime();
    bird.shape.move(bird.velocity * ctx.time.GetDeltaTime());
}

void Game::UpdateBirdRotation(Bird& bird)
{
    float velocityAdjusted = bird.velocity.y + BIRD_ROTATION_VELOCITY_OFFSET;
    float normalizedVelocity = std::clamp(velocityAdjusted / BIRD_ROTATION_VELOCITY_RANGE, 0.f, 1.f);
    float targetAngle = std::lerp(BIRD_ANGLE_MIN, BIRD_ANGLE_MAX, normalizedVelocity);

    bird.shape.setRotation(sf::degrees(targetAngle));
}

void Game::UpdateObstacles()
{
    for (auto& obstacle : obstacles)
    {
        UpdateObstacle(obstacle);
    }
}

void Game::UpdateObstacle(Obstacle& obstacle)
{
    obstacle.top.move({-OBSTACLE_SPEED * ctx.time.GetDeltaTime(), 0});
    obstacle.bottom.move({-OBSTACLE_SPEED * ctx.time.GetDeltaTime(), 0});
}

void Game::EventRestartBird(Bird& bird)
{
    bird.shape.setPosition({bird.shape.getPosition().x, gConfig.windowSize.y / 2});

    bird.velocity = {};
    bird.targetObstacleIndex = 0;
    bird.alive = true;
    bird.timeAlive = 0;
}

void Game::EventObstacleSpawn()
{
    auto& obstacle = obstacles.emplace_back();

    float gapSize = ctx.random.Float(OBSTACLE_GAP_MIN, OBSTACLE_GAP_MAX);
    float centerY = ctx.random.Float(gapSize / 2, gConfig.windowSize.y - gapSize / 2);
    float topHeight = centerY - gapSize / 2;
    float bottomHeight = gConfig.windowSize.y - (centerY + gapSize / 2);

    obstacle.top.setTexture(ctx.resources.FetchTexture(PIPE_TEXTURE_FILENAME));
    obstacle.top.setFillColor(BACKGROUND_COLOR);
    obstacle.top.setSize({OBSTACLE_WIDTH, topHeight});
    obstacle.top.setOrigin(obstacle.top.getGeometricCenter());
    obstacle.top.setPosition({gConfig.windowSize.x + OBSTACLE_WIDTH / 2, topHeight / 2});

    obstacle.bottom.setTexture(ctx.resources.FetchTexture(PIPE_TEXTURE_FILENAME));
    obstacle.bottom.setFillColor(BACKGROUND_COLOR);
    obstacle.bottom.setSize({OBSTACLE_WIDTH, bottomHeight});
    obstacle.bottom.setOrigin(obstacle.bottom.getGeometricCenter());
    obstacle.bottom.setPosition({gConfig.windowSize.x + OBSTACLE_WIDTH / 2,
                                 gConfig.windowSize.y - bottomHeight / 2});
    obstacle.bottom.setRotation(sf::degrees(180));
}

void Game::EventGenerationNext()
{
    obstacles.clear();

    std::ranges::sort(generation.birds, [](const Bird& a, const Bird& b) {
        return a.timeAlive > b.timeAlive;
    });

    std::array<std::array<float, BIRD_WEIGHT_COUNT>, GENERATION_ELITE_COUNT> eliteWeights;

    for (int i = 0; i < GENERATION_ELITE_COUNT; i++)
    {
        eliteWeights[i] = generation.birds[i].weights;
    }

    for (int i = 0; i < BIRD_COUNT; i++)
    {
        EventRestartBird(generation.birds[i]);

        if (i < GENERATION_ELITE_COUNT)
        {
            continue;
        }

        int eliteIndex = ctx.random.Int(0, GENERATION_ELITE_COUNT - 1);
        const std::array<float, 4>& parentWeights = eliteWeights[eliteIndex];

        for (std::size_t j = 0; j < parentWeights.size(); j++)
        {
            generation.birds[i].weights[j] = parentWeights[j] + ctx.random.Float(-0.1f, 0.1f);
        }
    }

    EventGenerationNextStats();
    EventObstacleSpawn();

    clock.restart();
}

void Game::EventGenerationNextStats()
{
    generation.index++;
    generation.indexText.setString("Generation: " + std::to_string(generation.index));

    generation.birdCount = (int)generation.birds.size();
    generation.birdCountText.setString("Birds: " + std::to_string(generation.birdCount));

    generation.score = 0;
    generation.scoreText.setString("Score: " + std::to_string(generation.score));
}

void Game::HandleCollisions()
{
    HandleCollisionsBirds();
    HandleCollisionsObstacle();

    if (generation.birdCount == 0)
    {
        EventGenerationNext();
    }
}

void Game::HandleCollisionsBirds()
{
    for (auto& bird : generation.birds)
    {
        if (bird.alive && ShouldBirdDie(bird))
        {
            ResolveCollisionBird(bird);
        }
    }
}

void Game::ResolveCollisionBird(Bird& bird)
{
    bird.alive = false;
    bird.timeAlive = clock.getElapsedTime().asSeconds();

    generation.birdCount--;
    generation.birdCountText.setString("Birds: " + std::to_string(generation.birdCount));
}

void Game::HandleCollisionsObstacle()
{
    if (obstacles[0].top.getPosition().x + obstacles[0].top.getSize().x / 2 < 0)
    {
        ResolveCollisionObstacle();
    }
}

void Game::ResolveCollisionObstacle()
{
    obstacles.erase(obstacles.begin());
}

float Game::GetSigmoid(float value) const
{
    return 1 / (1 + std::exp(-value));
}

bool Game::ShouldBirdJump(const Bird& bird) const
{
    const Obstacle& closest = obstacles[bird.targetObstacleIndex];
    
    float topLeftX = closest.top.getPosition().x - closest.top.getSize().x / 2;
    float topBottomY = closest.top.getPosition().y + closest.top.getSize().y / 2;
    float bottomTopY = closest.bottom.getPosition().y - closest.bottom.getSize().y / 2;

    float birdRightX = bird.shape.getPosition().x + bird.shape.getSize().x / 2;
    float birdTopY = bird.shape.getPosition().y - bird.shape.getSize().y / 2;
    float birdBottomY = bird.shape.getPosition().y + bird.shape.getSize().y / 2;

    std::array<float, 4> inputs = {topLeftX - birdRightX,
                                   topBottomY - birdTopY,
                                   bottomTopY - birdBottomY,
                                   bird.velocity.y};

    float value = std::inner_product(inputs.begin(), inputs.end(), bird.weights.begin(), 0.f);

    return GetSigmoid(value) > 0.5f;
}

bool Game::ShouldBirdDie(const Bird& bird) const
{
    const Obstacle& closest = obstacles[bird.targetObstacleIndex];

    return IsOutsideWindowBottom(bird.shape) || IsOutsideWindowTop(bird.shape) ||
           Intersects(bird.shape, closest.top) || Intersects(bird.shape, closest.bottom);
}

bool Game::ShouldObstacleSpawn() const
{
    return gConfig.windowSize.x - obstacles.back().top.getPosition().x >= OBSTACLE_SPAWN_DISTANCE;
}

void Game::Render() const
{
    ctx.renderer.Draw(background);

    for (const auto& bird : generation.birds)
    {
        if (bird.alive)
        {
            ctx.renderer.Draw(bird.shape);
        }
    }

    for (const auto& obstacle : obstacles)
    {
        ctx.renderer.Draw(obstacle.top);
        ctx.renderer.Draw(obstacle.bottom);
    }

    ctx.renderer.Draw(generation.indexText);
    ctx.renderer.Draw(generation.birdCountText);
    ctx.renderer.Draw(generation.scoreText);
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        clock.stop();
    }
    else
    {
        clock.start();
    }
}