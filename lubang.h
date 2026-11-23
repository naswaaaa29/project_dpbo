#pragma once
#include <SFML/Graphics.hpp>
#include "bola.h"

class Lubang {
public:
    Lubang(sf::Vector2f posisi, float radius);

    void gambar(sf::RenderWindow& window) const;
    bool cekBolaMasuk(const Bola& bola) const;

    sf::Vector2f posisi() const { return pos; }
    float radius() const { return jari; }

private:
    sf::Vector2f pos;
    float jari;
};
