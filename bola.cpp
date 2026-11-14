#include "bola.h"

Bola::Bola(int nomor, Jenis jenis, sf::Vector2f posisi)
    : nomorBola(nomor), jenisBola(jenis), pos(posisi) {}

sf::Color Bola::warnaDasar() const {
    switch (nomorBola % 8) {
        case 1:  return sf::Color(240, 200, 0);   // kuning
        case 2:  return sf::Color(0, 70, 200);    // biru
        case 3:  return sf::Color(210, 40, 40);   // merah
        case 4:  return sf::Color(120, 40, 160);  // ungu
        case 5:  return sf::Color(230, 120, 20);  // oranye
        case 6:  return sf::Color(20, 140, 70);   // hijau
        case 7:  return sf::Color(130, 30, 30);   // marun
        default: return sf::Color(240, 240, 240);
    }
}

void Bola::gerak(float dt) {
    // TODO minggu ke-3+ : pos += vel*dt, gesekan, pantulan dinding
    if (!aktif) return;
    pos += vel * dt;
}

void Bola::gambar(sf::RenderWindow& window) const {
    if (!aktif) return;

    sf::CircleShape c(jari);
    c.setOrigin(jari, jari);
    c.setPosition(pos);

    if (jenisBola == Jenis::putih) {
        c.setFillColor(sf::Color(245, 245, 245));
        window.draw(c);
    } else if (jenisBola == Jenis::hitam) {
        c.setFillColor(sf::Color(20, 20, 20));
        window.draw(c);
    } else if (jenisBola == Jenis::polos) {
        c.setFillColor(warnaDasar());
        window.draw(c);
    } else { // strip: bola putih dengan pita warna di tengah
        c.setFillColor(sf::Color(245, 245, 245));
        window.draw(c);

        sf::RectangleShape pita({1.7f * jari, jari});
        pita.setOrigin(0.85f * jari, jari / 2.f);
        pita.setPosition(pos);
        pita.setFillColor(warnaDasar());
        window.draw(pita);
    }
}
