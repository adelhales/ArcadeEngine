// Copyright (c) 2025 Adel Hales

#include "SpaceInvaders.h"

using namespace SpaceInvaders;

Game::Game(EngineContext& context) :
    Scene(context)
{
    InitPlayer();
    InitWave();
}

void Game::InitPlayer()
{
    player.shape.setTexture(ctx.resources.FetchTexture(PLAYER_TEXTURE_FILENAME));
    player.shape.setFillColor(PLAYER_COLOR);
    player.shape.setSize(gConfig.windowSize * 0.045f);
    player.shape.setOrigin(player.shape.getGeometricCenter());
    player.shape.setRotation(sf::degrees(-90));

    player.speed = PLAYER_SPEED;

    player.stats.scoreText.setFillColor(PLAYER_SCORE_TEXT_COLOR);
    player.stats.livesText.setFillColor(PLAYER_LIVES_TEXT_COLOR);

    player.shootCooldown.SetDuration(PLAYER_SHOOT_COOLDOWN_DURATION);
}

void Game::InitWave()
{
    wave.textures = {ctx.resources.FetchTexture(ENEMY_1_TEXTURE_FILENAME),
                     ctx.resources.FetchTexture(ENEMY_2_TEXTURE_FILENAME),
                     ctx.resources.FetchTexture(ENEMY_3_TEXTURE_FILENAME)};
    
    wave.shootCooldown.SetDuration(WAVE_SHOOT_COOLDOWN_DURATION);
}

void Game::Start()
{
    ctx.cursor.SetVisible(false);

    BindInputs();

    bullets.clear();

    StartPlayer();
    StartWave();
    StartBunkers();
}

void Game::BindInputs()
{
    ctx.input.Bind(MoveLeft, Input::Keyboard{sf::Keyboard::Scan::Left});
    ctx.input.Bind(MoveLeft, Input::Keyboard{sf::Keyboard::Scan::A});
    ctx.input.Bind(MoveLeft, Input::Axis{sf::Joystick::Axis::X, -0.3f});

    ctx.input.Bind(MoveRight, Input::Keyboard{sf::Keyboard::Scan::Right});
    ctx.input.Bind(MoveRight, Input::Keyboard{sf::Keyboard::Scan::D});
    ctx.input.Bind(MoveRight, Input::Axis{sf::Joystick::Axis::X, 0.3f});

    ctx.input.Bind(Shoot, Input::Keyboard{sf::Keyboard::Scan::Space});
    ctx.input.Bind(Shoot, Input::Gamepad{GamepadButton::South});
}

void Game::StartPlayer()
{
    player.shape.setPosition({gConfig.windowSize.x / 2, gConfig.windowSize.y - player.shape.getSize().y});

    player.stats.score = 0;
    player.stats.scoreText.setOutlineThickness(3);
    player.stats.scoreText.setString("Score: 0");
    player.stats.scoreText.setPosition({5, 5});

    player.stats.lives = PLAYER_LIVES;
    player.stats.livesText.setOutlineThickness(3);
    player.stats.livesText.setString("Lives: " + std::to_string(player.stats.lives));
    sf::Vector2f offset(0, player.stats.scoreText.getGlobalBounds().size.y * 2);
    player.stats.livesText.setPosition(player.stats.scoreText.getPosition() + offset);

    player.shootCooldown.Restart();
}

void Game::StartWave()
{
    wave.enemies.resize(WAVE_COUNT.x);

    for (int i = 0; i < WAVE_COUNT.x; i++)
    {
        wave.enemies[i].resize(WAVE_COUNT.y);

        for (int j = 0; j < WAVE_COUNT.y; j++)
        {
            auto& enemy = wave.enemies[i][j];

            StartEnemy(enemy, i, j);
        }
    }

    wave.direction = {1, 0};

    wave.moveCooldown.SetDuration(WAVE_MOVE_COOLDOWN_DURATION);

    wave.moveCooldown.Restart();
    wave.shootCooldown.Restart();
}

void Game::StartEnemy(Enemy& enemy, int i, int j)
{
    sf::Vector2f size = gConfig.windowSize.componentWiseMul({0.04f, 0.03f});

    enemy.shape.setFillColor(ENEMY_FULL_COLOR);
    enemy.shape.setTexture(wave.textures[(j + 1) / 2]);
    enemy.shape.setSize(size);
    enemy.shape.setOrigin(enemy.shape.getGeometricCenter());
    enemy.shape.setPosition({size.x + size.x * 2 * i, size.y + size.y * 2 * j});

    enemy.lives = ENEMY_LIVES;
}

void Game::StartBunkers()
{
    bunkers.clear();

    for (int i = 0; i < BUNKER_COUNT; i++)
    {
        auto& bunker = bunkers.emplace_back();

        StartBunker(bunker, i);
    }
}

void Game::StartBunker(Bunker& bunker, int i)
{    
    sf::Vector2f partSize = BUNKER_SIZE.componentWiseDiv(sf::Vector2f(BUNKER_PART_COUNT));

    float totalWidth = BUNKER_COUNT * BUNKER_SIZE.x + (BUNKER_COUNT - 1) * BUNKER_SPACING_X;
    
    sf::Vector2f start(gConfig.windowSize.x / 2, gConfig.windowSize.y - BUNKER_OFFSET_TOP_Y);
    sf::Vector2f offset(-totalWidth / 2 + i * (BUNKER_SIZE.x + BUNKER_SPACING_X), 0);

    bunker.parts.reserve(BUNKER_PART_COUNT.x * BUNKER_PART_COUNT.y);

    for (int x = 0; x < BUNKER_PART_COUNT.x; x++)
    {
        for (int y = 0; y < BUNKER_PART_COUNT.y; y++)
        {
            auto& part = bunker.parts.emplace_back();

            sf::Vector2f left = sf::Vector2f((float)x, (float)y).componentWiseMul(partSize);
            
            StartBunkerPart(part, {start + offset + left, partSize});
        }
    }

    bunker.bounds.setSize(BUNKER_SIZE);
    bunker.bounds.setPosition(start + offset - partSize / 2.f);
}

void Game::StartBunkerPart(BunkerPart& part, sf::FloatRect bounds)
{
    part.shape.setFillColor(BUNKER_COLOR);
    part.shape.setSize(bounds.size);
    part.shape.setOrigin(part.shape.getGeometricCenter());
    part.shape.setPosition(bounds.position);

    part.alive = true;
}

void Game::Update()
{
    if (ctx.input.Pressed(Shoot) && player.shootCooldown.IsOver())
    {
        EventPlayerShoot();
        player.shootCooldown.Restart();
    }

    if (wave.moveCooldown.IsOver())
    {
        EventEnemiesMovement();
        wave.moveCooldown.Restart();
    }

    if (wave.shootCooldown.IsOver())
    {
        EventEnemyShoot();
        wave.shootCooldown.Restart();
    }

    UpdatePlayer();
    UpdateBullets();

    HandleCollisions();
}

void Game::UpdatePlayer()
{
    sf::Vector2f lastPosition = player.shape.getPosition();

    if (ctx.input.Pressed(MoveLeft))
    {
        player.shape.move({-player.speed * ctx.time.GetDeltaTime(), 0});
    }
    if (ctx.input.Pressed(MoveRight))
    {
        player.shape.move({player.speed * ctx.time.GetDeltaTime(), 0});
    }

    if (IsOutsideWindowLeft(player.shape) || IsOutsideWindowRight(player.shape))
    {
        player.shape.setPosition(lastPosition);
    }
}

void Game::UpdateBullets()
{
    for (auto& bullet : bullets)
    {
        bullet.shape.move(bullet.direction * bullet.speed * ctx.time.GetDeltaTime());
    }
}

void Game::EventPlayerShoot()
{
    auto& bullet = bullets.emplace_back();

    bullet.shape.setFillColor(PLAYER_BULLET_COLOR);
    bullet.shape.setRadius(PLAYER_BULLET_RADIUS);
    bullet.shape.setOrigin(bullet.shape.getGeometricCenter());
    bullet.shape.setPosition(player.shape.getPosition() - sf::Vector2f(0, player.shape.getSize().y));

    bullet.direction = {0, -1};
    bullet.speed = PLAYER_BULLET_SPEED;

    bullet.alive = true;
}

void Game::EventEnemiesMovement()
{
    for (auto& col : wave.enemies)
    {
        for (auto& enemy : col)
        {
            enemy.shape.move(wave.direction.componentWiseMul({enemy.shape.getSize() / 2.f}));
        }
    }

    wave.direction.y = 0;
}

void Game::EventEnemyShoot()
{
    auto& enemy = wave.enemies[ctx.random.Int(0, (int)wave.enemies.size() - 1)].back();

    auto& bullet = bullets.emplace_back();

    bullet.shape.setFillColor(ENEMY_BULLET_COLOR);
    bullet.shape.setRadius(ENEMY_BULLET_RADIUS);
    bullet.shape.setOrigin(bullet.shape.getGeometricCenter());
    bullet.shape.setPosition(enemy.shape.getPosition() + sf::Vector2f(0, enemy.shape.getSize().y));

    bullet.direction = {0, 1};
    bullet.speed = ENEMY_BULLET_SPEED;

    bullet.alive = true;
}

void Game::EventEnemyDead()
{
    wave.moveCooldown.SetDuration(wave.moveCooldown.GetDuration() * WAVE_SPEEDUP_FACTOR);

    player.stats.score++;
    player.stats.scoreText.setString("Score: " + std::to_string(player.stats.score));
}

void Game::HandleCollisions()
{
    HandleCollisionsEnemiesMap();
    HandleCollisionsEnemiesBunker();
    HandleCollisionsBullets();

    if (wave.enemies.empty())
    {
        LOG_INFO("You Win!");
        ctx.scenes.RestartCurrentScene();
    }
    else if (player.stats.lives <= 0)
    {
        LOG_INFO("You Lose!");
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::HandleCollisionsEnemiesMap()
{
    sf::RectangleShape& leftmostEnemy = wave.enemies.front().back().shape;
    sf::RectangleShape& rightmostEnemy = wave.enemies.back().back().shape;

    sf::Vector2f leftmostPosition = leftmostEnemy.getPosition() + wave.direction * leftmostEnemy.getSize().x;
    sf::Vector2f rightmostPosition = rightmostEnemy.getPosition() + wave.direction * rightmostEnemy.getSize().x;
    rightmostPosition.x--; // Pixel perfect collision

    if ((wave.direction.x < 0 && IsOutsideWindow(leftmostPosition)) ||
        (wave.direction.x > 0 && IsOutsideWindow(rightmostPosition)))
    {
        wave.direction.x *= -1;
        wave.direction.y = 1;
    }
}

void Game::HandleCollisionsEnemiesBunker()
{
    if (bunkers.empty())
    {
        return;
    }
    
    float bunkerPositionY = bunkers.back().parts.back().shape.getPosition().y;

    bool collision = std::ranges::any_of(wave.enemies, [&](const auto& col) {
        return col.back().shape.getPosition().y > bunkerPositionY - col.back().shape.getSize().y * 2;
    });

    if (collision)
    {
        player.stats.lives = 0;
    }
}

void Game::HandleCollisionsBullets()
{
    for (auto& bullet : bullets)
    {
        HandleCollisionsBulletPlayer(bullet);
        HandleCollisionsBulletEnemies(bullet);
        HandleCollisionsBulletBunkers(bullet);
    }

    std::erase_if(bullets, [](const Bullet& bullet) {
        return !bullet.alive || IsOutsideWindow(bullet.shape.getPosition());
    });
}

void Game::HandleCollisionsBulletPlayer(Bullet& bullet)
{
    if (Intersects(bullet.shape, player.shape))
    {
        bullet.alive = false;

        player.stats.lives--;
        player.stats.livesText.setString("Lives: " + std::to_string(player.stats.lives));
    }
}

void Game::HandleCollisionsBulletEnemies(Bullet& bullet)
{
    for (auto& col : wave.enemies)
    {
        if (!bullet.alive)
        {
            break;
        }

        auto& enemy = col.back();

        if (Intersects(bullet.shape, enemy.shape))
        {
            ResolveCollisionBulletEnemy(bullet, enemy);
        }

        if (enemy.lives == 0)
        {
            EventEnemyDead();
            col.pop_back();
        }
    }

    std::erase_if(wave.enemies, [](const auto& col) { return col.empty(); });
}

void Game::ResolveCollisionBulletEnemy(Bullet& bullet, Enemy& enemy)
{
    bullet.alive = false;
    enemy.lives--;
    enemy.shape.setFillColor(ENEMY_HALF_COLOR);
}

void Game::HandleCollisionsBulletBunkers(Bullet& bullet)
{
    for (auto& bunker : bunkers)
    {
        if (!bullet.alive)
        {
            break;
        }

        if (Intersects(bullet.shape, bunker.bounds))
        {
            ResolveCollisionBulletBunker(bullet, bunker);
        }
    }

    player.stats.lives -= (int)std::erase_if(bunkers, [](const Bunker& bunker) { return bunker.parts.empty(); });
    player.stats.livesText.setString("Lives: " + std::to_string(player.stats.lives));
}

void Game::ResolveCollisionBulletBunker(Bullet& bullet, Bunker& bunker)
{
    for (auto& part : bunker.parts)
    {
        if (!bullet.alive)
        {
            break;
        }

        if (Intersects(bullet.shape, part.shape))
        {
            ResolveCollisionBulletBunkerPart(bullet, part);
        }
    }

    std::erase_if(bunker.parts, [](const BunkerPart& part) { return !part.alive; });
}

void Game::ResolveCollisionBulletBunkerPart(Bullet& bullet, BunkerPart& part)
{
    bullet.alive = false;
    part.alive = false;
}

void Game::Render() const
{
    ctx.renderer.Draw(player.shape);

    for (const auto& bunker : bunkers)
    {
        for (const auto& part : bunker.parts)
        {
            ctx.renderer.Draw(part.shape);
        }
    }

    for (const auto& col : wave.enemies)
    {
        for (const auto& enemy : col)
        {
            ctx.renderer.Draw(enemy.shape);
        }
    }

    for (const auto& bullet : bullets)
    {
        ctx.renderer.Draw(bullet.shape);
    }

    ctx.renderer.Draw(player.stats.scoreText);
    ctx.renderer.Draw(player.stats.livesText);
}