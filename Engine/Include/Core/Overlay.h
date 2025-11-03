// Copyright (c) 2025 Adel Hales

#pragma once

#include "Managers/GuiManager.h"

#include <optional>

enum class OverlaySelection
{
    Resume, Restart, Menu, Quit
};

class Overlay
{
private:
    std::optional<OverlaySelection> selection_;
    tgui::Group::Ptr group_;

public:
    Overlay(GuiManager& gui);

    std::optional<OverlaySelection> FetchSelection();

    void SetVisible(bool visible);
    bool IsVisible() const;

private:
    void InitBackground();
    void InitButtons();
};