// Copyright (c) 2025 Adel Hales

#include "Adventure.h"

using namespace Adventure;

Game::Game(EngineContext& context) :
    Scene(context),
    enemySpawnCooldown(ENEMY_SPAWN_COOLDOWN_DURATION),
    bonusSound(*ctx.resources.FetchSound(BONUS_SOUND_FILENAME))
{
    InitPlayer();
    InitCamera();
    InitSounds();
    InitBackground();
}

void Game::InitPlayer()
{
    player.shape.setFillColor(PLAYER_COLOR);

    player.jumpCooldown.SetDuration(PLAYER_JUMP_COOLDOWN_DURATION);
    player.shootCooldown.SetDuration(PLAYER_SHOOT_COOLDOWN_DURATION);
    player.forceFieldCooldown.SetDuration(PLAYER_FORCE_FIELD_COOLDOWN_DURATION);
}

void Game::InitCamera()
{
    camera.view.setSize(gConfig.windowSize / 6.f);
    camera.smoothFactor = CAMERA_SMOOTH_FACTOR;
}

void Game::InitSounds()
{
    bonusSound.setVolume(BONUS_SOUND_VOLUME);
}

void Game::InitBackground()
{
    background.setTexture(ctx.resources.FetchTexture(BACKGROUND_TEXTURE_FILENAME));
    background.setFillColor(BACKGROUND_COLOR);
    background.setSize(gConfig.windowSize);
}

void Game::Start()
{
    ctx.cursor.SetVisible(!sf::Joystick::isConnected(0));

    BindInputs();

    enemies.clear();
    bullets.clear();

    StartMap();
    StartPlayer();
    StartCamera();

    enemySpawnCooldown.Restart();
}

void Game::BindInputs()
{
    ctx.input.Bind(MoveLeft, Input::Keyboard{sf::Keyboard::Scan::A});
    ctx.input.Bind(MoveLeft, Input::Axis{sf::Joystick::Axis::X, -0.3f});

    ctx.input.Bind(MoveRight, Input::Keyboard{sf::Keyboard::Scan::D});
    ctx.input.Bind(MoveRight, Input::Axis{sf::Joystick::Axis::X, 0.3f});

    ctx.input.Bind(Sprint, Input::Keyboard{sf::Keyboard::Scan::LShift});
    ctx.input.Bind(Sprint, Input::Gamepad{GamepadButton::L2});

    ctx.input.Bind(Jump, Input::Keyboard{sf::Keyboard::Scan::W});
    ctx.input.Bind(Jump, Input::Gamepad{GamepadButton::R1});

    ctx.input.Bind(Climb, Input::Keyboard{sf::Keyboard::Scan::W});
    ctx.input.Bind(Climb, Input::Gamepad{GamepadButton::R1});

    ctx.input.Bind(Shoot, Input::Mouse{sf::Mouse::Button::Left});
    ctx.input.Bind(Shoot, Input::Gamepad{GamepadButton::R2});

    ctx.input.Bind(ForceField, Input::Mouse{sf::Mouse::Button::Middle});
    ctx.input.Bind(ForceField, Input::Gamepad{GamepadButton::R3});
}

void Game::StartMap()
{
    VERIFY(map.LoadFromFile(TILEMAP_LEVEL_FILENAME, TILEMAP_TEXTURE_FILENAME));
}

void Game::StartPlayer()
{
    player.shape.Start({ctx.resources.FetchTexture(PLAYER_TEXTURE_FILENAME),
                        PLAYER_ANIMATION_FRAMES_PER_ROW, PLAYER_ANIMATION_FRAME_DURATION});

    player.shape.setSize(sf::Vector2f(map.GetTileSize()));
    player.shape.setOrigin(player.shape.getGeometricCenter());
    player.shape.setPosition(gConfig.windowSize.componentWiseMul({0.50f, 0.20f}));

    player.forceField.setFillColor(PLAYER_FORCE_FIELD_COLOR);
    player.forceField.setRadius(PLAYER_FORCE_FIELD_RADIUS);
    player.forceField.setOrigin(player.forceField.getGeometricCenter());
    player.forceField.setPosition({0, 0});

    player.verticalVelocity = 0;
    player.state.Clear();

    player.score = 0;
    player.scoreText.setFillColor(STATS_TEXT_COLOR);
    player.scoreText.setOutlineThickness(2);
    player.scoreText.setString("Score: 0");
    player.scoreText.setPosition({10, 10});

    player.lives = PLAYER_LIVES;
    player.livesText.setFillColor(STATS_TEXT_COLOR);
    player.livesText.setOutlineThickness(2);
    player.livesText.setString("Lives: " + std::to_string(player.lives));
    sf::Vector2f offset(0, player.scoreText.getGlobalBounds().size.y * 2);
    player.livesText.setPosition(player.scoreText.getPosition() + offset);

    player.jumpCooldown.Restart();
    player.shootCooldown.Restart();
    player.forceFieldCooldown.Restart();
}

void Game::StartCamera()
{
    camera.view.setCenter(player.shape.getPosition());
}

void Game::Update()
{
    if (ctx.input.Pressed(Shoot) && player.shootCooldown.IsOver())
    {
        EventPlayerShoot();
        player.shootCooldown.Restart();
    }

    if (ctx.input.Pressed(ForceField) && player.forceFieldCooldown.IsOver())
    {
        EventPlayerForceField();
        player.forceFieldCooldown.Restart();
    }

    if (ctx.input.Pressed(Jump) && player.state.Has(Grounded) && player.jumpCooldown.IsOver())
    {
        EventPlayerJump();
        player.jumpCooldown.Restart();
    }

    if (enemySpawnCooldown.IsOver())
    {
        EventEnemySpawn();
        enemySpawnCooldown.Restart();
    }

    UpdatePlayer();
    UpdateCamera();
    UpdateEnemies();
    UpdateBullets();

    HandleCollisions();
}

void Game::UpdatePlayer()
{
    player.lastPosition = player.shape.getPosition();

    UpdatePlayerDirection();
    UpdatePlayerHorizontalMovement();
    UpdatePlayerClimbing();
    UpdatePlayerGravity();
    UpdatePlayerVerticalMovement();
    UpdatePlayerAnimation();
}

void Game::UpdatePlayerDirection()
{
    player.direction.x = 0;

    if (ctx.input.Pressed(MoveLeft))
    {
        player.direction.x--;
    }
    if (ctx.input.Pressed(MoveRight))
    {
        player.direction.x++;
    }
}

void Game::UpdatePlayerHorizontalMovement()
{
    if (ctx.input.Pressed(Sprint))
    {
        player.state.Set(Sprinting);
    }
    else
    {
        player.state.Unset(Sprinting);
    }

    float speed = player.state.Has(Sprinting) ? PLAYER_SPRINT_SPEED : PLAYER_WALK_SPEED;
    player.shape.move({player.direction.x * speed * ctx.time.GetDeltaTime(), 0});
}

void Game::UpdatePlayerClimbing()
{
    if (ctx.input.Pressed(Climb) && IsTileLadder(GetPlayerCurrentTile()))
    {
        player.verticalVelocity = -PLAYER_CLIMB_SPEED;
        player.state.Set(Climbing);
        player.state.Unset(Grounded);
    }
    else
    {
        player.state.Unset(Climbing);
    }
}

void Game::UpdatePlayerGravity()
{
    if (!player.state.Has(Grounded) && !player.state.Has(Climbing))
    {
        player.verticalVelocity += PLAYER_GRAVITY * PLAYER_FALL_SPEED * ctx.time.GetDeltaTime();
    }
}

void Game::UpdatePlayerVerticalMovement()
{
    player.shape.move({0, player.verticalVelocity * ctx.time.GetDeltaTime()});
}

void Game::UpdatePlayerAnimation()
{
    if (player.state.Has(Climbing))
    {
        player.shape.SetRow(0);
        player.shape.Update(ctx.time.GetDeltaTime());
    }
    else if (player.direction.x < 0)
    {
        player.shape.SetRow(1);
        player.shape.Update(ctx.time.GetDeltaTime() * (player.state.Has(Sprinting) ? 2 : 1));
    }
    else if (player.direction.x > 0)
    {
        player.shape.SetRow(3);
        player.shape.Update(ctx.time.GetDeltaTime() * (player.state.Has(Sprinting) ? 2 : 1));
    }
    else
    {
        player.shape.SetRow(2);
        player.shape.Update(0);
    }
}

void Game::UpdateCamera()
{
    sf::Vector2f target = player.shape.getPosition() - sf::Vector2f(0, player.shape.getSize().y);

    if (DistanceSquared(target, camera.view.getCenter()) > 1)
    {
        float t = camera.smoothFactor * ctx.time.GetDeltaTime();
        sf::Vector2f center = Lerp(camera.view.getCenter(), target, t);
        camera.view.setCenter(center);
    }
}

void Game::UpdateEnemies()
{
    for (auto& enemy : enemies)
    {
        if (enemy.knockbackCooldown.IsOver())
        {
            sf::Vector2f direction = player.shape.getPosition() - enemy.shape.getPosition();
            enemy.velocity = direction.normalized() * ENEMY_SPEED;
        }

        enemy.shape.move(enemy.velocity * ctx.time.GetDeltaTime());
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
    sf::Vector2f direction = GetPlayerShootDirection();

    if (direction.lengthSquared() == 0)
    {
        return;
    }

    auto& bullet = bullets.emplace_back();

    bullet.shape.setFillColor(BULLET_COLOR);
    bullet.shape.setRadius(BULLET_RADIUS);
    bullet.shape.setOrigin(bullet.shape.getGeometricCenter());
    bullet.shape.setPosition(player.shape.getPosition());

    bullet.direction = direction.normalized();
    bullet.speed = BULLET_SPEED;

    bullet.alive = true;
}

void Game::EventPlayerForceField()
{
    player.forceField.setPosition(player.shape.getPosition());

    for (auto& enemy : enemies)
    {
        if (Intersects(player.forceField, enemy.shape))
        {
            sf::Vector2f direction = enemy.shape.getPosition() - player.forceField.getPosition();
            EventEnemyKnockback(enemy, direction.normalized() * PLAYER_FORCE_FIELD_FORCE, 2);
        }
    }
}

void Game::EventPlayerJump()
{
    player.verticalVelocity = -PLAYER_JUMP_SPEED;
    player.state.Unset(Grounded);
}

void Game::EventPlayerBonus()
{
    player.score++;
    player.scoreText.setString("Score: " + std::to_string(player.score));
    bonusSound.play();
}

void Game::EventEnemySpawn()
{
    auto& enemy = enemies.emplace_back();

    enemy.shape.setTexture(ctx.resources.FetchTexture(ENEMY_TEXTURE_FILENAME));
    enemy.shape.setFillColor(ENEMY_COLOR);
    enemy.shape.setSize(player.shape.getSize() / 2.f);
    enemy.shape.setOrigin(enemy.shape.getGeometricCenter());
    sf::Vector2f offset(ENEMY_SPAWN_RADIUS, ctx.random.Angle(sf::Angle::Zero, sf::degrees(360)));
    enemy.shape.setPosition(player.shape.getPosition() + offset);

    enemy.lives = ENEMY_LIVES;

    enemy.knockbackCooldown.SetDuration(ENEMY_KNOCKBACK_COOLDOWN_DURATION);
}

void Game::EventEnemyKnockback(Enemy& enemy, sf::Vector2f velocity, int damage)
{
    enemy.velocity = velocity;
    enemy.lives -= damage;
    enemy.knockbackCooldown.Restart();
}

void Game::HandleCollisions()
{
    HandleCollisionsPlayer();
    HandleCollisionsBullets();
    HandleCollisionsEnemies();

    if (player.score == BONUS_GOAL)
    {
        LOG_INFO("You Win!");
        ctx.scenes.RestartCurrentScene();
    }
    else if (player.lives <= 0 || IsOutsideWindowBottom(player.shape))
    {
        LOG_INFO("You Lose!");
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::HandleCollisionsPlayer()
{
    HandleCollisionsPlayerUp();
    HandleCollisionsPlayerDown();
    HandleCollisionsPlayerLeftRight();
    HandleCollisionsPlayerBonus();
    HandleCollisionsPlayerEnemies();
}

void Game::HandleCollisionsPlayerUp()
{
    sf::Vector2f upOffset(0, player.shape.getSize().y / 4);
    sf::Vector2u upTile = map.WorldToTilePosition(player.shape.getPosition() - upOffset);

    if (IsTileSolid(upTile))
    {
        player.verticalVelocity = 0;

        float positionY = map.TileToWorldPosition(upTile).y + map.GetTileSize().y + upOffset.y;
        player.shape.setPosition({player.shape.getPosition().x, positionY});
    }
}

void Game::HandleCollisionsPlayerDown()
{
    if (player.state.Has(Climbing))
    {
        return;
    }

    sf::Vector2f downOffset(0, player.shape.getSize().y / 2 - 1);
    sf::Vector2u downTile = map.WorldToTilePosition(player.shape.getPosition() + downOffset);

    if (IsTileSolid(downTile))
    {
        player.state.Set(Grounded);
        player.verticalVelocity = 0;

        float positionY = map.TileToWorldPosition(downTile).y - downOffset.y;
        player.shape.setPosition({player.shape.getPosition().x, positionY});
    }
    else
    {
        player.state.Unset(Grounded);
    }
}

void Game::HandleCollisionsPlayerLeftRight()
{
    sf::Vector2f horizontalOffset(player.shape.getSize().x / 4, 0);

    sf::Vector2u leftTile = map.WorldToTilePosition(player.shape.getPosition() - horizontalOffset);
    sf::Vector2u rightTile = map.WorldToTilePosition(player.shape.getPosition() + horizontalOffset);

    if ((IsTileSolid(leftTile)  && player.direction.x < 0) ||
        (IsTileSolid(rightTile) && player.direction.x > 0))
    {
        player.shape.setPosition({player.lastPosition.x, player.shape.getPosition().y});
    }
}

void Game::HandleCollisionsPlayerBonus()
{
    sf::Vector2u currentTile = GetPlayerCurrentTile();

    if (IsTileBonus(currentTile))
    {
        EventPlayerBonus();

        map.SetTile(currentTile, TILE_EMPTY);
    }
}

void Game::HandleCollisionsPlayerEnemies()
{
    for (auto& enemy : enemies)
    {
        if (Intersects(player.shape, enemy.shape))
        {
            ResolveCollisionPlayerEnemy(enemy);
        }
    }

    std::erase_if(enemies, [](const Enemy& enemy) { return enemy.lives <= 0; });
}

void Game::ResolveCollisionPlayerEnemy(Enemy& enemy)
{
    player.lives--;
    player.livesText.setString("Lives: " + std::to_string(player.lives));

    enemy.lives = 0;
}

void Game::HandleCollisionsBullets()
{
    for (auto& bullet : bullets)
    {
        HandleCollisionsBulletMap(bullet);
        HandleCollisionsBulletEnemies(bullet);
    }

    std::erase_if(bullets, [](const Bullet& bullet) {
        return !bullet.alive || IsOutsideWindow(bullet.shape.getPosition());
    });
}

void Game::HandleCollisionsBulletMap(Bullet& bullet)
{
    sf::Vector2u currentTile = map.WorldToTilePosition(bullet.shape.getPosition());

    if (IsTileSolid(currentTile))
    {
        bullet.alive = false;
    }
    else if (IsTileBonus(currentTile))
    {
        bullet.alive = false;
        EventPlayerBonus();

        map.SetTile(currentTile, TILE_EMPTY);
    }
}

void Game::HandleCollisionsBulletEnemies(Bullet& bullet)
{
    for (auto& enemy : enemies)
    {
        if (bullet.alive && Intersects(enemy.shape, bullet.shape))
        {
            ResolveCollisionBulletEnemy(bullet, enemy);
        }
    }

    std::erase_if(enemies, [](const Enemy& enemy) { return enemy.lives <= 0; });
}

void Game::ResolveCollisionBulletEnemy(Bullet& bullet, Enemy& enemy)
{
    bullet.alive = false;
    EventEnemyKnockback(enemy, bullet.direction * ENEMY_KNOCKBACK_FORCE, 1);
}

void Game::HandleCollisionsEnemies()
{
    for (std::size_t i = 0; i < enemies.size(); i++)
    {
        for (std::size_t j = i + 1; j < enemies.size(); j++)
        {
            if (Intersects(enemies[i].shape, enemies[j].shape))
            {
                ResolveCollisionEnemyEnemy(enemies[i], enemies[j]);
            }
        }
    }
}

void Game::ResolveCollisionEnemyEnemy(Enemy& left, Enemy& right)
{
    sf::Vector2f direction = right.shape.getPosition() - left.shape.getPosition();

    if (left.knockbackCooldown.IsOver())
    {
        EventEnemyKnockback(left, (-direction).normalized() * ENEMY_KNOCKBACK_FORCE, 0);
    }

    if (right.knockbackCooldown.IsOver())
    {
        EventEnemyKnockback(right, direction.normalized() * ENEMY_KNOCKBACK_FORCE, 0);
    }
}

sf::Vector2u Game::GetPlayerCurrentTile() const
{
    return map.WorldToTilePosition(player.shape.getPosition());
}

sf::Vector2f Game::GetPlayerShootDirection() const
{
    sf::Vector2f joystickDirection(
        sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z) / 100,
        sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R) / 100
    );

    if (joystickDirection.length() > gConfig.joystickDeadzone)
    {
        return joystickDirection;
    }
    else
    {
        return ctx.cursor.GetPosition(camera.view) - player.shape.getPosition();
    }
}

bool Game::IsTileSolid(sf::Vector2u position) const
{
    Tile tile = map.GetTile(position);
    return (tile >= 0 && tile <= 10)  || (tile >= 13 && tile <= 20) || (tile >= 24 && tile <= 27) ||
           (tile >= 31 && tile <= 34) || (tile >= 41 && tile <= 44) || (tile >= 49 && tile <= 50);
}

bool Game::IsTileLadder(sf::Vector2u position) const
{
    Tile tile = map.GetTile(position);
    return tile == 21 || tile == 28 || tile == 35;
}

bool Game::IsTileBonus(sf::Vector2u position) const
{
    Tile tile = map.GetTile(position);
    return (tile >= 54 && tile <= 57) || (tile >= 61 && tile <= 64);
}

bool Game::IsTileEmpty(sf::Vector2u position) const
{
    Tile tile = map.GetTile(position);
    return tile == 12 || (tile >= 65 && tile <= 71) || (tile >= 75 && tile <= 77);
}

void Game::Render() const
{
    ctx.renderer.Draw(background);

    ctx.renderer.SetView(camera.view);
    ctx.renderer.Draw(map);
    ctx.renderer.Draw(player.shape);

    for (const auto& enemy : enemies)
    {
        ctx.renderer.Draw(enemy.shape);
    }

    for (const auto& bullet : bullets)
    {
        ctx.renderer.Draw(bullet.shape);
    }

    if (!player.forceFieldCooldown.IsOver())
    {
        ctx.renderer.Draw(player.forceField);
    }

    ctx.renderer.ResetView();
    ctx.renderer.Draw(player.scoreText);
    ctx.renderer.Draw(player.livesText);
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        enemySpawnCooldown.Stop();
        player.jumpCooldown.Stop();
        player.shootCooldown.Stop();
        player.forceFieldCooldown.Stop();
    }
    else
    {
        enemySpawnCooldown.Start();
        player.jumpCooldown.Start();
        player.shootCooldown.Start();
        player.forceFieldCooldown.Start();
    }
}