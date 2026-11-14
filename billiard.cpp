#include "billiard.h"

Billiard::Billiard()
    : window(sf::VideoMode(1000, 600), "Billiard - Kelompok 01"),
      meja({50.f, 75.f}, {900.f, 450.f}) {
    window.setFramerateLimit(60);
    meja.susunBola();
}

void Billiard::jalan() {
    while (window.isOpen()) {
        prosesEvent();
        render();
    }
}

void Billiard::prosesEvent() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Billiard::render() {
    window.clear(sf::Color(40, 42, 48));  
    meja.gambar(window);
    window.display();
}

