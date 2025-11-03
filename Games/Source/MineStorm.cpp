// Copyright (c) 2025 Adel Hales

#include "MineStorm.h"

using namespace MineStorm;

Game::Game(EngineContext& context) :
    Scene(context),
    explosionTexture(*ctx.resources.FetchTexture(EXPLOSION_TEXTURE_FILENAME))
{
    InitPlayer();
    InitBackground();
}

void Game::InitPlayer()
{
    player.shape.setTexture(ctx.resources.FetchTexture(PLAYER_TEXTURE_FILENAME));
    player.shape.setFillColor(PLAYER_COLOR);
    player.shape.setSize(PLAYER_SIZE);
    player.shape.setOrigin(player.shape.getGeometricCenter());

    player.shield.setFillColor(sf::Color::Transparent);
    player.shield.setOutlineColor(PLAYER_SHIELD_COLOR);
    player.shield.setOutlineThickness(1);
    player.shield.setSize(player.shape.getSize());
    player.shield.setOrigin(player.shape.getOrigin());
    
    player.stats.scoreText.setOutlineThickness(1);
    player.stats.livesText.setOutlineThickness(1);

    player.shootCooldown.SetDuration(PLAYER_SHOOT_COOLDOWN_DURATION);
    player.shieldCooldown.SetDuration(PLAYER_SHIELD_COOLDOWN_DURATION);
}

void Game::InitBackground()
{
    background.setTexture(ctx.resources.FetchTexture(BACKGROUND_TEXTURE_FILENAME));
    background.setSize(gConfig.windowSize);

    foreground.setTexture(ctx.resources.FetchTexture(FOREGROUND_TEXTURE_FILENAME));
    foreground.setSize(background.getSize());
}

void Game::Start()
{
    ctx.cursor.SetVisible(false);

    BindInputs();

    StartPlayer();

    EventWaveNew();
}

void Game::BindInputs()
{
    ctx.input.Bind(RotateLeft, Input::Keyboard{sf::Keyboard::Scan::Left});
    ctx.input.Bind(RotateLeft, Input::Keyboard{sf::Keyboard::Scan::A});
    ctx.input.Bind(RotateLeft, Input::Axis{sf::Joystick::Axis::X, -0.25f});

    ctx.input.Bind(RotateRight, Input::Keyboard{sf::Keyboard::Scan::Right});
    ctx.input.Bind(RotateRight, Input::Keyboard{sf::Keyboard::Scan::D});
    ctx.input.Bind(RotateRight, Input::Axis{sf::Joystick::Axis::X, 0.25f});

    ctx.input.Bind(Thrust, Input::Keyboard{sf::Keyboard::Scan::Up});
    ctx.input.Bind(Thrust, Input::Keyboard{sf::Keyboard::Scan::W});
    ctx.input.Bind(Thrust, Input::Gamepad{GamepadButton::R2});

    ctx.input.Bind(Shoot, Input::Keyboard{sf::Keyboard::Scan::Space});
    ctx.input.Bind(Shoot, Input::Gamepad{GamepadButton::South});
}

void Game::StartPlayer()
{
    player.stats.score = 0;
    player.stats.scoreText.setString("Score: 0");
    player.stats.scoreText.setPosition({50, 40});

    player.stats.lives = PLAYER_LIVES;
    sf::Vector2f offset(0, player.stats.scoreText.getGlobalBounds().size.y * 2);
    player.stats.livesText.setPosition(player.stats.scoreText.getPosition() + offset);
}

void Game::Update()
{
    if (ctx.input.Pressed(Shoot) && player.shootCooldown.IsOver())
    {
        EventPlayerShoot();
        player.shootCooldown.Restart();
    }

    if (enemies.empty())
    {
        EventWaveNew();
    }

    UpdatePlayer();
    UpdateEnemies();
    UpdateBullets();
    UpdateExplosions();

    HandleCollisions();
}

void Game::UpdatePlayer()
{
    UpdatePlayerRotation();

    if (ctx.input.Pressed(Thrust))
    {
        UpdatePlayerAccelerate();
    }

    UpdatePlayerDecelerate();

    player.shape.move(player.velocity * ctx.time.GetDeltaTime());

    if (!player.shieldCooldown.IsOver())
    {
        player.shield.setPosition(player.shape.getPosition());
        player.shield.setRotation(player.shape.getRotation());
    }
}

void Game::UpdatePlayerRotation()
{
    if (ctx.input.Pressed(RotateLeft))
    {
        player.shape.rotate(-PLAYER_TURN_ANGLE_SPEED * ctx.time.GetDeltaTime());
    }
    if (ctx.input.Pressed(RotateRight))
    {
        player.shape.rotate(PLAYER_TURN_ANGLE_SPEED * ctx.time.GetDeltaTime());
    }
}

void Game::UpdatePlayerAccelerate()
{
    sf::Vector2f thrust(1, player.shape.getRotation());

    player.velocity += thrust * PLAYER_ACCELERATION_SPEED * ctx.time.GetDeltaTime();
}

void Game::UpdatePlayerDecelerate()
{
    player.velocity *= (float)std::pow(PLAYER_FRICTION, ctx.time.GetDeltaTime() * 60);
}

void Game::UpdateEnemies()
{
    for (auto& enemy : enemies)
    {
        UpdateEnemy(enemy);
    }
}

void Game::UpdateEnemy(Enemy& enemy)
{
    switch (enemy.type)
    {
        case Magnetic:
        case MagneticFireball:
        {
            sf::Vector2f direction = player.shape.getPosition() - enemy.shape.getPosition();
            enemy.direction = (direction.lengthSquared() > 0) ? direction.normalized() : sf::Vector2f();
        }
        break;
        default:
            break;
    }

    enemy.shape.move(enemy.direction * enemy.speed * ctx.time.GetDeltaTime());
}

void Game::UpdateBullets()
{
    for (auto& bullet : bullets)
    {
        bullet.shape.move(bullet.direction * bullet.speed * ctx.time.GetDeltaTime());
    }
}

void Game::UpdateExplosions()
{
    for (auto& explosion : explosions)
    {
        explosion.shape.Update(ctx.time.GetDeltaTime());
    }

    std::erase_if(explosions, [](const Explosion& explosion) {
        return explosion.shape.IsFinished();
    });
}

void Game::EventPlayerShoot()
{
    auto& bullet = bullets.emplace_back();

    bullet.shape.setFillColor(PLAYER_BULLET_COLOR);
    bullet.shape.setRadius(PLAYER_BULLET_RADIUS);
    bullet.shape.setOrigin(bullet.shape.getGeometricCenter());
    bullet.shape.setPosition(player.shape.getPosition());

    bullet.direction = {1, player.shape.getRotation()};
    bullet.speed = PLAYER_BULLET_SPEED;

    bullet.alive = true;
    bullet.type = BulletType::Player;
}

void Game::EventPlayerShipReset()
{
    player.shape.setPosition(gConfig.windowSize / 2.f);
    player.shape.setRotation(sf::Angle::Zero);

    player.velocity = {};

    player.shootCooldown.Restart();
    player.shieldCooldown.Restart();
}

void Game::EventWaveNew()
{
    enemies.clear();
    bullets.clear();
    explosions.clear();

    int totalChildren = (int)std::pow(ENEMY_CHILD_COUNT, ENEMY_SIZE) - 1;
    wave.spawns.resize(ENEMY_COUNT * totalChildren);
    std::ranges::generate(wave.spawns, [&] { return GenerateSpawnPoint(); });

    wave.enemies = {};

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        EnemyType type = (EnemyType)ctx.random.Int(Floating, MagneticFireball);
        wave.enemies.push(EnemySettings{type, ENEMY_SIZE});

        EventEnemySpawn();
    }

    player.stats.lives = PLAYER_LIVES;
    player.stats.livesText.setString("Lives: " + std::to_string(player.stats.lives));

    EventPlayerShipReset();
}

void Game::EventEnemySpawn()
{
    auto& enemy = enemies.emplace_back();

    enemy.shape.setPosition(wave.spawns.back().getPosition());
    wave.spawns.pop_back();

    EnemySettings settings = wave.enemies.front();
    wave.enemies.pop();

    enemy.type = settings.type;
    enemy.shape.setFillColor(ENEMY_COLORS[enemy.type - 1]);

    enemy.size = settings.size;
    enemy.shape.setScale({enemy.size * ENEMY_SCALE, enemy.size * ENEMY_SCALE});

    auto& points = ENEMY_SHAPES[enemy.type - 1];
    enemy.shape.setPointCount(points.size());
    for (std::size_t i = 0; i < points.size(); i++)
    {
        enemy.shape.setPoint(i, points[i]);
    }

    enemy.direction = {1, ctx.random.Angle(sf::Angle::Zero, sf::degrees(360))};
    enemy.speed = ENEMY_SPEED / (int)enemy.type * (ENEMY_SIZE / enemy.size + 1);

    enemy.lives = ENEMY_LIVES;
}

void Game::EventEnemyShoot(const Enemy& enemy)
{
    sf::Vector2f direction = player.shape.getPosition() - enemy.shape.getPosition();

    if (direction.lengthSquared() == 0)
    {
        return;
    }

    auto& bullet = bullets.emplace_back();

    bullet.shape.setFillColor(ENEMY_BULLET_COLOR);
    bullet.shape.setRadius(ENEMY_BULLET_RADIUS);
    bullet.shape.setOrigin(bullet.shape.getGeometricCenter());
    bullet.shape.setPosition(enemy.shape.getPosition());

    bullet.direction = direction.normalized();
    bullet.speed = ENEMY_BULLET_SPEED;

    bullet.alive = true;
    bullet.type = BulletType::Enemy;
}

void Game::EventEnemyDead(const Enemy& enemy)
{
    if (enemy.size > 1)
    {
        for (int i = 0; i < ENEMY_CHILD_COUNT; i++)
        {
            wave.enemies.emplace(EnemySettings{enemy.type, enemy.size - 1});
        }
    }

    if (enemy.type == Fireball || enemy.type == MagneticFireball)
    {
        EventEnemyShoot(enemy);
    }

    player.stats.score++;
    player.stats.scoreText.setString("Score: " + std::to_string(player.stats.score));

    EventExplosion(enemy.shape);
}

void Game::EventExplosion(const sf::Shape& shape)
{
    auto& explosion = explosions.emplace_back();

    explosion.shape.Start({&explosionTexture, EXPLOSION_ANIMATION_FRAMES_PER_ROW,
                           EXPLOSION_ANIMATION_DURATION, false});

    explosion.shape.setFillColor(shape.getFillColor() * EXPLOSION_COLOR_FACTOR);
    explosion.shape.setSize(shape.getGlobalBounds().size * EXPLOSION_SCALE);
    explosion.shape.setOrigin(explosion.shape.getGeometricCenter());
    explosion.shape.setPosition(shape.getPosition());
}

sf::CircleShape Game::GenerateSpawnPoint() const
{
    sf::CircleShape spawn(1);

    spawn.setFillColor(SPAWN_POINT_COLOR);
    spawn.setOrigin(spawn.getGeometricCenter());
    spawn.setPosition(ctx.random.Position({}, gConfig.windowSize));

    return spawn;
}

void Game::HandleCollisions()
{
    HandleCollisionsPlayerMap();
    HandleCollisionsEnemiesMap();
    HandleCollisionsPlayerEnemies();
    HandleCollisionsBullets();

    if (player.stats.lives <= 0)
    {
        LOG_INFO("You Lose!");
        ctx.scenes.RestartCurrentScene();
        return;
    }

    while (!wave.enemies.empty())
    {
        EventEnemySpawn();
    }
}

void Game::HandleCollisionsPlayerMap()
{
    HandleCollisionsShapeMap(player.shape);
}

void Game::HandleCollisionsPlayerEnemies()
{
    for (auto& enemy : enemies)
    {
        if (player.shieldCooldown.IsOver() && Intersects(player.shape, enemy.shape))
        {
            ResolveCollisionPlayerEnemy();
        }
    }
}

void Game::ResolveCollisionPlayerEnemy()
{
    player.stats.lives--;
    player.stats.livesText.setString("Lives: " + std::to_string(player.stats.lives));

    EventExplosion(player.shape);
    EventPlayerShipReset();
}

void Game::HandleCollisionsEnemiesMap()
{
    for (auto& enemy : enemies)
    {
        HandleCollisionsShapeMap(enemy.shape);
    }
}

void Game::HandleCollisionsShapeMap(sf::Shape& shape)
{
    sf::Vector2f lastPosition = shape.getPosition();

    if (IsOutsideWindowLeft(shape))
    {
        shape.setPosition({gConfig.windowSize.x - shape.getGlobalBounds().size.x / 2, lastPosition.y});
    }
    else if (IsOutsideWindowRight(shape))
    {
        shape.setPosition({shape.getGlobalBounds().size.x / 2, lastPosition.y});
    }

    if (IsOutsideWindowTop(shape))
    {
        shape.setPosition({lastPosition.x, gConfig.windowSize.y - shape.getGlobalBounds().size.y / 2});
    }
    else if (IsOutsideWindowBottom(shape))
    {
        shape.setPosition({lastPosition.x, shape.getGlobalBounds().size.y / 2});
    }
}

void Game::HandleCollisionsBullets()
{
    for (auto& bullet : bullets)
    {
        if (bullet.type == BulletType::Enemy && player.stats.lives > 0 && player.shieldCooldown.IsOver())
        {
            if (bullet.alive && Intersects(bullet.shape, player.shape))
            {
                ResolveCollisionBulletPlayer(bullet);
            }
        }
        else if (bullet.type == BulletType::Player)
        {
            HandleCollisionsBulletEnemies(bullet);
        }
    }

    std::erase_if(bullets, [](const Bullet& bullet) {
        return !bullet.alive || IsOutsideWindow(bullet.shape.getPosition());
    });

    for (const auto& enemy : enemies)
    {
        if (enemy.lives <= 0)
        {
            EventEnemyDead(enemy);
        }
    }

    std::erase_if(enemies, [](const Enemy& enemy) { return enemy.lives <= 0; });
}

void Game::ResolveCollisionBulletPlayer(Bullet& bullet)
{
    bullet.alive = false;

    player.stats.lives--;
    player.stats.livesText.setString("Lives: " + std::to_string(player.stats.lives));

    EventExplosion(player.shape);
    EventPlayerShipReset();
}

void Game::HandleCollisionsBulletEnemies(Bullet& bullet)
{
    for (auto& enemy : enemies)
    {
        if (enemy.lives > 0 && bullet.alive && Intersects(bullet.shape, enemy.shape))
        {
            ResolveCollisionBulletEnemy(bullet, enemy);
        }
    }
}

void Game::ResolveCollisionBulletEnemy(Bullet& bullet, Enemy& enemy)
{
    bullet.alive = false;
    enemy.lives--;
}

void Game::Render() const
{
    ctx.renderer.Draw(background);

    for (const auto& enemy : enemies)
    {
        ctx.renderer.Draw(enemy.shape);
    }

    for (const auto& bullet : bullets)
    {
        ctx.renderer.Draw(bullet.shape);
    }

    for (const auto& spawn : wave.spawns)
    {
        ctx.renderer.Draw(spawn);
    }

    for (const auto& explosion : explosions)
    {
        ctx.renderer.Draw(explosion.shape);
    }

    ctx.renderer.Draw(player.shape);

    if (!player.shieldCooldown.IsOver())
    {
        ctx.renderer.Draw(player.shield);
    }

    ctx.renderer.Draw(foreground);

    ctx.renderer.Draw(player.stats.scoreText);
    ctx.renderer.Draw(player.stats.livesText);
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        player.shootCooldown.Stop();
        player.shieldCooldown.Stop();
    }
    else
    {
        player.shootCooldown.Start();
        player.shieldCooldown.Start();
    }
}