// Copyright (c) 2025 Adel Hales

#pragma once

#include "Types/LevelEditorTypes.h"

namespace LevelEditor
{
    class Game : public Scene
    {
    private:
        TileMap map;
        Camera camera;
        sf::RectangleShape preview;
        SelectedTile selected;
        sf::Text infos;
        bool showTileset;
        Cooldown placeCooldown;
        sf::RectangleShape background;

    public:
        Game(EngineContext&);

        void Start();
        void OnEvent(const sf::Event&);
        void Update();
        void Render() const;

    private:
        void InitInfos();
        void InitSelected();
        void InitBackground();

        void BindInputs();
        
        void StartCamera();
        void StartMap();
        void StartInfos();
        void StartSelected();

        void HandleEvent(const sf::Event::TextEntered&);
        void HandleEvent(const sf::Event::KeyPressed&);
        void HandleEvent(const sf::Event::JoystickButtonPressed&);
        void HandleEvent(const sf::Event::MouseButtonPressed&);
        void HandleEvent(const sf::Event::MouseWheelScrolled&);
        void HandleEvent(const auto&) {}

        void UpdateView();
        void UpdatePreview();

        void EventTileSelect();
        void EventTilesetSelect();
        void EventTilePlace();

        sf::Vector2i GetTilesetTile(sf::Vector2i position) const;

        void RenderGrid() const;
        void RenderTileset() const;
    };
}