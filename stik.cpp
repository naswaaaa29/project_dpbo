#include "stik.h"
#include "bola.h"
#include <cmath>

void Stik::pukul(Bola& putih) {
    float laju = bidik.kekuatan * lajuMaks;
    sf::Vector2f v(std::cos(bidik.sudut) * laju,
                   std::sin(bidik.sudut) * laju);
    putih.setKecepatan(v);
}

void Stik::gambar(sf::RenderWindow& window, const Bola& putih) const {
    if (!putih.diMeja()) return;

    sf::Vector2f p = putih.posisi();
    float a = bidik.sudut;
    sf::Vector2f dir(std::cos(a), std::sin(a)); // arah tembak 
    float r = putih.radius();

    const float panjangGaris = 260.f;
    const int   n = 20;
    for (int i = 1; i <= n; ++i) {
        if (i % 2 == 0) continue; //efek putus2
        float t0 = (float)i / n;
        sf::Vector2f titik = p + dir * (r + t0 * panjangGaris);
        sf::CircleShape dot(2.f);
        dot.setOrigin(2.f, 2.f);
        dot.setPosition(titik);
        dot.setFillColor(sf::Color(255, 255, 255, 160));
        window.draw(dot);
    }

    float gap         = r + 6.f + bidik.kekuatan * 60.f;  
    float panjangStik = 170.f;
    sf::Vector2f belakang(-dir.x, -dir.y);
    sf::Vector2f pangkal = p + belakang * gap;           
    float deg = a * 180.f / 3.14159265f + 180.f;          

    sf::RectangleShape batang(sf::Vector2f(panjangStik, 6.f));
    batang.setOrigin(0.f, 3.f);
    batang.setPosition(pangkal);
    batang.setRotation(deg);
    batang.setFillColor(sf::Color(190, 140, 70));
    window.draw(batang);

    sf::RectangleShape tip(sf::Vector2f(10.f, 6.f));      
    tip.setOrigin(0.f, 3.f);
    tip.setPosition(pangkal);
    tip.setRotation(deg);
    tip.setFillColor(sf::Color(90, 120, 200));
    window.draw(tip);

    if (bidik.kekuatan > 0.f) {
        float w = 60.f, h = 7.f;
        sf::Vector2f barPos(p.x - w / 2.f, p.y - r - 20.f);

        sf::RectangleShape bg(sf::Vector2f(w, h));
        bg.setPosition(barPos);
        bg.setFillColor(sf::Color(0, 0, 0, 120));
        window.draw(bg);

        sf::RectangleShape isi(sf::Vector2f(w * bidik.kekuatan, h));
        isi.setPosition(barPos);
        sf::Uint8 merah = (sf::Uint8)(255 * bidik.kekuatan);
        sf::Uint8 hijau = (sf::Uint8)(255 * (1.f - bidik.kekuatan));
        isi.setFillColor(sf::Color(merah, hijau, 40));
        window.draw(isi);
    }
}