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
    sf::Clock jam; //untuk hitung dt

    void prosesEvent();
    void update(float dt); 
    void render();
};
