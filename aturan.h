#pragma once
#include <vector>
#include "jenis.h"

class Bola;
class Meja;
class Pemain;

enum class HasilGiliran {
    LanjutMain,
    GantiGiliran,
    PelanggaranGantiGiliran,
    PenembakMenang,
    PenembakKalah
};

class Aturan {
    private:
        static Kelompok kelompokDari(Jenis j);
        int sisaBolaKelompok(const Pemain& p, const Meja& meja) const;

        std::vector<Jenis> masukTembakanIni;   // jenis bola apa yg masuk pada tembakan saat ini
        
    public:
        void bolaMasuk(const Bola& b);

        HasilGiliran evaluasi(Pemain& penembak, Pemain& lawan, const Meja& meja);

        void reset();
};