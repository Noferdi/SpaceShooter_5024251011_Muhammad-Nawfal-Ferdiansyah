#include "include/GameOverState.hpp"
#include "include/Audio.hpp"

using namespace std;
using namespace sf;

// final_score: skor diteruskan dari GameState sebelum di-destroy
GameOverState::GameOverState(int final_score) : State("GameOver") {
    this->score       = final_score;
    this->next_state  = "";
    this->delay_timer = 0; // cooldown 60 frame sebelum terima input

    // Muat background (cukup layer void saja)
    this->bg.load();

    audio.stopAudio("bgm");
    audio.playAudio("gameover");

    // --- Teks "GAME OVER" ---
    this->game_over_text.setFont(this->mono_font);
    this->game_over_text.setString("GAME OVER");
    this->game_over_text.setCharacterSize(60);
    this->game_over_text.setFillColor(Color(255, 60, 60)); // merah
    FloatRect gb = this->game_over_text.getLocalBounds();
    this->game_over_text.setOrigin(gb.width / 2.f, gb.height / 2.f);
    this->game_over_text.setPosition(480.f, 160.f);

    // --- Teks skor final ---
    this->score_text.setFont(this->mono_font);
    this->score_text.setString("Score: " + to_string(this->score));
    this->score_text.setCharacterSize(32);
    this->score_text.setFillColor(Color::Yellow);
    FloatRect sb = this->score_text.getLocalBounds();
    this->score_text.setOrigin(sb.width / 2.f, sb.height / 2.f);
    this->score_text.setPosition(480.f, 270.f);

    // --- Teks restart ---
    this->restart_text.setFont(this->mono_font);
    this->restart_text.setString("Press R to Return to Menu");
    this->restart_text.setCharacterSize(26);
    this->restart_text.setFillColor(Color::White);
    FloatRect rb = this->restart_text.getLocalBounds();
    this->restart_text.setOrigin(rb.width / 2.f, rb.height / 2.f);
    this->restart_text.setPosition(480.f, 360.f);

    // --- Teks quit ---
    this->quit_text.setFont(this->mono_font);
    this->quit_text.setString("Press Q to Quit");
    this->quit_text.setCharacterSize(22);
    this->quit_text.setFillColor(Color(160, 160, 160)); // abu-abu
    FloatRect qb = this->quit_text.getLocalBounds();
    this->quit_text.setOrigin(qb.width / 2.f, qb.height / 2.f);
    this->quit_text.setPosition(480.f, 430.f);
}

// Update dipanggil tiap frame; input diabaikan pada frame-frame awal
void GameOverState::update() {
    // Scroll background layer void secara pelan
    this->bg.updateMenuLayer();

    this->delay_timer++; // hitung frame

    // Abaikan input selama frame-frame awal
    if (this->delay_timer < 60) return;

    // Cek input setelah cooldown selesai
    if (Keyboard::isKeyPressed(Keyboard::R)) {
        this->next_state = "Menu"; // kembali ke menu
    }
    if (Keyboard::isKeyPressed(Keyboard::Q)) {
        this->next_state = "Quit"; // sinyal untuk tutup window
    }
}

// Render semua elemen visual ke window
void GameOverState::render(RenderWindow& window) {
    this->bg.drawMenuLayer(window);       // background layer void
    window.draw(this->game_over_text);    // judul
    window.draw(this->score_text);        // skor
    window.draw(this->restart_text);      // instruksi R
    window.draw(this->quit_text);         // instruksi Q
}

string GameOverState::getNextState() { return this->next_state; }

