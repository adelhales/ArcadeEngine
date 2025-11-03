// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Graphics.hpp>

#include <optional>
#include <string>
#include <vector>

using Tile = int;

inline constexpr Tile TILE_EMPTY = -1;

class TileMap : public sf::Drawable
{
private:
    sf::Texture tileset_;
    sf::VertexArray vertices_;
    std::vector<Tile> tiles_;
    sf::Vector2u tileSize_;
    sf::Vector2u mapSize_;
    sf::Vector2u gridSize_;

public:
    bool Init(const std::string& tilesetName, sf::Vector2u tileSize, sf::Vector2u mapSize);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool LoadFromFile(const std::string& filename, const std::string& tilesetName);
    bool SaveToFile(const std::string& filename) const;

    bool SetTile(sf::Vector2u position, Tile tile);
    bool IsTileValid(Tile tile) const;
    void Clear();

    sf::Vector2u WorldToTilePosition(sf::Vector2f position) const;
    sf::Vector2f TileToWorldPosition(sf::Vector2u position) const;

    Tile GetTile(sf::Vector2u position) const;
    std::optional<sf::IntRect> GetTileTextureRect(Tile tile) const;

    sf::Vector2u GetTileSize() const;
    sf::Vector2u GetMapSize() const;
    sf::Vector2u GetGridSize() const;
    const sf::Texture& GetTexture() const;
};