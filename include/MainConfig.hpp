#pragma once

#include "config-utils/shared/config-utils.hpp"
#include "UnityEngine/Color.hpp"
using namespace UnityEngine;

DECLARE_CONFIG(MainConfig,
    CONFIG_VALUE(ModEnabled, bool, "Enable Mod!", false, "Enable Saber Marqs Mod");
    CONFIG_VALUE(OverideColors, bool, "OverideColor", false, "Overide BaseGame Colors");
    CONFIG_VALUE(OverideLifetime, bool, "OverideLifeTime", false, "Enable or disable lifetime. If disabled then burnmarks will never fade!")
    CONFIG_VALUE(BurnOpacity, float, "Burn Opacity", 1.0f, "The Burn Opacity");
    CONFIG_VALUE(BurnScale, float, "Burn Scale", 1.0f, "The Scale of the Burn");
    CONFIG_VALUE(BurnLifeTime, float, "Burn Life Time", 2.0f, "The Life Time of the Burn");

    // Colors
    CONFIG_VALUE(LeftSaberColor, Color, "Left Saber Burn Marq Color", Color(1, 0, 0, 1));
    CONFIG_VALUE(RightSaberColor, Color, "Right Saber Burn Marq Color", Color(0, 0, 1, 1));

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(ModEnabled);
        CONFIG_INIT_VALUE(OverideColors);
        CONFIG_INIT_VALUE(OverideLifetime);
        CONFIG_INIT_VALUE(BurnOpacity);
        CONFIG_INIT_VALUE(BurnScale);
        CONFIG_INIT_VALUE(BurnLifeTime);

        
        CONFIG_INIT_VALUE(LeftSaberColor);
        CONFIG_INIT_VALUE(RightSaberColor);

    )
)