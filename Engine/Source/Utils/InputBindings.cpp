// Copyright (c) 2025 Adel Hales

#include "Utils/InputBindings.h"

#include <array>

namespace Mapping
{
    const std::array<int, 12> PlayStation = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11};
    const std::array<int, 12> Xbox        = {2, 0, 1, 3, 4, 5,-1,-1, 6, 7, 8, 9};

    //const int SonyVID    = 0x054C;
    const int MicrosoftVID = 0x045E;

    const auto& GetMapping(int id)
    {
        const int vendorId = sf::Joystick::getIdentification(id).vendorId;
        return (vendorId == MicrosoftVID) ? Xbox : PlayStation;
    }
}

std::optional<int> Input::LogicalToHardware(GamepadButton button, int id)
{
    const auto& mapping = Mapping::GetMapping(id);

    const int value = mapping[(int)button];

    return (value != -1) ? std::optional(value) : std::nullopt;
}

std::optional<GamepadButton> Input::HardwareToLogical(int button, int id)
{
    const auto& mapping = Mapping::GetMapping(id);

    for (std::size_t i = 0; i < mapping.size(); i++)
    {
        if (mapping[i] == button)
        {
            return GamepadButton(i);
        }
    }

    return std::nullopt;
}