#include "include/State.hpp"

State::State(string stateName) {
    this->stateName = stateName;
    // Font VCR OSD Mono dipakai oleh semua teks di semua state
    if (!this->mono_font.loadFromFile("VCR_OSD_MONO_1.001.ttf")) {
        cerr << "Error: gagal muat VCR_OSD_MONO_1.001.ttf" << endl;
    }
}

State::~State() {}
