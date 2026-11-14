#include "pemain.h"
#include "meja.h"
#include "bola.h"
#include <SFML/Graphics.hpp>

bool PemainManusia::ambilTembakan(const Meja&, const sf::RenderWindow&, Tembakan&) {
    // TODO buat minggu ke-3 -> baca mouse untuk arah dan tombol kiri buat kekuatan.
    // Mengembalikan true hanya saat tombol dilepas.
    return false;
}

bool PemainBot::ambilTembakan(const Meja&, const sf::RenderWindow&, Tembakan&) {
    // TODO buat minggu ke-6: pilih bola target, hitung sudut dan kekuatan tp diacakin dikit.
    return false;
}

const Bola* PemainBot::pilihTarget(const Meja&) const {
    // TODO buat minggu ke-6: pilih bola kelompoknya yang paling dekat sama bola putih.
    return nullptr;
}