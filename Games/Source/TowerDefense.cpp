// Copyright (c) 2025 Adel Hales

#include "TowerDefense.h"

using namespace TowerDefense;

Game::Game(EngineContext& context) :
    Scene(context),
    towerSpawnCooldown(TOWER_SPAWN_COOLDOWN_DURATION),
    waveSpawnCooldown(WAVE_SPAWN_COOLDOWN_DURATION)
{
    InitMap();
    InitPath();
    InitCastle();
}

void Game::InitMap()
{
    map.size = MAP_SIZE;
    map.tileSize = gConfig.windowSize.componentWiseDiv(sf::Vector2f(map.size));

    for (int i = 0; i < map.size.x; i++)
    {
        for (int j = 0; j < map.size.y; j++)
        {
            auto& cell = map.grid.emplace_back();

            InitMapRect(cell, {i, j});
        }
    }
}

void Game::InitMapRect(sf::RectangleShape& rect, sf::Vector2i tile)
{
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(-1);
    rect.setSize(map.tileSize);
    rect.setPosition(sf::Vector2f(tile).componentWiseMul(map.tileSize));
}

void Game::InitPath()
{
    enum Direction
    {
        Up, Down, Left, Right
    };

    struct DirectionRepeat
    {
        Direction direction;
        int repeat;
    };

    const std::vector<DirectionRepeat> ways = {
        {Right, 4}, {Up, 6}, {Right, 6}, {Down, 6},  {Left, 3},
        {Down, 4},  {Right, 6}, {Up, 4}, {Right, 2}, {Up, 2}
    };

    auto& start = map.path.emplace_back();

    InitPathRect(start);

    for (const auto& way : ways)
    {
        sf::Vector2f direction = 
            (way.direction == Right) ? sf::Vector2f(map.tileSize.x, 0) :
            (way.direction == Up)    ? sf::Vector2f(0, -map.tileSize.y) :
            (way.direction == Left)  ? sf::Vector2f(-map.tileSize.x, 0) :
            (way.direction == Down)  ? sf::Vector2f(0, map.tileSize.y) :
            sf::Vector2f();

        for (int i = 0; i < way.repeat; i++)
        {
            auto& rect = map.path.emplace_back(map.path.back());

            rect.move(direction);
        }
    }
}

void Game::InitPathRect(sf::RectangleShape& rect)
{
    rect.setFillColor(PATH_COLOR);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(-1);
    rect.setSize(map.tileSize);
    rect.setPosition({0, gConfig.windowSize.y / 2});
}

void Game::InitCastle()
{
    castle.shape.setTexture(ctx.resources.FetchTexture(CASTLE_TEXTURE_FILENAME));
    castle.shape.setFillColor(CASTLE_COLOR);
    castle.shape.setSize(map.tileSize * 5.f);
    castle.shape.setOrigin(castle.shape.getGeometricCenter());

    sf::Vector2f offset(map.tileSize.x / 2, map.tileSize.y * -2.5f);
    castle.shape.setPosition(map.path.back().getPosition() + offset);
}

void Game::Start()
{
    towers.clear();
    enemies.clear();
    bullets.clear();
    wave.enemies.clear();

    StartStats();
    StartCastle();
    StartPreview();
    StartGui();

    towerSpawnCooldown.Restart();
    waveSpawnCooldown.Restart();

    enemySpawnCooldown.SetDuration(ENEMY_SPAWN_COOLDOWN_DURATION);
    enemySpawnCooldown.Restart();
}

void Game::StartStats()
{
    stats.money = STATS_BASE_MONEY;
    stats.level = STATS_BASE_LEVEL;
}

void Game::StartCastle()
{
    castle.health.maxPoints = castle.health.points = CASTLE_HEALTH;
}

void Game::StartPreview()
{
    preview.enabled = false;
}

void Game::StartGui()
{
    ui.statsLabel = tgui::Label::create();
    auto statsPanel = tgui::Panel::create();
    statsPanel->add(ui.statsLabel);

    ui.waveLabel = tgui::Label::create();
    auto wavePanel = tgui::Panel::create();
    wavePanel->add(ui.waveLabel);

    ui.layout = tgui::VerticalLayout::create();
    ui.layout->add(statsPanel);
    ui.layout->add(wavePanel);

    ui.waveBar  = GenerateProgressBar("Wave");
    ui.towerBar = GenerateProgressBar("Tower");
    ui.enemyBar = GenerateProgressBar("Enemy");

    ui.container = tgui::Panel::create({"28%", "39%"});
    ui.container->setPosition("0%", "61%");
    ui.container->add(ui.layout);

    ctx.gui.Add(ui.container);
}

void Game::OnEvent(const sf::Event& event)
{
    event.visit([this](const auto& type) { this->HandleEvent(type); });
}

void Game::HandleEvent(const sf::Event::KeyPressed& key)
{
    if (key.scancode == sf::Keyboard::Scan::P)
    {
        preview.enabled = !preview.enabled;
    }
}

void Game::HandleEvent(const sf::Event::MouseButtonPressed&)
{
    if (towerSpawnCooldown.IsOver())
    {
        EventTowerPlacement();
    }
}

void Game::HandleEvent(const sf::Event::JoystickButtonPressed& joystick)
{
    if (auto button = Input::HardwareToLogical(joystick.button, joystick.joystickId))
    {
        if (*button == GamepadButton::South)
        {
            if (towerSpawnCooldown.IsOver())
            {
                EventTowerPlacement();
            }
        }
        else if (*button == GamepadButton::North)
        {
            preview.enabled = !preview.enabled;
        }
    }
}

void Game::Update()
{
    if (enemies.empty() && wave.enemies.empty())
    {
        EventWaveNew();
        waveSpawnCooldown.Restart();
    }

    if (!wave.enemies.empty() && waveSpawnCooldown.IsOver() && enemySpawnCooldown.IsOver())
    {
        EventEnemySpawn();
        enemySpawnCooldown.Restart();
    }

    if (preview.enabled)
    {
        UpdatePreview();
    }

    UpdateEnemies();
    UpdateTowers();
    UpdateBullets();
    UpdateCastle();
    UpdateUI();

    HandleCollisions();
}

void Game::UpdatePreview()
{
    sf::Vector2f position = WorldToTile(ctx.cursor.GetPosition());

    preview.shape.setFillColor(TOWER_COLOR);
    preview.shape.setOutlineColor(sf::Color::Black);
    preview.shape.setOutlineThickness(-1);
    preview.shape.setRadius(std::min(map.tileSize.x / 2, map.tileSize.y / 2));
    preview.shape.setPointCount(3);
    preview.shape.setPosition(position);
    preview.shape.setRotation(sf::Angle::Zero);

    preview.area.setFillColor(TOWER_AREA_COLOR);
    preview.area.setOutlineColor(sf::Color::Black);
    preview.area.setOutlineThickness(-1);
    preview.area.setRadius(std::min(map.tileSize.x * 4, map.tileSize.y * 4));
    preview.area.setPosition(position);

    if (!IsTileValid(position))
    {
        preview.shape.setFillColor(PREVIEW_INVALID_COLOR);
        preview.area.setFillColor(PREVIEW_INVALID_AREA_COLOR);
    }
    else if (auto tower = TowerFind(position))
    {
        preview.shape.setRotation(tower->shape.getRotation());

        if (tower->level < TOWER_MAX_LEVEL)
        {
            preview.shape.setFillColor(PREVIEW_UPGRADE_COLOR);
            preview.area.setRadius(tower->area.getRadius() * 1.1f);
            preview.shape.setPointCount(tower->shape.getPointCount());
        }
        else
        {
            preview.shape.setFillColor(PREVIEW_INVALID_COLOR);
            preview.area.setRadius(tower->area.getRadius());
            preview.shape.setPointCount(tower->shape.getPointCount());
        }
    }

    preview.shape.setOrigin(preview.shape.getGeometricCenter());
    preview.area.setOrigin(preview.area.getGeometricCenter());
}

void Game::UpdateEnemies()
{
    for (auto& enemy : enemies)
    {
        if (enemy.pathIndex < (int)map.path.size())
        {
            UpdateEnemyDirection(enemy);
        }

        enemy.shape.move(enemy.direction * enemy.speed * ctx.time.GetDeltaTime());

        UpdateHealth(enemy.health, enemy.shape);
    }
}

void Game::UpdateEnemyDirection(Enemy& enemy)
{
    sf::Vector2f target = map.path[enemy.pathIndex].getPosition() + map.tileSize / 2.f;
    sf::Vector2f direction = target - enemy.shape.getPosition();

    if (direction.length() > 1)
    {
        enemy.direction = direction.normalized();
        enemy.shape.setRotation(enemy.direction.angle());
    }
    else
    {
        enemy.pathIndex++;
    }
}

void Game::UpdateTowers()
{
    for (auto& tower : towers)
    {
        if (tower.shootCooldown.IsOver())
        {
            EventTowerShoot(tower);
            tower.shootCooldown.Restart();
        }
    }
}

void Game::UpdateBullets()
{
    for (auto& bullet : bullets)
    {
        bullet.shape.move(bullet.direction * bullet.speed * ctx.time.GetDeltaTime());
    }
}

void Game::UpdateCastle()
{
    UpdateHealth(castle.health, castle.shape);
}

void Game::UpdateHealth(Health& health, const sf::Shape& shape)
{
    health.barBackground.setFillColor(HEALTH_BACKGROUND_COLOR);

    sf::Vector2f size = shape.getGlobalBounds().size;
    health.barBackground.setSize({size.x, HEALTH_BAR_HEIGHT});

    sf::Vector2f offset(size.x / 2, size.y / 2 + map.tileSize.y / 2);
    health.barBackground.setPosition(shape.getPosition() - offset);

    health.bar.setFillColor(HEALTH_FOREGROUND_COLOR);
    health.bar.setSize({size.x * (health.points / (float)health.maxPoints), HEALTH_BAR_HEIGHT});
    health.bar.setPosition(health.barBackground.getPosition());
}

void Game::UpdateUI()
{
    ui.enemyBar->setValue(int(enemySpawnCooldown.GetElapsedTime() / enemySpawnCooldown.GetDuration() * 100));
    ui.towerBar->setValue(int(towerSpawnCooldown.GetElapsedTime() / towerSpawnCooldown.GetDuration() * 100));
    ui.waveBar->setValue(int(waveSpawnCooldown.GetElapsedTime() / waveSpawnCooldown.GetDuration() * 100));

    ui.waveLabel->setText(std::format("Wave elapsed time (s): {}\nNext Enemy (level): {}\nEnemies Left: {}",
        int(waveSpawnCooldown.GetElapsedTime()),
        wave.enemies.empty() ? "None" : std::to_string(wave.enemies.back()),
        wave.enemies.size()
    ));

    ui.statsLabel->setText("Money: " + std::to_string(stats.money) + '\n' + "Level: " + std::to_string(stats.level));
}

void Game::EventTowerPlacement()
{
    sf::Vector2f position = WorldToTile(ctx.cursor.GetPosition());

    if (IsTileValid(position))
    {
        if (auto tower = TowerFind(position))
        {
            if (stats.money >= TOWER_UPGRADE_COST && tower->level < TOWER_MAX_LEVEL)
            {
                stats.money -= TOWER_UPGRADE_COST;
                EventTowerUpgrade(*tower);
                towerSpawnCooldown.Restart();
            }
        }
        else if (stats.money >= TOWER_PLACE_COST)
        {
            stats.money -= TOWER_PLACE_COST;
            EventTowerPlace(position);
            towerSpawnCooldown.Restart();
        }
    }
}

void Game::EventTowerUpgrade(Tower& tower)
{
    tower.level++;

    tower.area.setRadius(tower.area.getRadius() * 1.1f);
    tower.area.setOrigin(tower.area.getGeometricCenter());
    tower.shape.setPointCount(tower.shape.getPointCount() + 1);
}

void Game::EventTowerPlace(sf::Vector2f position)
{
    auto& tower = towers.emplace_back();

    tower.shape.setFillColor(TOWER_COLOR);
    tower.shape.setOutlineColor(sf::Color::Black);
    tower.shape.setOutlineThickness(-1);
    tower.shape.setRadius(std::min(map.tileSize.x / 2, map.tileSize.y / 2));
    tower.shape.setOrigin(tower.shape.getGeometricCenter());
    tower.shape.setPosition(position);
    tower.shape.setPointCount(3);

    tower.area.setFillColor(TOWER_AREA_COLOR);
    tower.area.setOutlineColor(sf::Color::Black);
    tower.area.setOutlineThickness(-1);
    tower.area.setRadius(std::min(map.tileSize.x * 4, map.tileSize.y * 4));
    tower.area.setOrigin(tower.area.getGeometricCenter());
    tower.area.setPosition(position);

    tower.level = 1;

    tower.shootCooldown.SetDuration(TOWER_SHOOT_COOLDOWN_DURATION);
}

void Game::EventTowerShoot(Tower& tower)
{
    auto enemyIt = std::ranges::find_if(enemies, [&](const Enemy& enemy) { 
        return (bool)Intersects(tower.area, enemy.shape); 
    });

    if (enemyIt != enemies.end())
    {
        EventBulletSpawn(tower, *enemyIt);
    }
}

void Game::EventBulletSpawn(Tower& tower, const Enemy& target)
{
    auto& bullet = bullets.emplace_back();

    bullet.shape.setFillColor(BULLET_COLOR);
    bullet.shape.setOutlineColor(sf::Color::Black);
    bullet.shape.setOutlineThickness(-1);
    bullet.shape.setRadius(std::min(map.tileSize.x / 4.f, map.tileSize.y / 4.f));
    bullet.shape.setOrigin(bullet.shape.getGeometricCenter());
    bullet.shape.setPosition(tower.shape.getPosition());

    sf::Vector2f direction = target.shape.getPosition() - bullet.shape.getPosition();
    bullet.direction = direction.normalized();
    bullet.speed = BULLET_SPEED + BULLET_SPEED / 10 * tower.level;

    tower.shape.setRotation(bullet.direction.angle());

    bullet.damage = BULLET_DAMAGE * tower.level;
    bullet.alive = true;
}

void Game::EventWaveNew()
{
    stats.level++;
    stats.money += WAVE_MONEY_BONUS;

    wave.enemies.resize(stats.level);
    std::ranges::generate(wave.enemies, [&] { return ctx.random.Int(1, 3); });

    enemySpawnCooldown.SetDuration(std::max(enemySpawnCooldown.GetDuration() * 0.9f, 0.25f));
}

void Game::EventEnemySpawn()
{
    auto& enemy = enemies.emplace_back();

    enemy.level = wave.enemies.back();
    wave.enemies.pop_back();

    enemy.health.maxPoints = enemy.health.points = int(ENEMY_HEALTH * std::sqrt(enemy.level));
    enemy.speed = ENEMY_SPEED * (float)std::sqrt(enemy.level);
    enemy.damage = ENEMY_DAMAGE;
    enemy.pathIndex = 0;

    enemy.shape.setTexture(ctx.resources.FetchTexture(ENEMY_TEXTURE_FILENAME));
    enemy.shape.setFillColor(ENEMY_COLORS[enemy.level - 1]);
    enemy.shape.setOutlineColor(sf::Color::Black);
    enemy.shape.setOutlineThickness(-1);
    enemy.shape.setSize(map.tileSize);
    enemy.shape.setOrigin(enemy.shape.getGeometricCenter());
    enemy.shape.setPosition(map.path.front().getPosition() + map.tileSize / 2.f);
}

void Game::HandleCollisions()
{
    HandleCollisionsBullets();
    HandleCollisionsEnemies();

    if (castle.health.points <= 0)
    {
        LOG_INFO("You Lose!");
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::HandleCollisionsBullets()
{
    for (auto& bullet : bullets)
    {
        HandleCollisionsBulletEnemies(bullet);
    }

    std::erase_if(bullets, [](const Bullet& bullet) { return !bullet.alive || IsOutsideWindow(bullet.shape.getPosition()); });
}

void Game::HandleCollisionsBulletEnemies(Bullet& bullet)
{
    for (auto& enemy : enemies)
    {
        if (bullet.alive && enemy.health.points > 0 && Intersects(bullet.shape, enemy.shape))
        {
            ResolveCollisionBulletEnemy(bullet, enemy);
        }
    }
}

void Game::ResolveCollisionBulletEnemy(Bullet& bullet, Enemy& enemy)
{
    bullet.alive = false;
    enemy.health.points -= bullet.damage;

    if (enemy.health.points <= 0)
    {
        stats.money += enemy.level;
    }
}

void Game::HandleCollisionsEnemies()
{
    for (auto& enemy : enemies)
    {
        if (enemy.health.points > 0 && Intersects(enemy.shape, castle.shape))
        {
            ResolveCollisionEnemyCastle(enemy);
        }
    }

    std::erase_if(enemies, [](const Enemy& enemy) { return enemy.health.points <= 0; });
}

void Game::ResolveCollisionEnemyCastle(Enemy& enemy)
{
    castle.health.points -= enemy.damage;
    enemy.health.points = 0;
}

tgui::ProgressBar::Ptr Game::GenerateProgressBar(const tgui::String& title)
{
    auto label = tgui::Label::create(title);

    auto bar = tgui::ProgressBar::create();
    bar->setSize("100%", "50%");
    bar->setPosition("0%", "50%");
    bar->setMaximum(100);

    auto panel = tgui::Panel::create();
    panel->add(label);
    panel->add(bar);

    ui.layout->add(panel);

    return bar;
}

Tower* Game::TowerFind(sf::Vector2f position)
{
    for (auto& tower : towers)
    {
        if (Contains(tower.shape, position))
        {
            return &tower;
        }
    }
    
    return nullptr;
}

sf::Vector2f Game::WorldToTile(sf::Vector2f position) const
{
    return {std::floor(position.x / map.tileSize.x) * map.tileSize.x + map.tileSize.x / 2,
            std::floor(position.y / map.tileSize.y) * map.tileSize.y + map.tileSize.y / 2};
}

bool Game::IsTileValid(sf::Vector2f position) const
{
    return !IsOutsideWindow(position) && !Contains(castle.shape, position) && 
           !ui.container->isMouseOnWidget(ctx.cursor.GetPosition()) &&
           std::ranges::none_of(map.path, [&](const auto& rect) { return Contains(rect, position); });
}

void Game::Render() const
{
    for (const auto& rect : map.grid)
    {
        ctx.renderer.Draw(rect);
    }

    for (const auto& rect : map.path)
    {
        ctx.renderer.Draw(rect);
    }

    for (const auto& tower : towers)
    {
        ctx.renderer.Draw(tower.shape);
    }

    for (const auto& bullet : bullets)
    {
        ctx.renderer.Draw(bullet.shape);
    }

    for (const auto& enemy : enemies)
    {
        ctx.renderer.Draw(enemy.shape);
        ctx.renderer.Draw(enemy.health.barBackground);
        ctx.renderer.Draw(enemy.health.bar);
    }

    ctx.renderer.Draw(castle.shape);
    ctx.renderer.Draw(castle.health.barBackground);
    ctx.renderer.Draw(castle.health.bar);

    if (preview.enabled)
    {
        ctx.renderer.Draw(preview.area);
        ctx.renderer.Draw(preview.shape);
    }
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        for (auto& tower : towers)
        {
            tower.shootCooldown.Stop();
        }

        waveSpawnCooldown.Stop();
        enemySpawnCooldown.Stop();
        towerSpawnCooldown.Stop();
    }
    else
    {
        for (auto& tower : towers)
        {
            tower.shootCooldown.Start();
        }

        waveSpawnCooldown.Start();
        enemySpawnCooldown.Start();
        towerSpawnCooldown.Start();
    }
}

void Game::OnCleanup()
{
    ctx.gui.Remove(ui.container);
}