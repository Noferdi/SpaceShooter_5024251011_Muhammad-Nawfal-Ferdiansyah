#pragma once

#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

class Audio {
    private:
    Music bgm;                   // background music (loop)
    SoundBuffer shoot_buffer;    // buffer untuk efek tembakan
    Sound shoot_sound;           // efek tembakan (play sekali tiap tembak)
    SoundBuffer gameover_buffer; // buffer untuk efek game over
    Sound gameover_sound;        // efek game over (play sekali saat game over)

    public:
    Audio();

    bool setAudio(const string& filePath, const string& type);
    void playAudio(const string& type);
    void stopAudio(const string& type);
};

extern Audio audio;