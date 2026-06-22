#include <SFML/Graphics.hpp>
#include "include/Game.hpp"
#include "include/Player.hpp"
#include "include/Enemy.hpp"
#include "include/Ship.hpp"

using namespace sf;
using namespace std;

int main() {
    // Buat objek Game (menginisialisasi window 960x540, limit 60fps)
    Game game;

    // Jalankan game loop (blocking hingga window ditutup)
    game.run();

    return 0;
}