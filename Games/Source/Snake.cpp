// Copyright (c) 2025 Adel Hales

#include "Snake.h"

using namespace Snake;

Game::Game(EngineContext& context) :
    Scene(context),
    tilesetTexture(*ctx.resources.FetchTexture(TILEMAP_TEXTURE_FILENAME)),
    bonusSound(*ctx.resources.FetchSound(BONUS_SOUND_FILENAME))
{
    InitMap();
    InitPlayer();
    InitBonus();
}

void Game::InitMap()
{
    map.tileSize = gConfig.windowSize.componentWiseDiv(sf::Vector2f(MAP_SIZE));

    for (int i = 0; i < MAP_SIZE.x; i++)
    {
        for (int j = 0; j < MAP_SIZE.y; j++)
        {
            auto& rect = map.grid.emplace_back();

            InitMapRect(rect, {i, j});
        }
    }
}

void Game::InitMapRect(sf::RectangleShape& rect, sf::Vector2i tile)
{
    rect.setFillColor(MAP_COLOR);
    rect.setOutlineColor(MAP_OUTLINE_COLOR);
    rect.setOutlineThickness(-1);
    rect.setSize(map.tileSize);
    rect.setPosition(sf::Vector2f(tile).componentWiseMul(map.tileSize));
}

void Game::InitPlayer()
{
    player.head.shape.setFillColor(PLAYER_COLOR);
    player.head.shape.setSize(gConfig.windowSize.componentWiseDiv(sf::Vector2f(MAP_SIZE)));
    player.head.shape.setOrigin(player.head.shape.getGeometricCenter());

    player.stats.scoreText.setFillColor(STATS_SCORE_TEXT_COLOR);
    player.stats.scoreText.setOutlineThickness(2);
    player.stats.scoreText.setPosition({10, 10});

    player.moveCooldown.SetDuration(PLAYER_MOVE_COOLDOWN_DURATION);

    InitPlayerTextures();
}

void Game::InitPlayerTextures()
{
    sf::Vector2i size(64, 64);

    player.texturesRect.emplace("Head Up",    sf::IntRect({3 * 64, 0},      size));
    player.texturesRect.emplace("Head Down",  sf::IntRect({4 * 64, 1 * 64}, size));
    player.texturesRect.emplace("Head Left",  sf::IntRect({3 * 64, 1 * 64}, size));
    player.texturesRect.emplace("Head Right", sf::IntRect({4 * 64, 0},      size));

    player.texturesRect.emplace("Body Vertical",   sf::IntRect({2 * 64, 1 * 64}, size));
    player.texturesRect.emplace("Body Horizontal", sf::IntRect({1 * 64, 0 * 64}, size));

    player.texturesRect.emplace("Corner Up Right / Left Down", sf::IntRect({0 * 64, 0 * 64}, size));
    player.texturesRect.emplace("Corner Down Right / Left Up", sf::IntRect({0 * 64, 1 * 64}, size));
    player.texturesRect.emplace("Corner Up Left / Right Down", sf::IntRect({2 * 64, 0 * 64}, size));
    player.texturesRect.emplace("Corner Down Left / Right Up", sf::IntRect({2 * 64, 2 * 64}, size));

    player.texturesRect.emplace("Tail Up",    sf::IntRect({3 * 64, 2 * 64}, size));
    player.texturesRect.emplace("Tail Down",  sf::IntRect({4 * 64, 3 * 64}, size));
    player.texturesRect.emplace("Tail Left",  sf::IntRect({3 * 64, 3 * 64}, size));
    player.texturesRect.emplace("Tail Right", sf::IntRect({4 * 64, 2 * 64}, size));

    player.texturesRect.emplace("Apple", sf::IntRect({0 * 64, 3 * 64}, size));

    player.head.shape.setTexture(&tilesetTexture);
}

void Game::InitBonus()
{
    bonus.shape.setTexture(&tilesetTexture);
    bonus.shape.setTextureRect(player.texturesRect.at("Apple"));
    bonus.shape.setFillColor(BONUS_COLOR);
    bonus.shape.setSize(player.head.shape.getSize() * 0.8f);
    bonus.shape.setOrigin(bonus.shape.getGeometricCenter());

    bonusSound.setVolume(BONUS_SOUND_VOLUME);
}

void Game::Start()
{
    ctx.cursor.SetVisible(false);
    
    BindInputs();

    StartPlayer();

    EventBonusSpawn();
}

void Game::BindInputs()
{
    ctx.input.Bind(MoveLeft, Input::Keyboard{sf::Keyboard::Scan::Left});
    ctx.input.Bind(MoveLeft, Input::Keyboard{sf::Keyboard::Scan::A});
    ctx.input.Bind(MoveLeft, Input::Axis{sf::Joystick::Axis::X, -0.5f});
    ctx.input.Bind(MoveLeft, Input::Axis{sf::Joystick::Axis::PovX, -0.5f});

    ctx.input.Bind(MoveRight, Input::Keyboard{sf::Keyboard::Scan::Right});
    ctx.input.Bind(MoveRight, Input::Keyboard{sf::Keyboard::Scan::D});
    ctx.input.Bind(MoveRight, Input::Axis{sf::Joystick::Axis::X, 0.5f});
    ctx.input.Bind(MoveRight, Input::Axis{sf::Joystick::Axis::PovX, 0.5f});

    ctx.input.Bind(MoveUp, Input::Keyboard{sf::Keyboard::Scan::Up});
    ctx.input.Bind(MoveUp, Input::Keyboard{sf::Keyboard::Scan::W});
    ctx.input.Bind(MoveUp, Input::Axis{sf::Joystick::Axis::Y, -0.5f});
    ctx.input.Bind(MoveUp, Input::Axis{sf::Joystick::Axis::PovY, 0.5f});

    ctx.input.Bind(MoveDown, Input::Keyboard{sf::Keyboard::Scan::Down});
    ctx.input.Bind(MoveDown, Input::Keyboard{sf::Keyboard::Scan::S});
    ctx.input.Bind(MoveDown, Input::Axis{sf::Joystick::Axis::Y, 0.5f});
    ctx.input.Bind(MoveDown, Input::Axis{sf::Joystick::Axis::PovY, -0.5f});
}

void Game::StartPlayer()
{
    player.head.shape.setPosition(player.head.shape.getSize() / 2.f);
    player.head.nextPosition = player.head.shape.getPosition();
    player.head.direction = player.nextDirection = Direction::Right;

    player.body.clear();
    player.corners.clear();

    for (int i = 0; i < std::max(2, PLAYER_BODY_SIZE_DEFAULT); i++)
    {
        EventPlayerMovement();
        EventPlayerGrow();
    }

    EventPlayerMovement();

    player.moveCooldown.Restart();

    player.stats.score = 0;
    player.stats.scoreText.setString("Score: 0");
}

void Game::Update()
{
    UpdatePlayerDirection();

    if (player.moveCooldown.IsOver())
    {
        EventPlayerMovement();
        player.moveCooldown.Restart();

        HandleCollisions();
    }

    UpdatePlayerSmoothMovement();
}

void Game::UpdatePlayerDirection()
{
    if (ctx.input.Pressed(MoveUp) && player.head.direction != Direction::Down && player.head.direction != Direction::Up)
    {
        player.nextDirection = Direction::Up;
    }
    if (ctx.input.Pressed(MoveDown) && player.head.direction != Direction::Up && player.head.direction != Direction::Down)
    {
        player.nextDirection = Direction::Down;
    }
    if (ctx.input.Pressed(MoveLeft) && player.head.direction != Direction::Right && player.head.direction != Direction::Left)
    {
        player.nextDirection = Direction::Left;
    }
    if (ctx.input.Pressed(MoveRight) && player.head.direction != Direction::Left && player.head.direction != Direction::Right)
    {
        player.nextDirection = Direction::Right;
    }
}

void Game::UpdatePlayerSmoothMovement()
{
    float progress = player.moveCooldown.GetElapsedTime() / player.moveCooldown.GetDuration();

    player.head.shape.setPosition(Lerp(player.head.lastPosition, player.head.nextPosition, progress));

    for (auto& part : player.body)
    {
        part.shape.setPosition(Lerp(part.lastPosition, part.nextPosition, progress));
    }
}

void Game::EventPlayerMovement()
{
    if (!player.body.empty())
    {
        EventPlayerBodyMovement();
        EventPlayerBodySecondMovement();
    }

    EventPlayerHeadMovement();
}

void Game::EventPlayerBodyMovement()
{
    int last = (int)player.body.size() - 1;

    for (int i = last; i > 0; i--)
    {
        auto& current = player.body[i];
        const auto& next = player.body[i - 1];

        EventPlayerBodyPartMovement(current, next);

        if (i == last)
        {
            EventPlayerTailChangeTexture(current);
        }
        else
        {
            EventPlayerBodyChangeTexture(current);
        }
    }

    std::erase_if(player.corners, [&](const Corner& corner) { 
        return corner.shape.getPosition() == player.body.back().shape.getPosition();
    });
}

void Game::EventPlayerBodyPartMovement(Part& current, const Part& next)
{
    current.shape.setPosition(current.nextPosition);
    
    current.lastPosition = current.nextPosition;
    current.nextPosition = next.nextPosition;
    current.direction = next.direction;
}

void Game::EventPlayerBodySecondMovement()
{
    auto& first = player.body.front();

    EventPlayerBodyPartMovement(first, player.head);
    EventPlayerBodyChangeTexture(first);
}

void Game::EventPlayerHeadMovement()
{
    player.head.shape.setPosition(player.head.nextPosition);

    if (player.head.direction != player.nextDirection)
    {
        EventCornerSpawn();
    }

    player.head.lastPosition = player.head.nextPosition;
    player.head.nextPosition = GetPlayerHeadNextPosition();
    player.head.direction = player.nextDirection;

    EventPlayerHeadChangeTexture(player.head);
}

void Game::EventPlayerTailMovement(Part& tail, const Part& next)
{
    tail.shape.setPosition(next.lastPosition);
    tail.lastPosition = next.lastPosition;
    tail.nextPosition = next.shape.getPosition();
    tail.direction = next.direction;
}

void Game::EventPlayerGrow()
{
    auto& tail = player.body.emplace_back();

    tail.shape.setTexture(&tilesetTexture);
    tail.shape.setFillColor(PLAYER_COLOR);
    tail.shape.setSize(player.head.shape.getSize());
    tail.shape.setOrigin(tail.shape.getGeometricCenter());

    if (player.body.size() == 1)
    {
        EventPlayerTailMovement(tail, player.head);
    }
    else
    {
        auto& next = player.body[player.body.size() - 2];

        EventPlayerTailMovement(tail, next);
        EventPlayerBodyChangeTexture(next);
    }

    EventPlayerTailChangeTexture(tail);
}

void Game::EventCornerSpawn()
{
    auto& corner = player.corners.emplace_back();

    corner.shape.setTexture(&tilesetTexture);
    corner.shape.setTextureRect(player.texturesRect.at(GetCornerTextureKey()));
    corner.shape.setFillColor(PLAYER_COLOR);
    corner.shape.setSize(player.head.shape.getSize());
    corner.shape.setOrigin(corner.shape.getGeometricCenter());
    corner.shape.setPosition(player.head.shape.getPosition());

    corner.background.setFillColor(MAP_COLOR);
    corner.background.setOutlineThickness(-1);
    corner.background.setOutlineColor(sf::Color::Black);
    corner.background.setSize(corner.shape.getSize());
    corner.background.setOrigin(corner.background.getGeometricCenter());
    corner.background.setPosition(corner.shape.getPosition());
}

void Game::EventPlayerHeadChangeTexture(Part& head)
{
    switch (head.direction)
    {
        case Direction::Up:    head.shape.setTextureRect(player.texturesRect.at("Head Up"));    break;
        case Direction::Down:  head.shape.setTextureRect(player.texturesRect.at("Head Down"));  break;
        case Direction::Left:  head.shape.setTextureRect(player.texturesRect.at("Head Left"));  break;
        case Direction::Right: head.shape.setTextureRect(player.texturesRect.at("Head Right")); break;
        default: break;
    }
}

void Game::EventPlayerBodyChangeTexture(Part& body)
{
    switch (body.direction)
    {
        case Direction::Up:
        case Direction::Down:  body.shape.setTextureRect(player.texturesRect.at("Body Vertical"));   break;
        case Direction::Left:
        case Direction::Right: body.shape.setTextureRect(player.texturesRect.at("Body Horizontal")); break;
        default: break;
    }
}

void Game::EventPlayerTailChangeTexture(Part& tail)
{
    switch (tail.direction)
    {
        case Direction::Up:    tail.shape.setTextureRect(player.texturesRect.at("Tail Up"));    break;
        case Direction::Down:  tail.shape.setTextureRect(player.texturesRect.at("Tail Down"));  break;
        case Direction::Left:  tail.shape.setTextureRect(player.texturesRect.at("Tail Left"));  break;
        case Direction::Right: tail.shape.setTextureRect(player.texturesRect.at("Tail Right")); break;
        default: break;
    }
}

void Game::EventBonusSpawn()
{
    do {
        sf::Vector2f size = player.head.shape.getSize();
        bonus.shape.setPosition({ctx.random.Int(0, MAP_SIZE.x - 1) * size.x + size.x / 2,
                                 ctx.random.Int(0, MAP_SIZE.y - 1) * size.y + size.y / 2});
    } while (Intersects(player.head.shape, bonus.shape) ||
        std::ranges::any_of(player.body, [&](const Part& part) { 
            return (bool)Intersects(part.shape, bonus.shape); 
        })
    );
}

void Game::HandleCollisions()
{
    HandleCollisionsPlayerMap();
    HandleCollisionsPlayerBody();
    HandleCollisionsPlayerBonus();

    if (player.stats.score == STATS_SCORE_GOAL)
    {
        LOG_INFO("You Win!");
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::HandleCollisionsPlayerMap()
{
    if (IsOutsideWindow(player.head.shape.getPosition()))
    {
        LOG_INFO("You Lose!");
        ctx.scenes.RestartCurrentScene();
    }
}

void Game::HandleCollisionsPlayerBody()
{
    for (const auto& part : player.body)
    {
        if (player.head.shape.getPosition() == part.shape.getPosition())
        {
            LOG_INFO("You Lose!");
            ctx.scenes.RestartCurrentScene();
            return;
        }
    }
}

void Game::HandleCollisionsPlayerBonus()
{
    if (Intersects(player.head.shape, bonus.shape))
    {
        ResolveCollisionPlayerBonus();
    }
}

void Game::ResolveCollisionPlayerBonus()
{
    EventPlayerGrow();
    EventBonusSpawn();

    player.stats.score++;
    player.stats.scoreText.setString("Score: " + std::to_string(player.stats.score));

    bonusSound.play();
}

sf::Vector2f Game::GetPlayerHeadNextPosition() const
{
    switch (player.nextDirection)
    {
        case Direction::Up:   return player.head.nextPosition - sf::Vector2f(0, player.head.shape.getSize().y);
        case Direction::Down: return player.head.nextPosition + sf::Vector2f(0, player.head.shape.getSize().y);
        case Direction::Left: return player.head.nextPosition - sf::Vector2f(player.head.shape.getSize().x, 0);
        default:              return player.head.nextPosition + sf::Vector2f(player.head.shape.getSize().x, 0);
    }
}

std::string Game::GetCornerTextureKey() const
{
    Direction from = player.head.direction;
    Direction to = player.nextDirection;

    if ((from == Direction::Up && to == Direction::Right) || (from == Direction::Left && to == Direction::Down))
    {
        return "Corner Up Right / Left Down";
    }
    else if ((from == Direction::Down && to == Direction::Right) || (from == Direction::Left && to == Direction::Up))
    {
        return "Corner Down Right / Left Up";
    }
    else if ((from == Direction::Up && to == Direction::Left) || (from == Direction::Right && to == Direction::Down))
    {
        return "Corner Up Left / Right Down";
    }
    else
    {
        return "Corner Down Left / Right Up";
    }
}

void Game::Render() const
{
    for (const auto& rect : map.grid)
    {
        ctx.renderer.Draw(rect);
    }

    ctx.renderer.Draw(bonus.shape);

    for (const auto& part : player.body)
    {
        ctx.renderer.Draw(part.shape);
    }

    for (const auto& corner : player.corners)
    {
        ctx.renderer.Draw(corner.background);
        ctx.renderer.Draw(corner.shape);
    }

    ctx.renderer.Draw(player.head.shape);

    ctx.renderer.Draw(player.stats.scoreText);
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        player.moveCooldown.Stop();
        bonusSound.stop();
    }
    else
    {
        player.moveCooldown.Start();
    }
}

void Game::OnCleanup()
{
    bonusSound.stop();
}