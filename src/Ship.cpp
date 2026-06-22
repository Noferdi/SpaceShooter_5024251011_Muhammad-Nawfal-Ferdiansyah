#include "include/Ship.hpp"
#include <iostream>

using namespace std;

// Konstruktor: set HP dan attack awal
Ship::Ship(int Health_point, int Attack_point) {
    this->health_point = Health_point;
    this->attack_point = Attack_point;
}

// Kurangi HP; dipanggil saat kena tembakan atau tabrakan
void Ship::takeDamage(int damage) {
    this->health_point -= damage;
    if (this->health_point < 0) {
        this->health_point = 0;
    }
}

// Kembalikan HP saat ini
int Ship::getHealth() {
    return this->health_point;
}

// Kembalikan damage per tembakan
int Ship::getAttack() {
    return this->attack_point;
}

// True bila HP masih di atas 0
bool Ship::isAlive() {
    return this->health_point > 0;
}

// Placeholder Attack; di-override oleh Player dan Enemy
void Ship::Attack(const string& texturePath) {
    (void)texturePath; // no-op default
}

// Hapus kapal dari permainan (implementasi di turunan)
void Ship::remove() {}

// Virtual destructor
Ship::~Ship() {}
