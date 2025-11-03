// Copyright (c) 2025 Adel Hales

#include "Menu.h"

using namespace Menu;

Game::Game(EngineContext& context) :
    Scene(context),
    music(*ctx.resources.FetchMusic(MUSIC_FILENAME))
{
    InitButtons();
    InitControls();
    InitMusic();
}

void Game::InitButtons()
{
    int count = (int)BUTTONS_NAMES.size();

    int cols = std::min(BUTTONS_PER_ROW, count);
    int rows = (count + cols - 1) / cols;

    sf::Vector2f size(gConfig.windowSize.x / (cols + BUTTONS_PADDING_RATIO),
                      gConfig.windowSize.y / (rows + BUTTONS_PADDING_RATIO));

    sf::Vector2f spacing(size.x * BUTTONS_PADDING_RATIO / (cols + 1),
                         size.y * BUTTONS_PADDING_RATIO / (rows + 1));

    sf::Vector2f start = spacing + size / 2.f;
    sf::Vector2f step  = spacing + size;

    buttons.reserve(count);

    for (int i = 0; i < count; i++)
    {
        sf::Vector2f position(start.x + step.x * (i % cols),
                              start.y + step.y * (i / cols));

        auto& button = buttons.emplace_back();

        InitButton(button, {position, size}, BUTTONS_NAMES[i]);
    }
}

void Game::InitButton(Button& button, sf::FloatRect bounds, std::string name)
{
    button.shape.setTexture(ctx.resources.FetchTexture(BUTTON_TEXTURE_FILENAME));
    button.shape.setFillColor(BUTTON_COLOR);
    button.shape.setSize(bounds.size);
    button.shape.setOrigin(button.shape.getGeometricCenter());
    button.shape.setPosition(bounds.position);

    button.name = name;
    std::ranges::replace(name, ' ', '\n');

    button.text.setFillColor(BUTTON_TEXT_COLOR);
    button.text.setString(name);
    button.text.setCharacterSize(BUTTON_TEXT_SIZE);
    button.text.setOrigin(button.text.getLocalBounds().getCenter());
    button.text.setPosition(bounds.position);
}

void Game::InitControls()
{
    controls.background.setFillColor(CONTROLS_BACKGROUND_COLOR);
    controls.background.setSize(gConfig.windowSize);

    controls.texts.reserve(CONTROLS_TEXTS.size());

    for (const auto& info : CONTROLS_TEXTS)
    {
        auto& text = controls.texts.emplace_back(GetDefaultFont(), info, CONTROLS_TEXT_SIZE);

        text.setFillColor(CONTROLS_TEXT_COLOR);
        text.setOrigin(text.getLocalBounds().getCenter());
        text.setPosition(gConfig.windowSize / 2.f);
    }
}

void Game::InitMusic()
{
    music.setVolume(MUSIC_VOLUME);
    music.setPitch(MUSIC_PITCH);
}

void Game::Start()
{
    StartControls();
    StartMusic();
}

void Game::StartControls()
{
    controls.current.reset();
}

void Game::StartMusic()
{
    music.play();
}

void Game::OnEvent(const sf::Event& event)
{
    event.visit([this](const auto& type) { this->HandleEvent(type); });
}

void Game::HandleEvent(const sf::Event::MouseButtonPressed& mouse)
{
    if (mouse.button == sf::Mouse::Button::Left && !controls.current)
    {
        EventSceneLoad();
    }
    else if (mouse.button == sf::Mouse::Button::Right)
    {
        EventControlsShow();
    }
}

void Game::HandleEvent(const sf::Event::JoystickButtonPressed& joystick)
{
    if (auto button = Input::HardwareToLogical(joystick.button, joystick.joystickId))
    {
        if (*button == GamepadButton::South && !controls.current)
        {
            EventSceneLoad();
        }
        else if (*button == GamepadButton::West)
        {
            EventControlsShow();
        }
    }
}

void Game::Update()
{
    if (!controls.current)
    {
        UpdateButtons();
    }
}

void Game::UpdateButtons()
{
    for (auto& button : buttons)
    {
        UpdateButton(button);
    }
}

void Game::UpdateButton(Button& button)
{
    sf::Color color = IsButtonHovered(button) ? BUTTON_HOVERED_COLOR : BUTTON_COLOR;

    if (button.shape.getFillColor() != color)
    {
        button.shape.setFillColor(color);
    }
}

void Game::EventSceneLoad()
{
    for (const auto& button : buttons)
    {
        if (IsButtonHovered(button))
        {
            ctx.scenes.ChangeScene(button.name);
            return;
        }
    }
}

void Game::EventControlsShow()
{
    if (controls.current)
    {
        controls.current.reset();
        return;
    }

    for (std::size_t i = 0; i < buttons.size(); i++)
    {
        if (IsButtonHovered(buttons[i]))
        {
            controls.current = (int)i;
            return;
        }
    }
}

bool Game::IsButtonHovered(const Button& button) const
{
    return Contains(button.shape, ctx.cursor.GetPosition());
}

void Game::Render() const
{
    if (controls.current)
    {
        ctx.renderer.Draw(controls.background);
        ctx.renderer.Draw(controls.texts[*controls.current]);
        return;
    }

    for (const auto& button : buttons)
    {
        ctx.renderer.Draw(button.shape);
        ctx.renderer.Draw(button.text);
    }
}

void Game::OnPause(bool paused)
{
    if (paused)
    {
        music.pause();
    }
    else
    {
        music.play();
    }
}

void Game::OnCleanup()
{
    music.stop();
}