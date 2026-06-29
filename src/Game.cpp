#include "include/Game.hpp"
#include "include/State.hpp"
#include "include/MenuState.hpp"
#include "include/GameState.hpp"
#include "include/GameOverState.hpp"
#include "include/Audio.hpp"
#include <cstdlib>   // srand, rand
#include <ctime>     // time
#include <cstdlib>   // srand, rand
#include <ctime>     // time


Game::Game() : window(VideoMode(960, 540), "Space Shooter", Style::Close | Style::Titlebar) {}

void Game::run() {
    this->window.setFramerateLimit(60); // Kontrol FPS di sini 
    
    srand(static_cast<unsigned>(time(nullptr))); // seed random untuk posisi Y enemy

    // Muat semua audio sekali di awal
    audio.setAudio("Audio/MainMenu.mp3", "bgm");
    audio.setAudio("Audio/PlayerShot.wav", "shoot");
    audio.setAudio("Audio/GameOver.wav", "gameover");

    int last_score = 0; // skor terakhir diteruskan ke GameOverState

    State* current_state = new MenuState();

    // Game loop: jalan terus selama window terbuka
    while (this->window.isOpen()) {

        // --- Handle event SFML (close button) ---
        Event event;
        while (this->window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                this->window.close(); // tutup bila user klik X
            }
        }

        // --- Update state aktif  ---
        current_state->update();

        // --- Cek apakah state ingin transisi ---
        string next = current_state->getNextState();

        if (next == "Game") {
            // Mulai sesi gameplay baru; teruskan dimensi window
            delete current_state;
            current_state = new GameState(960.f, 540.f);

        } else if (next == "GameOver") {
            // Ambil skor dari GameState sebelum di-destroy oleh unique_ptr
            GameState* gs = dynamic_cast<GameState*>(current_state);
            if (gs != nullptr) {
                last_score = gs->getScore(); // simpan skor
            }
            delete current_state;
            current_state = new GameOverState(last_score);

        } else if (next == "Menu") {
            delete current_state;
            current_state = new MenuState();

        } else if (next == "Quit") {
            this->window.close(); // GameState atau state lain meminta keluar
        }

        // --- Render ---
        this->window.clear(Color(5, 5, 20));        // bersihkan ke biru sangat gelap
        current_state->render(this->window);         // gambar state aktif
        this->window.display();                      // tampilkan frame ke layar
    }

    // Clean up current state
    delete current_state;
}
