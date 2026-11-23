#include "lubang.h"
#include <cmath>

Lubang::Lubang(sf::Vector2f posisi, float radius)
    : pos(posisi), jari(radius) {}

void Lubang::gambar(sf::RenderWindow& window) const {
    sf::CircleShape c(jari);
    c.setOrigin(jari, jari);
    c.setPosition(pos);
    c.setFillColor(sf::Color(0, 0, 0));
    window.draw(c);
}

bool Lubang::cekBolaMasuk(const Bola& bola) const {
    if (!bola.diMeja()) return false;
    sf::Vector2f d = bola.posisi() - pos;
    float jarak = std::sqrt(d.x * d.x + d.y * d.y);
    return jarak < jari;
}
