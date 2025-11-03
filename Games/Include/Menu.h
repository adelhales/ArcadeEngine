// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/MenuTypes.h"

namespace Menu
{
    class Game : public Scene
    {
    private:
        std::vector<Button> buttons;
        Controls controls;
        sf::Music music;

    public:
        Game(EngineContext&);

        void Start();
        void OnEvent(const sf::Event&);
        void Update();
        void Render() const;
        void OnPause(bool);
        void OnCleanup();

    private:
        void InitButtons();
        void InitButton(Button& button, sf::FloatRect bounds, std::string name);
        void InitControls();
        void InitMusic();

        void StartControls();
        void StartMusic();

        void HandleEvent(const sf::Event::MouseButtonPressed&);
        void HandleEvent(const sf::Event::JoystickButtonPressed&);
        void HandleEvent(const auto&) {}

        void UpdateButtons();
        void UpdateButton(Button& button);

        void EventSceneLoad();
        void EventControlsShow();

        bool IsButtonHovered(const Button& button) const;
    };
}