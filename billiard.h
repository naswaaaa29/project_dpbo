#pragma once
#include <SFML/Graphics.hpp>
#include "meja.h"

class Billiard {
public:
    Billiard();
    void jalan();

private:
    sf::RenderWindow window;
    Meja meja;

    void prosesEvent();
    void render();
};
