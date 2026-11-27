#include "meja.h"
#include <cmath>

Meja::Meja(sf::Vector2f posisi, sf::Vector2f ukuran)
    : pos(posisi), ukuran(ukuran) {
    buatLubang();
}

sf::FloatRect Meja::areaMain() const {
    return sf::FloatRect(pos.x + tepi, pos.y + tepi,
                         ukuran.x - 2.f * tepi, ukuran.y - 2.f * tepi);
}

void Meja::buatLubang() {
    sf::FloatRect m = areaMain();
    float r = 20.f;
    float left = m.left, right = m.left + m.width;
    float top  = m.top,  bottom = m.top + m.height;
    float midX = m.left + m.width / 2.f;

    lubang.clear();
    lubang.emplace_back(sf::Vector2f(left,  top),    r);
    lubang.emplace_back(sf::Vector2f(midX,  top),    r);
    lubang.emplace_back(sf::Vector2f(right, top),    r);
    lubang.emplace_back(sf::Vector2f(left,  bottom), r);
    lubang.emplace_back(sf::Vector2f(midX,  bottom), r);
    lubang.emplace_back(sf::Vector2f(right, bottom), r);
}

void Meja::susunBola() {
    bola.clear();
    masukTerakhir.clear();
    sf::FloatRect m = areaMain();
    float cy = m.top + m.height / 2.f;
    float r = 11.f;

    // bola putih di area break (kiri)
    bola.emplace_back(0, Jenis::putih,
                      sf::Vector2f(m.left + m.width * 0.25f, cy));

    // 15 bola, apex menghadap bola putih; no.8 di tengah
    int urutan[15] = { 1, 9, 2, 10, 8, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15 };
    float apexX = m.left + m.width * 0.70f;
    float dx = r * std::sqrt(3.f);  // jarak antar baris
    float dy = 2.f * r;             // jarak antar bola dalam satu baris

    int idx = 0;
    for (int baris = 0; baris < 5; ++baris) {
        float x = apexX + baris * dx;
        for (int j = 0; j <= baris; ++j) {
            float y = cy + (j - baris / 2.f) * dy;
            int nomor = urutan[idx++];
            Jenis jn = (nomor == 8) ? Jenis::hitam
                     : (nomor <  8) ? Jenis::polos
                                    : Jenis::strip;
            bola.emplace_back(nomor, jn, sf::Vector2f(x, y));
        }
    }
}

void Meja::update(float dt) {
    const float langkahMaks = 1.f / 120.f;
    int jumlahLangkah = (int)std::ceil(dt / langkahMaks);
    if (jumlahLangkah < 1) jumlahLangkah = 1;
    float h = dt / jumlahLangkah;

    for (int i = 0; i < jumlahLangkah; ++i) {
        for (auto& b : bola) b.gerak(h);
        for (auto& b : bola) b.cekDinding(areaMain());
        cekTabrakanBola();
        cekLubang();
    }
}

void Meja::cekTabrakanBola() {
    for (size_t i = 0; i < bola.size(); ++i)
        for (size_t j = i + 1; j < bola.size(); ++j)
            bola[i].tabrak(bola[j]);
}

void Meja::cekLubang() {
    for (auto& b : bola) {
        if (!b.diMeja()) continue;
        for (const auto& l : lubang) {
            if (l.cekBolaMasuk(b)) {
                if (b.jenis() == Jenis::putih) {
                    b.kembaliKeMeja(posisiAwalPutih());
                } else {
                    masukTerakhir.push_back(b.nomor());
                    b.masukkan();
                }
                break;
            }
        }
    }
}

sf::Vector2f Meja::posisiAwalPutih() const {
    sf::FloatRect m = areaMain();
    float cy = m.top + m.height / 2.f;
    return {m.left + m.width * 0.25f, cy};
}

std::vector<int> Meja::ambilBolaMasuk() {
    std::vector<int> hasil = masukTerakhir;
    masukTerakhir.clear();
    return hasil;
}

bool Meja::semuaBolaBerhenti() const{
    for (const auto& b : bola){
        if (b.bergerak()) return false;
    }
    return true;
}

Bola* Meja::bolaPutih() {
    for (auto& b : bola)
        if (b.jenis() == Jenis::putih) return &b;
    return nullptr;
}

void Meja::gambar(sf::RenderWindow& window) const {
    sf::RectangleShape bingkai(ukuran); // kayu
    bingkai.setPosition(pos);
    bingkai.setFillColor(sf::Color(101, 67, 33));
    window.draw(bingkai);

    sf::FloatRect m = areaMain(); // kain hijau
    sf::RectangleShape kain({m.width, m.height});
    kain.setPosition(m.left, m.top);
    kain.setFillColor(sf::Color(39, 119, 61));
    window.draw(kain);

    for (const auto& l : lubang) l.gambar(window);
    for (const auto& b : bola)   b.gambar(window);
}
