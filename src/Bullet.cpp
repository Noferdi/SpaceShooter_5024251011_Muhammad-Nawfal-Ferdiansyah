#include "include/Bullet.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

Bullet::Bullet(Texture* texture_ptr) {
    this->texture        = texture_ptr;
    this->active         = false;
    this->frame_count    = 3;
    this->frame_width    = 32;
    this->frame_height   = 32;
    this->current_frame  = 0;
    this->frame_timer    = 0;
    this->frame_interval = 5;
    this->speed          = 7.f;
}

Bullet::Bullet(const Bullet& other) {
    this->texture        = other.texture;
    this->active         = other.active;
    this->frame_count    = other.frame_count;
    this->frame_width    = other.frame_width;
    this->frame_height   = other.frame_height;
    this->current_frame  = other.current_frame;
    this->frame_timer    = other.frame_timer;
    this->frame_interval = other.frame_interval;
    this->speed          = other.speed;
    this->texture_path   = other.texture_path;

    if (!this->texture_path.empty()) {
        this->internal_texture.loadFromFile(this->texture_path);
    }

    this->bullet_shape = other.bullet_shape;
    this->bullet_shape.setTexture(this->internal_texture, false);
}

Bullet& Bullet::operator=(const Bullet& other) {
    if (this == &other) return *this;

    this->texture        = other.texture;
    this->active         = other.active;
    this->frame_count    = other.frame_count;
    this->frame_width    = other.frame_width;
    this->frame_height   = other.frame_height;
    this->current_frame  = other.current_frame;
    this->frame_timer    = other.frame_timer;
    this->frame_interval = other.frame_interval;
    this->speed          = other.speed;
    this->texture_path   = other.texture_path;

    if (!this->texture_path.empty()) {
        this->internal_texture.loadFromFile(this->texture_path);
    }

    this->bullet_shape = other.bullet_shape;
    this->bullet_shape.setTexture(this->internal_texture, false);

    return *this;
}

Sprite& Bullet::setShape(const string& texturePath, float spd, float rotation,
                         int   fcount, int   fw, int   fh) {
    // Inisialisasi tampilan bullet saat di-spawn:
    //   texturePath : path file gambar
    //   spd         : kecepatan (+7 ke kanan untuk player, -5 ke kiri untuk enemy)
    //   rotation    : 90° untuk player (nozzle atas→kanan), 270° untuk enemy (nozzle atas→kiri)
    //   fcount      : jumlah frame animasi (selalu 3 untuk bullet.png dan Torpedo.png)
    //   fw          : lebar satu frame pixel (32 untuk player bullet, 11 untuk torpedo)
    //   fh          : tinggi satu frame pixel (32 untuk keduanya)
    this->bullet_shape   = Sprite();
    this->texture_path   = texturePath;
    this->frame_count    = fcount;
    this->frame_width    = fw;
    this->frame_height   = fh;
    this->current_frame  = 0;
    this->frame_timer    = 0;
    this->speed          = spd;
    this->active         = true;

    if (this->internal_texture.loadFromFile(texturePath)) {
        this->bullet_shape.setTexture(this->internal_texture);
        this->bullet_shape.setTextureRect(
            IntRect(0, 0, this->frame_width, this->frame_height)
        );
    }

    FloatRect bounds = this->bullet_shape.getLocalBounds();
    this->bullet_shape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    this->bullet_shape.setScale(2.f, 2.f);
    this->bullet_shape.setRotation(rotation);

    return this->bullet_shape;
}

Sprite& Bullet::getShape() {
    return this->bullet_shape;
}

// Gerakkan bullet sejauh speed px per frame (tanpa dt)
void Bullet::move() {
    if (!this->active) return;
    this->bullet_shape.move(this->speed, 0.f);
}

// Maju animasi ke frame berikutnya setiap frame_interval frame game
void Bullet::updateAnimation() {
    if (!this->active)          return;
    if (this->frame_count <= 1) return;

    this->frame_timer++;
    if (this->frame_timer >= this->frame_interval) {
        this->frame_timer   = 0;
        this->current_frame = (this->current_frame + 1) % this->frame_count;
        this->bullet_shape.setTextureRect(
            IntRect(this->current_frame * this->frame_width, 0,
                    this->frame_width, this->frame_height)
        );
    }
}

// Nonaktifkan bullet bila sudah keluar batas kiri atau kanan layar
void Bullet::checkBounds(float window_width) {
    float x = this->bullet_shape.getPosition().x;
    if (x < -80.f || x > window_width + 80.f) {
        this->active = false;
    }
}

bool Bullet::collision(const Sprite& other) {
    bool hit = this->bullet_shape.getGlobalBounds()
                   .intersects(other.getGlobalBounds());
    if (hit) {
        this->bullet_shape.setPosition(-200.f, -200.f);
        this->active = false;
        return true;
    }
    return false;
}

bool Bullet::collision(const FloatRect& other_bounds) {
    bool hit = this->bullet_shape.getGlobalBounds().intersects(other_bounds);
    if (hit) {
        this->bullet_shape.setPosition(-200.f, -200.f);
        this->active = false;
        return true;
    }
    return false;
}

// Cek tumbukan dengan Sprite lain; nonaktifkan dan pindah off-screen bila kena
// Overload: cek tumbukan dengan FloatRect (dari getBounds() enemy/player)

bool Bullet::isActive() const { return this->active; }

// Getter status dan geometri untuk GameState
FloatRect Bullet::getBounds() const { return this->bullet_shape.getGlobalBounds(); }

Vector2f Bullet::getPosition() const { return this->bullet_shape.getPosition(); }
