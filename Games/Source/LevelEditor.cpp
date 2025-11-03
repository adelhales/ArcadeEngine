// Copyright (c) 2025 Adel Hales

#include "LevelEditor.h"

using namespace LevelEditor;

Game::Game(EngineContext& context) :
    Scene(context),
    infos(GetDefaultFont()),
    placeCooldown(PLACE_COOLDOWN_DURATION)
{
    InitInfos();
    InitSelected();
    InitBackground();
}

void Game::InitInfos()
{
    infos.setFillColor(INFOS_COLOR);
    infos.setOutlineThickness(1);
    infos.setCharacterSize(INFOS_TEXT_SIZE);
    infos.setPosition({5, 5});
}

void Game::InitSelected()
{
    selected.text = infos;
}

void Game::InitBackground()
{
    background.setTexture(ctx.resources.FetchTexture(BACKGROUND_TEXTURE_FILENAME));
    background.setFillColor(BACKGROUND_COLOR);
    background.setSize(gConfig.windowSize);
}

void Game::Start()
{
    BindInputs();
    
    StartCamera();
    StartMap();
    StartInfos();
    StartSelected();

    showTileset = false;

    placeCooldown.Restart();
}

void Game::BindInputs()
{
    ctx.input.Bind(MoveLeft, Input::Keyboard{sf::Keyboard::Scan::A});
    ctx.input.Bind(MoveLeft, Input::Axis{sf::Joystick::Axis::Z, -0.5f});

    ctx.input.Bind(MoveRight, Input::Keyboard{sf::Keyboard::Scan::D});
    ctx.input.Bind(MoveRight, Input::Axis{sf::Joystick::Axis::Z, 0.5f});

    ctx.input.Bind(MoveUp, Input::Keyboard{sf::Keyboard::Scan::W});
    ctx.input.Bind(MoveUp, Input::Axis{sf::Joystick::Axis::R, -0.5f});

    ctx.input.Bind(MoveDown, Input::Keyboard{sf::Keyboard::Scan::S});
    ctx.input.Bind(MoveDown, Input::Axis{sf::Joystick::Axis::R, 0.5f});

    ctx.input.Bind(Click, Input::Mouse{sf::Mouse::Button::Left});
    ctx.input.Bind(Click, Input::Gamepad{GamepadButton::South});
}

void Game::StartCamera()
{
    camera.view.setSize(gConfig.windowSize);
    camera.view.setCenter(camera.view.getSize() / 2.f);

    camera.speed = CAMERA_SPEED;
}

void Game::StartMap()
{
    map.Init(TILEMAP_TEXTURE_FILENAME, TILE_SIZE, MAP_SIZE);

    preview.setSize(sf::Vector2f(map.GetTileSize()));
    preview.setTexture(&map.GetTexture());
}

void Game::StartInfos()
{
    int tileCount = map.GetGridSize().x * map.GetGridSize().y;

    std::string infosText = 
     " [0 - " + std::to_string(tileCount - 1) + "] : Select Tile \n" +
     " + / R2 : Increment Tile \n" + 
     " - / L2 : Decrement Tile \n" + 
     " CTRL C / L1 : Save Map \n" + 
     " CTRL V / R1 : Load Map \n" +
     " T / West : Toggle Tileset \n";

    infos.setString(infosText);
}

void Game::StartSelected()
{
    selected.input = '0';

    EventTileSelect();
}

void Game::OnEvent(const sf::Event& event)
{
    event.visit([this](const auto& type) { this->HandleEvent(type); });
}

void Game::HandleEvent(const sf::Event::TextEntered& text)
{ 
    if (text.unicode >= '0' && text.unicode <= '9')
    {
        if (!selected.used)
        {
            selected.input += (char)text.unicode;
        }
        else
        {
            selected.input = (char)text.unicode;
        }

        EventTileSelect();
    }
}

void Game::HandleEvent(const sf::Event::KeyPressed& key)
{
    switch (key.scancode)
    {
        case sf::Keyboard::Scan::NumpadPlus:
        {
            selected.input = std::to_string(std::stoi(selected.input) + 1);
            EventTileSelect();
            break;
        }
        case sf::Keyboard::Scan::NumpadMinus:
        {
            selected.input = std::to_string(std::stoi(selected.input) - 1);
            EventTileSelect();
            break;
        }
        case sf::Keyboard::Scan::C:
        {
            if (key.control)
            {
                map.SaveToFile(TILEMAP_LEVEL_FILENAME);
            }
            break;
        }
        case sf::Keyboard::Scan::V:
        {
            if (key.control)
            {
                VERIFY(map.LoadFromFile(TILEMAP_LEVEL_FILENAME, TILEMAP_TEXTURE_FILENAME));
            }
            break;
        }
        case sf::Keyboard::Scan::Q:
        {
            ctx.scenes.ChangeScene("Adventure");
            return;
        }
        case sf::Keyboard::Scan::T:
        {
            showTileset = !showTileset;
            break;
        }
        default: break;
    }
}

void Game::HandleEvent(const sf::Event::JoystickButtonPressed& joystick)
{
    auto button = Input::HardwareToLogical(joystick.button, joystick.joystickId);
    if (!button)
    {
        return;
    }

    switch (*button)
    {
        case GamepadButton::R2:
        {
            selected.input = std::to_string(std::stoi(selected.input) + 1);
            EventTileSelect();
            break;
        }
        case GamepadButton::L2:
        {
            selected.input = std::to_string(std::stoi(selected.input) - 1);
            EventTileSelect();
            break;
        }
        case GamepadButton::L1:
        {
            map.SaveToFile(TILEMAP_LEVEL_FILENAME);
            break;
        }
        case GamepadButton::R1:
        {
            VERIFY(map.LoadFromFile(TILEMAP_LEVEL_FILENAME, TILEMAP_TEXTURE_FILENAME));
            break;
        }
        case GamepadButton::North:
        {
            ctx.scenes.ChangeScene("Adventure");
            return;
        }
        case GamepadButton::West:
        {
            showTileset = !showTileset;
            break;
        }
        case GamepadButton::South:
        {
            EventTilesetSelect();
            break;
        }
        case GamepadButton::R3:
        {
            camera.view.zoom(0.9f);
            break;
        }
        case GamepadButton::L3:
        {
            camera.view.zoom(1.1f);
            break;
        }
        default: break;
    }
}

void Game::HandleEvent(const sf::Event::MouseButtonPressed& mouse)
{
    if (mouse.button == sf::Mouse::Button::Left)
    {
        EventTilesetSelect();
    }
}

void Game::HandleEvent(const sf::Event::MouseWheelScrolled& mouse)
{
    if (mouse.wheel == sf::Mouse::Wheel::Vertical)
    {
        camera.view.zoom((mouse.delta > 0) ? 0.9f : mouse.delta < 0 ? 1.1f : 1);
    }
}

void Game::Update()
{
    if (ctx.input.Pressed(Click) && placeCooldown.IsOver())
    {
        EventTilePlace();
    }

    if (!showTileset)
    {
        UpdateView();
        UpdatePreview();
    }
}

void Game::UpdateView()
{
    if (ctx.input.Pressed(MoveUp))
    {
        camera.view.move({0, -camera.speed * ctx.time.GetDeltaTime()});
    }
    if (ctx.input.Pressed(MoveLeft))
    {
        camera.view.move({-camera.speed * ctx.time.GetDeltaTime(), 0});
    }
    if (ctx.input.Pressed(MoveDown))
    {
        camera.view.move({0, camera.speed * ctx.time.GetDeltaTime()});
    }
    if (ctx.input.Pressed(MoveRight))
    {
        camera.view.move({camera.speed * ctx.time.GetDeltaTime(), 0});
    }
}

void Game::UpdatePreview()
{
    preview.setPosition(ctx.cursor.GetPosition(camera.view) + sf::Vector2f(10, 10));
}

void Game::EventTileSelect()
{
    selected.used = false;

    int input = std::stoi(selected.input);
    Tile tile = map.IsTileValid(input) ? input : TILE_EMPTY;

    selected.input = std::to_string(tile);
    selected.tile = std::stoi(selected.input);
    selected.text.setString("Selected Tile: " + selected.input + ' ');
    selected.text.setPosition({gConfig.windowSize.x - selected.text.getLocalBounds().size.x, 
                               gConfig.windowSize.y - selected.text.getLocalBounds().size.y * 2});

    preview.setTextureRect(map.GetTileTextureRect(tile).value_or(sf::IntRect()));
    preview.setFillColor((tile != TILE_EMPTY) ? sf::Color::White : sf::Color::Transparent);
}

void Game::EventTilesetSelect()
{
    sf::Vector2f cursorPosition = ctx.cursor.GetPosition();

    if (!showTileset || IsOutsideWindow(cursorPosition))
    {
        return;
    }

    sf::Vector2i tile = GetTilesetTile(sf::Vector2i(cursorPosition));
    selected.input = std::to_string(tile.x + tile.y * map.GetGridSize().x);
    selected.used = false;

    showTileset = false;
    placeCooldown.Restart();

    EventTileSelect();
}

void Game::EventTilePlace()
{
    selected.used = true;

    sf::Vector2u position = map.WorldToTilePosition(ctx.cursor.GetPosition(camera.view));
    map.SetTile(position, selected.tile);
}

sf::Vector2i Game::GetTilesetTile(sf::Vector2i position) const
{
    sf::Vector2u tileSize = map.GetTileSize();
    sf::Vector2u textureSize = map.GetTexture().getSize();

    return {int(position.x / (tileSize.x * gConfig.windowSize.x / textureSize.x)),
            int(position.y / (tileSize.y * gConfig.windowSize.y / textureSize.y))};
}

void Game::Render() const
{
    ctx.renderer.Draw(background);

    if (showTileset)
    {
        RenderTileset();
    }
    else
    {
        ctx.renderer.SetView(camera.view);
        ctx.renderer.Draw(map);

        RenderGrid();

        ctx.renderer.Draw(preview);
    }

    ctx.renderer.ResetView();

    if (!showTileset)
    {
        ctx.renderer.Draw(selected.text);
        ctx.renderer.Draw(infos);
    }
}

void Game::RenderGrid() const
{
    sf::Vector2i mapPixelSize(map.GetMapSize().componentWiseMul(map.GetTileSize()));

    for (int x = 0; x <= mapPixelSize.x; x += map.GetTileSize().x)
    {
        std::array<sf::Vertex, 2> line =
        {
            sf::Vertex{{(float)x, 0}, GRID_COLOR},
            sf::Vertex{{(float)x, (float)mapPixelSize.y}, GRID_COLOR}
        };

        ctx.renderer.Draw(line, sf::PrimitiveType::Lines);
    }

    for (int y = 0; y <= mapPixelSize.y; y += map.GetTileSize().y)
    {
        std::array<sf::Vertex, 2> line =
        {
            sf::Vertex{{0, (float)y}, GRID_COLOR},
            sf::Vertex{{(float)mapPixelSize.x, (float)y}, GRID_COLOR}
        };

        ctx.renderer.Draw(line, sf::PrimitiveType::Lines);
    }
}

void Game::RenderTileset() const
{
    sf::Sprite tilesetSprite(map.GetTexture());

    tilesetSprite.setTextureRect(sf::IntRect({}, sf::Vector2i(map.GetTexture().getSize())));
    tilesetSprite.setScale(gConfig.windowSize.componentWiseDiv(tilesetSprite.getLocalBounds().size));

    ctx.renderer.Draw(tilesetSprite);
}