#include "include/MenuState.hpp"
#include "include/Audio.hpp"

MenuState::MenuState() : State("Menu"), next_state(""), blink_timer(0), blink_visible(true) {
    // Muat background (hanya layer void yang dipakai di menu)
    this->bg.load();

    audio.playAudio("bgm");

    // --- Teks judul ---
    this->title_text.setFont(this->mono_font);
    this->title_text.setString("SPACE SHOOTER");
    this->title_text.setCharacterSize(50);
    this->title_text.setFillColor(Color(0, 220, 255)); // biru cyan
    // Pusatkan teks secara horizontal di layar 960px
    FloatRect tb = this->title_text.getLocalBounds();
    this->title_text.setOrigin(tb.width / 2.f, tb.height / 2.f);
    this->title_text.setPosition(480.f, 190.f);

    // --- Teks start (akan berkedip) ---
    this->start_text.setFont(this->mono_font);
    this->start_text.setString("Press ENTER to Start");
    this->start_text.setCharacterSize(28);
    this->start_text.setFillColor(Color::White);
    FloatRect sb = this->start_text.getLocalBounds();
    this->start_text.setOrigin(sb.width / 2.f, sb.height / 2.f);
    this->start_text.setPosition(480.f, 320.f);

    // --- Teks quit ---
    this->quit_text.setFont(this->mono_font);
    this->quit_text.setString("Press Q to Quit");
    this->quit_text.setCharacterSize(22);
    this->quit_text.setFillColor(Color(160, 160, 160)); // abu-abu
    FloatRect qb = this->quit_text.getLocalBounds();
    this->quit_text.setOrigin(qb.width / 2.f, qb.height / 2.f);
    this->quit_text.setPosition(480.f, 400.f);
}

void MenuState::update() {
    // Scroll background layer void
    this->bg.updateMenuLayer();

    // Efek kedip berdasarkan frame count
    this->blink_timer++;
    if (this->blink_timer >= 30) {
        this->blink_timer   = 0;
        this->blink_visible = !this->blink_visible;
    }

    this->start_text.setFillColor(
        this->blink_visible ? Color::White : Color::Transparent
    );

    // Cek input keyboard untuk transisi atau keluar
    if (Keyboard::isKeyPressed(Keyboard::Return)) {
        this->next_state = "Game"; // mulai permainan
    }
    if (Keyboard::isKeyPressed(Keyboard::Q)) {
        this->next_state = "Quit"; // sinyal untuk tutup window di Game::run
    }
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        this->next_state = "Menu"; // Balik ke menu
    }
}

void MenuState::render(RenderWindow& window) {
    this->bg.drawMenuLayer(window); // background layer void
    window.draw(this->title_text);
    window.draw(this->start_text);
    window.draw(this->quit_text);
}

string MenuState::getNextState() {
    return this->next_state;
}
