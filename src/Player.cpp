#include "include/Player.hpp"
#include "include/Audio.hpp"
#include <algorithm>

using namespace std;
using namespace sf;

// Konstruktor: set HP, attack, dan alokasi texture kapal
Player::Player(int health_point, int attack_point)
    : Ship(health_point, attack_point), Bullet(nullptr) {
    this->health_point   = health_point;
    this->attack_point   = attack_point;
    this->texture        = new Texture(); // alokasi texture kapal
    this->shoot_cooldown = 15;   // tembak maks tiap 15 frame
    this->shoot_timer    = 0;
    this->max_ammo       = 20;
    this->ammo           = this->max_ammo;
    this->ammo_regen_timer = 0;
    this->ammo_regen_interval = 180; // 3 detik pada 60 FPS
    this->window_height  = 540.f;
}

Player::~Player() {
    delete this->texture; // bebaskan texture kapal
}

// Muat texture kapal dan atur tampilan sprite awal
// win_height: tinggi layar untuk pembatasan gerak
Sprite& Player::setShape(const string& texturePath, float win_height) {
    this->window_height = win_height;
    this->player_shape  = Sprite();

    if (this->texture->loadFromFile(texturePath)) {
        this->player_shape.setTexture(*this->texture);
    }

    this->player_shape.setScale(2.5f, 2.5f);
    // Origin di tengah sprite agar rotasi berputar di pusat
    this->player_shape.setOrigin(
        this->player_shape.getLocalBounds().width  / 2.f,
        this->player_shape.getLocalBounds().height / 2.f
    );
    this->player_shape.setRotation(90.f);            // sprite menghadap kanan
    this->player_shape.setPosition(100.f, this->window_height / 2.f); // sisi kiri tengah

    return this->player_shape;
}

// Kembalikan referensi sprite kapal
Sprite& Player::getShape() {
    return this->player_shape;
}

// Gerak ke atas satu langkah per frame; dibatasi tepi atas layar
void Player::moveUp() {
    float half_h = this->player_shape.getGlobalBounds().height / 2.f;
    if (this->player_shape.getPosition().y - half_h > 0.f) {
        this->player_shape.move(0.f, -3.f); // 3px per frame ke atas
    }
}

// Gerak ke bawah satu langkah per frame; dibatasi tepi bawah layar
void Player::moveDown() {
    float half_h = this->player_shape.getGlobalBounds().height / 2.f;
    if (this->player_shape.getPosition().y + half_h < this->window_height) {
        this->player_shape.move(0.f, 3.f);  // 3px per frame ke bawah
    }
}

// Spawn satu Bullet bila cooldown sudah selesai
// bulletTexPath: path sprite sheet bullet (96x32, 3 frame 32x32)
void Player::Attack(const string& bulletTexPath) {
    if (this->shoot_timer < this->shoot_cooldown) return; // belum waktunya
    if (this->ammo <= 0) return; // tidak bisa menembak bila peluru habis

    this->shoot_timer = 0; // reset cooldown
    this->ammo--; // kurangi peluru

    // Buat bullet baru; tiap Bullet alokasi internal_texture sendiri
    Bullet b(nullptr);
    // speed +7 (ke kanan), rotasi 90° (nozzle sprite atas → mengarah kanan)
    // fcount=3 karena bullet.png adalah sprite sheet 3 frame
    b.setShape(bulletTexPath, 7.f, 90.f, 3);
    b.getShape().setPosition(this->player_shape.getPosition()); // spawn di posisi player
    this->bullets.push_back(b);

    audio.playAudio("shoot");
}

// Update semua bullet: cooldown, gerak, animasi, bersihkan tidak aktif
// Dipanggil tiap frame; tidak pakai dt (kecepatan per frame tetap)
void Player::update(float window_width) {
    this->shoot_timer++; // hitung frame untuk cooldown tembakan

    if (this->ammo < this->max_ammo) {
        this->ammo_regen_timer++;
        if (this->ammo_regen_timer >= this->ammo_regen_interval) {
            this->ammo_regen_timer = 0;
            this->ammo++;
        }
    } else {
        this->ammo_regen_timer = 0;
    }

    // Loop semua bullet aktif
    for (auto& b : this->bullets) {
        b.move();                      // geser posisi
        b.updateAnimation();           // animasi frame
        b.checkBounds(window_width);   // nonaktifkan bila keluar layar
    }

    // Hapus bullet yang sudah tidak aktif dari vector
    this->bullets.erase(
        remove_if(this->bullets.begin(), this->bullets.end(),
            [](const Bullet& b) { return !b.isActive(); }),
        this->bullets.end()
    );
}

// Gambar kapal dan semua bullet ke window
void Player::draw(RenderWindow& window) {
    window.draw(this->player_shape); // gambar kapal

    // Gambar setiap bullet yang masih aktif
    for (auto& b : this->bullets) {
        if (b.isActive()) {
            window.draw(b.getShape());
        }
    }
}

// Kurangi HP (override takeDamage dari Ship)
void Player::takeDamage(int damage) {
    this->health_point -= damage;
    if (this->health_point < 0) {
        this->health_point = 0;
    }
}

// Getter untuk collision dan HUD di GameState
vector<Bullet>& Player::getBullets() { return this->bullets; }
FloatRect Player::getBounds() { return this->player_shape.getGlobalBounds(); }
Vector2f Player::getPosition() { return this->player_shape.getPosition(); }
int Player::getHealth() { return this->health_point; }
int Player::getAttack() { return this->attack_point; }
int Player::getAmmo() { return this->ammo; }
int Player::getMaxAmmo() { return this->max_ammo; }
bool Player::isAlive() { return this->health_point > 0; }
