#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

class Bullet {
    private:
    Sprite   bullet_shape;      // sprite yang dirender ke layar
    Texture  internal_texture;  // texture MILIK BULLET; di-rebind saat copy
    Texture* texture;           // pointer luar (style FP6, tidak dipakai render)

    // --- Variabel animasi sprite sheet ---
    int   frame_count;      // jumlah frame dalam sprite sheet (3 untuk keduanya)
    int   frame_width;      // lebar satu frame: 32 untuk player, 11 untuk torpedo
    int   frame_height;     // tinggi satu frame: 32 untuk keduanya
    int   current_frame;    // indeks frame yang sedang tampil (0, 1, atau 2)
    int   frame_timer;      // hitung frame game sejak frame terakhir berganti
    int   frame_interval;   // selang frame game antar pergantian animasi

    bool  active;           // true = bullet masih hidup di layar
    float speed;            // kecepatan gerak horizontal (px per frame, tanpa dt)

    // Disimpan untuk copy constructor agar bisa load ulang texture
    string texture_path;    // path file yang dipakai setShape()

    public:
    Bullet(Texture* texture_ptr);

    Bullet(const Bullet& other);

    Bullet& operator=(const Bullet& other);

    Sprite& setShape(const string& texturePath, float spd, float rotation,
                     int   fcount = 3, int   fw     = 32, int   fh     = 32);

    Sprite& getShape();

    void move();

    void updateAnimation();

    void checkBounds(float window_width);

    bool collision(const Sprite& other);

    bool collision(const FloatRect& other_bounds);

    bool      isActive()    const;
    FloatRect getBounds()   const;
    Vector2f  getPosition() const;
};
