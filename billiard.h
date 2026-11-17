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
    enum class Status { Menu, Aturan, Bermain };

    sf::RenderWindow window;
    Meja meja;
    Stik stik;
    PemainManusia pemain;
    sf::Clock jam;
    Status status = Status::Menu;
    sf::Font font;
    bool adaFont = false;

    // Tombol menu
    sf::FloatRect tombolMulai;
    sf::FloatRect tombolLanjut;
    sf::FloatRect tombolKembali;

    bool hoverTombol = false;
    bool hoverLanjut = false;
    bool hoverKembali = false;

    // Fungsi utama
    void prosesEvent();
    void update(float dt);
    void render();

    // Tampilan
    void renderMenu();
    void renderAturan();

    void mulaiGame();
};

