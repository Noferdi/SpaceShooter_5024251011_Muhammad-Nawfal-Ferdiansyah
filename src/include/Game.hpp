#pragma once

#include "Player.hpp"
#include "Enemy.hpp"
#include "Ship.hpp"

class Game {
    private:
    RenderWindow window;   // window SFML tempat semua state dirender

    public:
    // Konstruktor: buat window 960x540 dengan limit 60fps
    Game();

    // Jalankan game loop utama (blocking sampai window ditutup)
    void run();
};
