#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;


class State {
    private:
    string stateName;   // nama state: "Menu", "Game", atau "GameOver"

    protected:
    Font mono_font;     // font VCR mono; tersedia di semua state turunan

    public:
    // Konstruktor: simpan nama dan muat font bersama
    State(string stateName);

    virtual ~State();

    // Pure virtual: logika game setiap frame (tanpa dt, per-frame konsisten)
    virtual void update() = 0;

    // Pure virtual: render semua objek visual ke window
    virtual void render(RenderWindow& window) = 0;

    // Kembalikan nama state tujuan untuk transisi:
    //   ""           = tetap di state ini
    //   "Menu"       = pindah ke MenuState
    //   "Game"       = pindah ke GameState
    //   "GameOver"   = pindah ke GameOverState
    virtual string getNextState() { return ""; }

    // Getter nama state (berguna untuk debug atau transisi di Game::run)
    string getName() { return this->stateName; }
};
