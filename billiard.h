#pragma once
#include <SFML/Graphics.hpp>
#include "meja.h"
#include "stik.h"
#include "pemain.h"

class Billiard {
public:
    Billiard();
    void jalan();

private:
    enum class Status { Menu, Bermain };

    sf::RenderWindow window;
    Meja meja;
    Stik stik;
    PemainManusia pemain;
    sf::Clock jam; 

    Status status = Status::Menu;
    sf::Font font;
    bool adaFont = false;
    sf::FloatRect tombolMulai;   
    bool hoverTombol = false;

    void prosesEvent();
    void update(float dt);
    void render();

    // layar menu
    void renderMenu();
    void mulaiGame();
};