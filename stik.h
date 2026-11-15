#pragma once
#include <SFML/Graphics.hpp>
#include "jenis.h"

class Bola;

class Stik {
    private:
        Tembakan bidik{0.f, 0.f};
        float lajuMaks = 900.f; //kecepatan bola klo misalkan 100%

    public:
        void aturBidik(const Tembakan& t) {
            bidik = t; 
        } //nyimpen sudut sama kekuatan
        
        void pukul(Bola& putih); // ngasih kecepatan ke bola putih

        void gambar(sf::RenderWindow& window, const Bola& putih) const;
};
