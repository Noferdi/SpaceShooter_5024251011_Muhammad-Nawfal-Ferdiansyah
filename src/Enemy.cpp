#include "include/Enemy.hpp"
#include <algorithm>
#include <iostream>

using namespace std;
using namespace sf;

// Konstruktor: set HP, attack, tipe musuh, dan stagger waktu tembak awal
Enemy::Enemy(int Health_point, int Attack_point, string enemy_type)
    : Ship(Health_point, Attack_point), Bullet(nullptr) {
    this->enemy_type        = enemy_type;
    this->base_health_point = static_cast<float>(Health_point);
    this->shoot_cooldown    = 120;  // tembak tiap
    this->shoot_timer       = rand() % 60;
}

// Muat texture kapal; posisi (x, y) ditentukan oleh GameState saat spawn
Sprite& Enemy::setShape(const string& texturePath, float x, float y) {
    this->enemy_shape = Sprite();

    if (this->enemy_texture.loadFromFile(texturePath)) {
        this->enemy_shape.setTexture(this->enemy_texture);
    }

    this->enemy_shape.setScale(1.f, 1.f);
    // Origin di tengah agar rotasi berputar di pusat sprite
    this->enemy_shape.setOrigin(
        this->enemy_shape.getLocalBounds().width  / 2.f,
        this->enemy_shape.getLocalBounds().height / 2.f
    );
    this->enemy_shape.setRotation(270.f);   // sprite menghadap kiri
    this->enemy_shape.setPosition(x, y);

    return this->enemy_shape;
}

// Kembalikan referensi sprite kapal
Sprite& Enemy::getShape() {
    return this->enemy_shape;
}

// Gerakkan kapal ke kiri sebesar 1.5px per frame
void Enemy::move() {
    this->enemy_shape.move(-1.5f, 0.f); // bergerak ke kiri
}

// Spawn torpedo ke kiri bila cooldown sudah selesai
// path Torpedo.png (sprite sheet 33x32, 3 frame masing-masing 11x32)
void Enemy::Attack(const string& torpedoTexPath) {
    if (this->shoot_timer < this->shoot_cooldown) return;

    this->shoot_timer = 0; // reset cooldown

    // Torpedo: speed -5 (ke kiri), rotasi 270° 
    // fcount=3 karena Torpedo.png punya 3 frame animasi
    // fw=11 karena lebar tiap frame adalah 11px (33px total / 3 frame)
    // fh=32 karena tinggi frame adalah 32px
    Bullet b(nullptr);
    b.setShape(torpedoTexPath, -5.f, 270.f, 3, 11, 32);
    b.getShape().setPosition(this->enemy_shape.getPosition()); // dari posisi kapal
    this->bullets.push_back(b);
}

// Update semua torpedo: cooldown, gerak, animasi, bersihkan tidak aktif
// torpedoTexPath: diteruskan untuk spawn torpedo baru
void Enemy::update(float window_width, const string& torpedoTexPath) {
    if (!this->isAlive()) return; // enemy mati, tidak perlu update

    this->shoot_timer++; // hitung frame untuk cooldown tembakan

    this->move();                          // kapal enemy bergerak ke kiri
    this->Attack(torpedoTexPath);          // coba tembak torpedo

    // Update tiap torpedo aktif
    for (auto& b : this->bullets) {
        b.move();
        b.updateAnimation();
        b.checkBounds(window_width);
    }

    // Hapus torpedo tidak aktif
    this->bullets.erase(
        remove_if(this->bullets.begin(), this->bullets.end(),
            [](const Bullet& b) { return !b.isActive(); }),
        this->bullets.end()
    );
}

// Gambar kapal dan semua torpedo ke window
void Enemy::draw(RenderWindow& window) {
    if (this->isAlive()) {
        window.draw(this->enemy_shape); // gambar kapal hanya bila hidup
    }

    // Torpedo tetap digambar meski enemy mati (masih terbang)
    for (auto& b : this->bullets) {
        if (b.isActive()) {
            window.draw(b.getShape());
        }
    }
}

// Kurangi HP (override takeDamage dari Ship)
void Enemy::takeDamage(int damage) {
    Ship::takeDamage(damage); // panggil implementasi Ship
}

// True bila kapal sudah melewati tepi kiri layar
bool Enemy::isOffScreen() {
    return this->enemy_shape.getPosition().x < -80.f;
}

vector<Bullet>& Enemy::getBullets() {
    return this->bullets;
}

FloatRect Enemy::getBounds() {
    return this->enemy_shape.getGlobalBounds();
}

Vector2f Enemy::getPosition() {
    return this->enemy_shape.getPosition();
}
