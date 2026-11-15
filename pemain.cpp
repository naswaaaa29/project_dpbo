#include "pemain.h"
#include "meja.h"
#include "bola.h"
#include <SFML/Graphics.hpp>
#include <cmath>

bool PemainManusia::ambilTembakan(const Meja& meja, const sf::RenderWindow& window, Tembakan& hasil) {
    const Bola* putih = nullptr;
    for (const auto& b : meja.daftarBola())
        if (b.jenis() == Jenis::putih) { putih = &b; break; }

    if (!putih || !putih->diMeja()) {
        menahan = false;
        kekuatan = 0.f;
        return false;
    }

    sf::Vector2f p = putih->posisi();
    sf::Vector2f m = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    float sudut = std::atan2(m.y - p.y, m.x - p.x);

    bool tekan = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    if (tekan) {
        if (!menahan) {          
            menahan = true;
            kekuatan = 0.f;
            jamIsi.restart();
        }

        kekuatan = jamIsi.getElapsedTime().asSeconds() / 1.2f;
        if (kekuatan > 1.f) kekuatan = 1.f;

        bidik = { sudut, kekuatan }; 
        return false;                  
    }

    bidik = { sudut, 0.f };         

    if (menahan) {                    
        menahan = false;
        float k = kekuatan;
        kekuatan = 0.f;
        if (k > 0.03f) {             
            hasil = { sudut, k };
            return true;
        }
    }
    return false;
}

bool PemainBot::ambilTembakan(const Meja&, const sf::RenderWindow&, Tembakan&) {
    return false;
}

const Bola* PemainBot::pilihTarget(const Meja&) const {
    return nullptr;
}