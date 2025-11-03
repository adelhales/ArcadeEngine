// Copyright (c) 2025 Adel Hales

#include "Managers/ClipboardManager.h"

#include <SFML/Window/Clipboard.hpp>

void ClipboardManager::SetString(const sf::String& text)
{
    sf::Clipboard::setString(text);
}

sf::String ClipboardManager::GetString() const
{
    return sf::Clipboard::getString();
}