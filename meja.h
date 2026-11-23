#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "bola.h"
#include "lubang.h"

class Meja {
public:
    Meja(sf::Vector2f posisi, sf::Vector2f ukuran);

    void susunBola();
    void update(float dt);                 // minggu ke-3+ (gerak/gesekan/tabrakan)
    void gambar(sf::RenderWindow& window) const;

    std::vector<Bola>&       daftarBola()       { return bola; }
    const std::vector<Bola>& daftarBola() const { return bola; }
    Bola* bolaPutih();
    bool semuaBolaBerhenti() const; //true jika ga ada bola yang bergerak

    // Nomor bola yang masuk lubang sejak panggilan terakhir.
    // Dipakai nanti oleh Aturan / Billiard untuk skor dan giliran.
    std::vector<int> ambilBolaMasuk();

    sf::FloatRect areaMain() const;        // area hijau (dalam bantalan)

private:
    sf::Vector2f pos;        // pojok kiri-atas meja (termasuk bingkai)
    sf::Vector2f ukuran;     // ukuran luar meja
    float tepi = 35.f;       // tebal bingkai/bantalan

    std::vector<Bola>   bola;
    std::vector<Lubang> lubang;
    std::vector<int>    masukTerakhir; // nomor bola yang baru masuk lubang

    void buatLubang();
    void cekLubang();
    sf::Vector2f posisiAwalPutih() const;
};
