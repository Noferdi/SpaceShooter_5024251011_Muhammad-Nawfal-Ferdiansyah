#include "include/GameState.hpp"
#include "include/Audio.hpp"

GameState::GameState(float win_w, float win_h)
        : State("Game"), player(100, 20)  {
    this->win_width      = win_w;
    this->win_height     = win_h;
    this->score          = 0;
    this->next_state     = "";
    this->spawn_timer    = 0;
    this->spawn_interval = 120; // spawn enemy tiap 120 frame

    audio.stopAudio("bgm");

    // Setup player di posisi awal
    this->player.setShape(PlayerTexPath, this->win_height);

    // Muat background kedua layer
    this->bg.load();

    // --- Setup teks HUD skor ---
    this->score_text.setFont(this->mono_font);
    this->score_text.setCharacterSize(22);
    this->score_text.setFillColor(Color::White);
    this->score_text.setPosition(10.f, 10.f);   // pojok kiri atas

    // --- Setup teks HUD HP ---
    this->health_text.setFont(this->mono_font);
    this->health_text.setCharacterSize(22);
    this->health_text.setFillColor(Color(80, 255, 80));  // hijau
    this->health_text.setPosition(10.f, 40.f);

    // --- Setup teks HUD ammo ---
    this->ammo_text.setFont(this->mono_font);
    this->ammo_text.setCharacterSize(22);
    this->ammo_text.setFillColor(Color(255, 255, 255)); // putih
    this->ammo_text.setPosition(10.f, 70.f);

    // --- Setup teks instruksi game ---
    this->instructions_text.setFont(this->mono_font);
    this->instructions_text.setCharacterSize(20);
    this->instructions_text.setFillColor(Color(200, 200, 200));
    this->instructions_text.setString("E = Menu, Q = Quit");
    this->instructions_text.setPosition(10.f, 100.f);
}

void GameState::update() {
    // --- Update background (kedua layer) ---
    this->bg.update();

    // --- Input keyboard untuk player (hanya atas/bawah) ---
    if (Keyboard::isKeyPressed(Keyboard::Up)   || Keyboard::isKeyPressed(Keyboard::W)) {
        this->player.moveUp();    // gerak ke atas 3px/frame
    }
    if (Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) {
        this->player.moveDown();  // gerak ke bawah 3px/frame
    }
    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        this->player.Attack(BulletTexPath); // coba spawn bullet
    }

    // Update semua bullet player (gerak, animasi, bersihkan tidak aktif)
    this->player.update(this->win_width);

    // --- Spawn enemy baru secara periodik ---
    this->spawn_timer++;
    if (this->spawn_timer >= this->spawn_interval) {
        this->spawn_timer = 0; // reset timer

        // Y random dengan margin 60px dari tepi atas dan bawah layar
        float margin = 60.f;
        float rand_y = margin + static_cast<float>(
            rand() % static_cast<int>(this->win_height - 2.f * margin)
        );

        // Spawn enemy di luar tepi kanan layar (tidak langsung terlihat)
        Enemy* e = new Enemy(30, 10, "Klaed");
        e->setShape(EnemyTexPath, this->win_width + 60.f, rand_y);
        this->enemies.push_back(e);
    }

    // --- Update setiap enemy + collision detection ---
    for (auto& e : this->enemies) {
        // Update gerak kapal, timer tembak, dan torpedo milik enemy
        if (e != nullptr) e->update(this->win_width, TorpedoTexPath);

        if (!e->isAlive()) continue; // enemy sudah mati, skip collision

        // Cek: bullet player mengenai badan enemy
        for (auto& b : this->player.getBullets()) {
            if (b.collision(e->getBounds())) {
                // collision() sudah menonaktifkan bullet secara internal
                e->takeDamage(this->player.getAttack()); // kurangi HP enemy
                if (!e->isAlive()) {
                    this->score += 100; // tambah skor per enemy hancur
                }
            }
        }

        // Cek: torpedo enemy mengenai badan player
        for (auto& b : e->getBullets()) {
            if (b.collision(this->player.getBounds())) {
                this->player.takeDamage(e->getAttack()); // kurangi HP player
            }
        }

        // Cek: badan enemy langsung menabrak badan player (ram)
        if (e->getBounds().intersects(this->player.getBounds())) {
            this->player.takeDamage(1); // damage kecil per frame saat ram
        }
    }

    // --- Bersihkan enemy mati atau yang sudah melewati tepi kiri ---
    // Hapus enemy yang mati atau melewati layar: delete pointer then erase
    this->enemies.erase(
        remove_if(this->enemies.begin(), this->enemies.end(),
            [](Enemy* e) {
                bool rem = (e == nullptr) || !e->isAlive() || e->isOffScreen();
                if (rem && e != nullptr) delete e;
                return rem;
            }),
        this->enemies.end()
    );

    // --- Perbarui teks HUD ---
    this->score_text.setString("Score: " + to_string(this->score));
    this->health_text.setString("HP: "   + to_string(this->player.getHealth()));
    this->ammo_text.setString("Ammo: " + to_string(this->player.getAmmo()) + "/" + to_string(this->player.getMaxAmmo()));

    // --- Cek kondisi game over ---
    if (!this->player.isAlive()) {
        this->next_state = "GameOver";
    } else if(Keyboard::isKeyPressed(Keyboard::E)) {
        this->next_state = "Menu"; // kembali ke menu jika Escape ditekan
    } else if (Keyboard::isKeyPressed(Keyboard::Q)) {
        this->next_state = "Quit"; // keluar dari game jika Escape ditekan
    }
}

GameState::~GameState() {
    for (auto e : this->enemies) {
        if (e != nullptr) delete e;
    }
    this->enemies.clear();
}

void GameState::render(RenderWindow& window) {
    this->bg.draw(window);            // background (paling belakang)
    this->player.draw(window);        // player dan bullet-nya
    for (auto& e : this->enemies) {
        e->draw(window);              // tiap enemy dan torpedo-nya
    }
    window.draw(this->score_text);    // HUD skor (paling depan)
    window.draw(this->health_text);   // HUD HP
    window.draw(this->ammo_text);     // HUD ammo
    window.draw(this->instructions_text); // instruksi menu / quit
}

string GameState::getNextState() {
    return this->next_state;
}

int GameState::getScore() {
    return this->score;
}

