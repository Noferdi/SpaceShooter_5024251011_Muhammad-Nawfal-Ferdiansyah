#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Backgrounds {
    private:
    Texture tex_void;    // texture Layer 01 – Void (latar gelap)
    Texture tex_stars;   // texture Layer 02 – Stars (bintang bergerak)

    // Dua copy tiap layer untuk seamless tile
    Sprite layer1a, layer1b;   // Layer 01 copy A dan B
    Sprite layer2a, layer2b;   // Layer 02 copy A dan B

    float scroll_void;    // posisi scroll Layer 01 (pixel, selalu negatif/nol)
    float scroll_stars;   // posisi scroll Layer 02 (pixel, selalu negatif/nol)

    // Kecepatan scroll per frame (bukan per detik) karena setFramerateLimit(60)
    static const float SCALE;          // scale sprite dari ukuran PNG asli
    static const float SPEED_VOID;     // px/frame untuk Layer 01
    static const float SPEED_STARS;    // px/frame untuk Layer 02

    bool loaded;   // true bila kedua texture berhasil dimuat

    public:
    Backgrounds();

    bool load();

    void updateMenuLayer();

    void update();

    void drawMenuLayer(RenderWindow& window);

    void draw(RenderWindow& window);

    bool isLoaded();
};

// Static constants declared; definitions are in Backgrounds.cpp
