#pragma once

namespace Maps {
    constexpr int MAP_WIDTH 1280;
    constexpr int MAP_HEIGHT 960;
}

namespace Tiles {
    // tile constants
    constexpr int TILE_WIDTH = 64;
    constexpr int TILE_HEIGHT = 64;
    constexpr int TOTAL_TILES = 192;
    constexpr int TOTAL_TILE_SPRITES 12;

    enum ID {
        Grass,
        Brick
    };
}
