#pragma once

#include "State.hpp"
#include "Backgrounds.hpp"

class GameOverState : public State {
    private:
    Backgrounds bg;          // background layer void (scroll pelan)

    Text game_over_text;     // teks besar "GAME OVER"
    Text score_text;         // teks skor: "Score: XXXX"
    Text restart_text;       // instruksi "Press R to Return to Menu"
    Text quit_text;          // instruksi "Press Q to Quit"

    string next_state;       // "" = tetap; "Menu" = kembali ke menu
    int    score;            // skor akhir yang diterima dari GameState
    int    delay_timer;      // hitung frame; input diabaikan selama < 60 frame

    public:
    // final_score: skor diteruskan dari GameState sebelum di-destroy
    GameOverState(int final_score = 0);

    // Update dipanggil tiap frame; input diabaikan pada frame-frame awal
    void update() override;

    // Render semua elemen visual ke window
    void render(RenderWindow& window) override;

    string getNextState() override;
};
