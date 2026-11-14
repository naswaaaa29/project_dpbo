#pragma once
#include <SFML/Graphics.hpp>

class Bola;

class Lubang {
public:
    Lubang(sf::Vector2f posisi, float radius) : pos(posisi), jari(radius) {}

    bool cekMasuk(const Bola& b) const;
    void gambar(sf::RenderWindow& window) const;

    sf::Vector2f posisi() const { return pos; }
    float radius() const        { return jari; }

private:
    sf::Vector2f pos;
    float jari;
};
