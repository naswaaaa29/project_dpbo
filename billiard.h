#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "aturan.h"
#include "meja.h"
#include "stik.h"
#include "pemain.h"

class Billiard {
public:
    Billiard();
    void jalan();

private:
    enum class Status { Menu, Aturan, Bermain, Jeda, KonfirmasiKeluar };

    sf::RenderWindow window;
    Meja meja;
    Stik stik;
    PemainManusia pemain;
    PemainManusia pemain2;
    Pemain* aktif = nullptr;
    Aturan aturan;

    sf::Clock jam;
    Status status = Status::Menu;
    sf::Font font;
    bool adaFont = false;

    // Tombol menu
    sf::FloatRect tombolMulai;
    sf::FloatRect tombolLanjut;
    sf::FloatRect tombolKembali;

    //tombol jeda dan keluar
    sf::FloatRect tombolJeda, tombolLanjutkan, tombolKeluarJeda, tombolYa, tombolTidak;
    bool hoverJeda = false, hoverLanjutkan = false, hoverKeluarJeda = false, hoverYa = false, hoverTidak = false;

    //kelompok bola
    bool tembakanBerjalan = false;
    std::string notifTeks;
    float notifSisa = 0.f;

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

    void renderJeda();
    void renderKonfirmasi();
    void bukaJeda();
    void lanjutDariJeda();
    void keluarKeMenu();
    void selesaiTembakan();
    void gantiGiliran();
    void gambarPanelKelompok();
    void gambarTombolJeda();
    void gambarNotif();

    void mulaiGame();
};

