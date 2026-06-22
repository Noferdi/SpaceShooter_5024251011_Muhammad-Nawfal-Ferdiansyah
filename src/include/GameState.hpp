#pragma once

#include "State.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Backgrounds.hpp"
#include <vector>
#include <cstdlib>   // rand()

// Path aset; ubah di sini bila nama file berubah
#define PlayerTexPath   "img/Player/player100.png"
#define BulletTexPath   "img/Player/bullet.png"
#define EnemyTexPath    "img/Enemy/Kla'ed/Kla'ed - Battlecruiser - Base.png"
#define TorpedoTexPath  "img/Enemy/Kla'ed/Kla'ed - Torpedo.png"

class GameState : public State {
    private:
    Player  player;                        // objek pemain tunggal
    vector<Enemy*> enemies;     // semua enemy aktif di layar (raw pointers)
    Backgrounds bg;                        // dual-layer background

    Text score_text;      // teks HUD skor di pojok kiri atas
    Text health_text;     // teks HUD HP player di bawah skor
    Text ammo_text;       // teks HUD ammo player di bawah HP
    Text instructions_text; // teks instruksi: E to menu, Q to quit

    int    score;          // skor pemain; +100 per enemy yang dihancurkan
    string next_state;   // "" = tetap; "GameOver" = game over

    // Variabel spawn enemy
    int    spawn_timer;     // hitung frame sejak enemy terakhir spawn
    int    spawn_interval;  // jeda antar spawn dalam frame (120)

    float  win_width;    // lebar layar untuk batas bullet dan spawn x
    float  win_height;   // tinggi layar untuk batas gerak player dan random Y

    public:
    GameState(float win_w = 960.f, float win_h = 540.f);

    // Update dipanggil tiap frame; tidak pakai dt (frame rate tetap)
    void update() override;

    // Pastikan hapus semua enemy yang tersisa saat GameState dihancurkan
    ~GameState();

    // Render semua elemen visual ke window
    void render(RenderWindow& window) override;

    string getNextState() override;

    int getScore();
};
