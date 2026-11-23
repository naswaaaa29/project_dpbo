#include "bola.h"
#include <cmath>

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
    // TODO minggu ke-4 : tabraan bola dengan dinding
    if (!aktif) return;
    
    //1. Mindahin bola sesuai kecepatan saat ini
    pos += vel * dt;

    //2. Laju berkurang setiap detik, arah geraknya ga berubah
    float laju = std::sqrt(vel.x * vel.x + vel.y * vel.y);
    if (laju == 0.f) return;

    float lajuBaru = laju - perlambatan * dt;
    if (lajuBaru < lajuBerhenti){
        vel = {0.f, 0.f};
    } else{
        vel = vel * (lajuBaru / laju); //bikin laju lebih kecil
    }
}

bool Bola::tabrak(Bola& lain) {
    if (!aktif || !lain.aktif) return false;

    //Apakah kedua bola bersentuhan
    sf::Vector2f d = lain.pos - pos;
    float jarak = std::sqrt(d.x * d.x + d.y * d.y);
    float jarakMin = jari + lain.jari;
    if (jarak >= jarakMin) return false;

    //Arah tumbukan
    sf::Vector2f n(1.f, 0.f);
    if (jarak > 0.0001f) n = d * (1.f / jarak);

    //Pisahin bola yang saling tumpang tindih, masing-masing setengahnya
    float tumpang = jarakMin - jarak;
    pos -= n * (tumpang / 2.f);
    lain.pos += n * (tumpang / 2.f);

    //Seberapa cepat keduanya saling mendekat
    float vRel = (vel.x - lain.vel.x) * n.x + (vel.y - lain.vel.y) * n.y;
    if (vRel <= 0.f) return false;

    //Pertukaran kecepatan searah n
    float impuls = vRel * (1.f + restitusi) / 2.f;
    vel -= n * impuls;
    lain.vel += n * impuls;
    return true;
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
