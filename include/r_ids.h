#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

/**
 * @namespace Textures
 * A namespace to hold textures in.
 */
namespace Textures {
    /**
     * @enum ID
     * enum of Texture IDs in namespace Textures.
     */
    enum ID {
        Player,
        Grass,
        FireProjectile,
        Bunny,
        Bear,
        TitleScreen,
        HealthRefill,
    };
}

/**
 * @namespace Fonts
 * A namespace to hold fonts in.
 */
namespace Fonts {
    /**
     * @enum ID
     * enum of Font IDs in namespace Fonts.
     */
    enum ID {
        Main
    };
}
