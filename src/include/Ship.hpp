#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

class Ship {
    private:
    int health_point;   // nyawa kapal saat ini
    int attack_point;   // damage yang diberikan tiap tembakan

    public:
    // Konstruktor: set HP dan attack awal
    Ship(int Health_point, int Attack_point);

    // Kurangi HP; dipanggil saat kena tembakan atau tabrakan
    virtual void takeDamage(int damage);

    // Kembalikan HP saat ini
    int getHealth();

    // Kembalikan damage per tembakan
    int getAttack();

    // True bila HP masih di atas 0
    bool isAlive();

    // Placeholder Attack; di-override oleh Player dan Enemy
    // Use a texture path parameter so derived classes can override without hiding
    virtual void Attack(const string& texturePath);

    // Hapus kapal dari permainan (implementasi di turunan)
    void remove();

    // Virtual destructor to allow safe deletion via base pointers
    virtual ~Ship();
};
