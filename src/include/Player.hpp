#pragma once

#include "Ship.hpp"
#include "Bullet.hpp"
#include <vector>

class Player : public Ship, public Bullet {
    private:
    Sprite  player_shape;    // sprite visual kapal player
    Texture* texture;        // pointer texture kapal (style FP6: new Texture)

    int health_point;        // HP lokal (sync dengan Ship base)
    int attack_point;        // damage tiap tembakan (sync dengan Ship base)

    vector<Bullet> bullets;  // semua bullet aktif milik player

    int shoot_cooldown;      // jeda antar tembakan dalam satuan frame game
    int shoot_timer;         // hitungan frame sejak tembakan terakhir
    int ammo;                // jumlah peluru saat ini
    int max_ammo;            // kapasitas peluru maksimum
    int ammo_regen_timer;    // timer regenarasi peluru dalam frame
    int ammo_regen_interval; // jeda regenarasi dalam frame (3 detik)
    float window_height;     // tinggi layar untuk batas gerak atas/bawah

    public:
    // Konstruktor: set HP, attack, dan alokasi texture kapal
    Player(int health_point, int attack_point);

    ~Player();

    // Muat texture kapal dan atur tampilan sprite awal
    // win_height: tinggi layar untuk pembatasan gerak
    Sprite& setShape(const string& texturePath, float win_height = 540.f);

    // Kembalikan referensi sprite kapal
    Sprite& getShape();

    // Gerak ke atas satu langkah per frame; dibatasi tepi atas layar
    void moveUp();

    // Gerak ke bawah satu langkah per frame; dibatasi tepi bawah layar
    void moveDown();

    // Spawn satu Bullet bila cooldown sudah selesai
    // bulletTexPath: path sprite sheet bullet (96x32, 3 frame 32x32)
    void Attack(const string& bulletTexPath);

    // Update semua bullet: cooldown, gerak, animasi, bersihkan tidak aktif
    // Dipanggil tiap frame; tidak pakai dt (kecepatan per frame tetap)
    void update(float window_width);

    // Gambar kapal dan semua bullet ke window
    void draw(RenderWindow& window);

    // Kurangi HP (override takeDamage dari Ship)
    void takeDamage(int damage);

    // Getter untuk collision dan HUD di GameState
    vector<Bullet>& getBullets();
    FloatRect getBounds();
    Vector2f getPosition();
    int getHealth();
    int getAttack();
    int getAmmo();
    int getMaxAmmo();
    bool isAlive();
};
