#pragma once
#include <SFML/Graphics.hpp>
#include "jenis.h"

class Bola {
public:
    Bola(int nomor, Jenis jenis, sf::Vector2f posisi);

    void gambar(sf::RenderWindow& window) const;
    void gerak(float dt);

    Jenis jenis() const            { return jenisBola; }
    int   nomor() const            { return nomorBola; }
    sf::Vector2f posisi() const    { return pos; }
    sf::Vector2f kecepatan() const { return vel; }
    float radius() const           { return jari; }
    bool  diMeja() const           { return aktif; }
    bool bergerak() const          { return aktif && (vel.x != 0.f || vel.y != 0.f);}

    void setPosisi(sf::Vector2f p)    { pos = p; }
    void setKecepatan(sf::Vector2f v) { vel = v; }
    void masukkan()                   { aktif = false; vel = {0.f, 0.f}; }
    void kembaliKeMeja(sf::Vector2f p) { pos = p; vel = {0.f, 0.f}; aktif = true; }

private:
    int   nomorBola;
    Jenis jenisBola;
    sf::Vector2f pos;
    sf::Vector2f vel{0.f, 0.f};
    float jari = 11.f;
    bool  aktif = true;

    //Untuk gesekan bolanya
    float perlambatan = 350.f;
    float lajuBerhenti = 8.f;

    sf::Color warnaDasar() const;
};
