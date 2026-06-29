#include "include/Backgrounds.hpp"
#include <iostream>

using namespace std;
using namespace sf;

// Definisi konstanta statis; kecepatan dalam px/frame
const float Backgrounds::SCALE       = 1.5f;
const float Backgrounds::SPEED_VOID  = 0.5f;  
const float Backgrounds::SPEED_STARS = 1.2f; 

Backgrounds::Backgrounds() {
    this->scroll_void  = 0.f;
    this->scroll_stars = 0.f;
    this->loaded       = false;
}

// Muat kedua texture dan setup posisi awal kedua copy per layer
bool Backgrounds::load() {
    if (!this->tex_void.loadFromFile(
            "img/Backgrounds/Starry background  - Layer 01 - Void.png")) {
        cerr << "Error: gagal muat Layer 01 Void" << endl;
        return false;
    }

    if (!this->tex_stars.loadFromFile(
            "img/Backgrounds/Starry background  - Layer 02 - Stars.png")) {
        cerr << "Error: gagal muat Layer 02 Stars" << endl;
        return false;
    }

    this->layer1a.setTexture(this->tex_void);
    this->layer1a.setScale(SCALE, SCALE);
    this->layer1a.setPosition(0.f, 0.f);

    this->layer1b.setTexture(this->tex_void);
    this->layer1b.setScale(SCALE, SCALE);
    this->layer1b.setPosition(this->layer1a.getGlobalBounds().width, 0.f);

    this->layer2a.setTexture(this->tex_stars);
    this->layer2a.setScale(SCALE, SCALE);
    this->layer2a.setPosition(0.f, 0.f);

    this->layer2b.setTexture(this->tex_stars);
    this->layer2b.setScale(SCALE, SCALE);
    this->layer2b.setPosition(this->layer2a.getGlobalBounds().width, 0.f);

    this->loaded = true;
    return true;
}

// Update scroll Layer 01 saja (dipakai Menu dan GameOver)
// Dipanggil tiap frame; kecepatan dalam px/frame 
void Backgrounds::updateMenuLayer() {
    float w1 = this->layer1a.getGlobalBounds().width;

    this->scroll_void -= SPEED_VOID;
    if (this->scroll_void <= -w1) {
        this->scroll_void += w1;
    }
    this->layer1a.setPosition(this->scroll_void,      0.f);
    this->layer1b.setPosition(this->scroll_void + w1, 0.f);
}

// Update scroll kedua layer (dipakai GameState setiap frame)
void Backgrounds::update() {
    float w1 = this->layer1a.getGlobalBounds().width;
    float w2 = this->layer2a.getGlobalBounds().width;

    this->scroll_void -= SPEED_VOID;
    if (this->scroll_void <= -w1) {
        this->scroll_void += w1;
    }
    this->layer1a.setPosition(this->scroll_void,      0.f);
    this->layer1b.setPosition(this->scroll_void + w1, 0.f);

    this->scroll_stars -= SPEED_STARS;
    if (this->scroll_stars <= -w2) {
        this->scroll_stars += w2;
    }
    this->layer2a.setPosition(this->scroll_stars,      0.f);
    this->layer2b.setPosition(this->scroll_stars + w2, 0.f);
}
// Gambar hanya Layer 01 (untuk Menu dan GameOver)
void Backgrounds::drawMenuLayer(RenderWindow& window) {
    window.draw(this->layer1a);
    window.draw(this->layer1b);
}

// Gambar kedua layer (untuk GameState)
void Backgrounds::draw(RenderWindow& window) {
    window.draw(this->layer1a);
    window.draw(this->layer1b);
    window.draw(this->layer2a);
    window.draw(this->layer2b);
}

// true bila kedua texture berhasil dimuat
bool Backgrounds::isLoaded() { return this->loaded; }
