// Copyright (c) 2025 Adel Hales

#include "Graphics/TileMap.h"

#include <cmath>
#include <format>
#include <fstream>

#include "Utils/Log.h"

bool TileMap::Init(const std::string& tilesetName, sf::Vector2u tileSize, sf::Vector2u mapSize)
{
    if (!tileset_.loadFromFile("Content/Textures/" + tilesetName))
    {
        LOG_ERROR("Failed to load tileset: {}", tilesetName);
        return false;
    }

    vertices_.setPrimitiveType(sf::PrimitiveType::Triangles);
    tileSize_ = tileSize;
    mapSize_  = mapSize;
    gridSize_ = tileset_.getSize().componentWiseDiv(tileSize_);

    Clear();

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = &tileset_;
    target.draw(vertices_, states);
}

bool TileMap::LoadFromFile(const std::string& filename, const std::string& tilesetName)
{
    std::ifstream file("Content/Levels/" + filename);
    if (!file)
    {
        LOG_ERROR("Failed to open file: {}", filename);
        return false;
    }

    sf::Vector2u tileSize, mapSize;
    file >> tileSize.x >> tileSize.y >> mapSize.x >> mapSize.y;

    if (!Init(tilesetName, tileSize, mapSize))
    {
        return false;
    }

    for (std::size_t i = 0; i < tiles_.size(); i++)
    {
        file >> tiles_[i];
        SetTile({(int)i % mapSize_.x, (int)i / mapSize_.x}, tiles_[i]);
    }

    return true;
}

bool TileMap::SaveToFile(const std::string& filename) const
{
    std::ofstream file("Content/Levels/" + filename);
    if (!file)
    {
        LOG_ERROR("Failed to create file: {}", filename);
        return false;
    }

    file << std::format("{} {} {} {}\n", tileSize_.x, tileSize_.y, mapSize_.x, mapSize_.y);

    for (std::size_t i = 0; i < tiles_.size(); i++)
    {
        const bool newline = ((i + 1) % mapSize_.x == 0);
        file << std::format("{:02d}{}", tiles_[i], newline ? '\n' : ' ');
    }

    return true;
}

bool TileMap::SetTile(sf::Vector2u position, Tile tile)
{
    if (position.x >= mapSize_.x || position.y >= mapSize_.y)
    {
        return false;
    }

    const std::size_t index = position.x + position.y * mapSize_.x;
    tiles_[index] = tile;

    static const sf::Vector2u offsets[] = { {0,0}, {1,0}, {0,1}, {1,0}, {1,1}, {0,1} };

    const bool isValid = IsTileValid(tile);
    const sf::Vector2u uv = isValid ? sf::Vector2u(tile % gridSize_.x, tile / gridSize_.x) : sf::Vector2u();
    const sf::Color color = isValid ? sf::Color::White : sf::Color::Transparent;

    for (int i = 0; i < 6; i++)
    {
        sf::Vertex& vertex = vertices_[index * 6 + i];
        vertex.position = sf::Vector2f((position + offsets[i]).componentWiseMul(tileSize_));
        vertex.texCoords = sf::Vector2f((uv + offsets[i]).componentWiseMul(tileSize_));
        vertex.color = color;
    }

    return true;
}

bool TileMap::IsTileValid(Tile tile) const
{
    return tile >= 0 && tile < int(gridSize_.x * gridSize_.y);
}

void TileMap::Clear()
{
    vertices_.clear();
    vertices_.resize(mapSize_.x * mapSize_.y * 6);

    tiles_.clear();
    tiles_.resize(mapSize_.x * mapSize_.y, TILE_EMPTY);
}

sf::Vector2u TileMap::WorldToTilePosition(sf::Vector2f position) const
{
    return {unsigned(std::floor(position.x / tileSize_.x)),
            unsigned(std::floor(position.y / tileSize_.y))};
}

sf::Vector2f TileMap::TileToWorldPosition(sf::Vector2u position) const
{
    return sf::Vector2f(position.componentWiseMul(tileSize_));
}

Tile TileMap::GetTile(sf::Vector2u position) const
{
    const bool isValid = (position.x < mapSize_.x && position.y < mapSize_.y);
    return isValid ? tiles_[position.x + position.y * mapSize_.x] : TILE_EMPTY;
}

std::optional<sf::IntRect> TileMap::GetTileTextureRect(Tile tile) const
{
    if (!IsTileValid(tile))
    {
        return std::nullopt;
    }

    const sf::Vector2i position(tile % gridSize_.x * tileSize_.x, tile / gridSize_.x * tileSize_.y);
    return sf::IntRect(position, sf::Vector2i(tileSize_));
}

sf::Vector2u TileMap::GetTileSize() const
{
    return tileSize_;
}

sf::Vector2u TileMap::GetMapSize() const
{
    return mapSize_;
}

sf::Vector2u TileMap::GetGridSize() const
{
    return gridSize_;
}

const sf::Texture& TileMap::GetTexture() const
{
    return tileset_;
}