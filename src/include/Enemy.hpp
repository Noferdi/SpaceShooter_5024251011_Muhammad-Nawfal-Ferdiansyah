#pragma once

#include "Ship.hpp"
#include "Bullet.hpp"
#include <vector>
#include <cstdlib>   // rand()

class Enemy : public Ship, public Bullet {
    private:
    Sprite  enemy_shape;     // sprite visual kapal enemy
    Texture enemy_texture;   // texture kapal (owned oleh Enemy, bukan pointer)

    vector<Bullet> bullets;  // semua torpedo aktif milik enemy ini

    string  enemy_type;             // tipe musuh untuk ekstensi ke depan
    float   base_health_point;      // HP awal sebelum kena damage

    int     shoot_cooldown;  // jeda antar tembakan torpedo (dalam frame game)
    int     shoot_timer;     // hitung frame sejak torpedo terakhir ditembak

    public:
    // Konstruktor: set HP, attack, tipe musuh, dan stagger waktu tembak awal
    Enemy(int Health_point, int Attack_point, string enemy_type);

    // Muat texture kapal; posisi (x, y) ditentukan oleh GameState saat spawn
    Sprite& setShape(const string& texturePath, float x, float y);

    // Kembalikan referensi sprite kapal
    Sprite& getShape();

    // Gerakkan kapal ke kiri sebesar 1.5px per frame
    void move();

    // Spawn torpedo ke kiri bila cooldown sudah selesai
    // torpedoTexPath: path Torpedo.png (sprite sheet 33x32, 3 frame masing-masing 11x32)
    void Attack(const string& torpedoTexPath);

    // Update semua torpedo: cooldown, gerak, animasi, bersihkan tidak aktif
    // torpedoTexPath: diteruskan untuk spawn torpedo baru
    void update(float window_width, const string& torpedoTexPath);

    // Gambar kapal dan semua torpedo ke window
    void draw(RenderWindow& window);

    // Kurangi HP (override takeDamage dari Ship)
    void takeDamage(int damage);

    // True bila kapal sudah melewati tepi kiri layar (perlu dihapus)
    bool isOffScreen();

    vector<Bullet>& getBullets();

    FloatRect getBounds();

    Vector2f  getPosition();
};
