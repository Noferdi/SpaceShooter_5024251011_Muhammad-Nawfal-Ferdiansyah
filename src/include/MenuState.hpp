#pragma once

#include "State.hpp"
#include "Backgrounds.hpp"

class MenuState : public State {
    private:
    Backgrounds bg;          // background, hanya layer void

    Text title_text;         // teks judul "SPACE SHOOTER"
    Text start_text;         // teks "Press ENTER to Start" (berkedip)
    Text quit_text;          // teks "Press Q to Quit"

    string next_state;       // "" = tetap; "Game" = pindah ke GameState
    int    blink_timer;      // hitung frame untuk efek kedip teks start
    bool   blink_visible;    // toggle: tampilkan atau sembunyikan teks start

    public:
    MenuState();

    void update() override;

    void render(RenderWindow& window) override;

    string getNextState() override;
};
