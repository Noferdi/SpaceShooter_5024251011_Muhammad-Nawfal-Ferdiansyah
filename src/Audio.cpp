#include "include/Audio.hpp"

using namespace std;
using namespace sf;

Audio audio;

Audio::Audio() {
}

bool Audio::setAudio(const string& filePath, const string& type) {
    if (type == "bgm") {
        if (!this->bgm.openFromFile(filePath)) {
            cerr << "Error: gagal muat BGM " << filePath << endl;
            return false;
        }
        this->bgm.setLoop(true);
        return true;
    } else if (type == "shoot") {
        if (!this->shoot_buffer.loadFromFile(filePath)) {
            cerr << "Error: gagal muat Shoot SFX " << filePath << endl;
            return false;
        }
        this->shoot_sound.setBuffer(this->shoot_buffer);
        return true;
    } else if (type == "gameover") {
        if (!this->gameover_buffer.loadFromFile(filePath)) {
            cerr << "Error: gagal muat GameOver SFX " << filePath << endl;
            return false;
        }
        this->gameover_sound.setBuffer(this->gameover_buffer);
        return true;
    } else {
        cerr << "Error: tipe audio tidak valid: " << type << endl;
        return false;
    }
}

void Audio::playAudio(const string& type) {
    if (type == "bgm") {
        if (this->bgm.getStatus() != Music::Playing) {
            this->bgm.play();
        }
    } else if (type == "shoot") {
        this->shoot_sound.play();
    } else if (type == "gameover") {
        this->gameover_sound.play();
    }
}

void Audio::stopAudio(const string& type) {
    if (type == "bgm") {
        this->bgm.stop();
    } else if (type == "shoot") {
        this->shoot_sound.stop();
    } else if (type == "gameover") {
        this->gameover_sound.stop();
    }
}
