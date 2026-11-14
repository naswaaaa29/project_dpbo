#include "lubang.h"
#include "bola.h"
#include <cmath>

bool Lubang::cekMasuk(const Bola& b) const {
    sf::Vector2f d = b.posisi() - pos;
    return std::sqrt(d.x * d.x + d.y * d.y) < jari;
}

void Lubang::gambar(sf::RenderWindow& window) const {
    sf::CircleShape c(jari);
    c.setOrigin(jari, jari);
    c.setPosition(pos);
    c.setFillColor(sf::Color(15, 15, 15));
    window.draw(c);
}
