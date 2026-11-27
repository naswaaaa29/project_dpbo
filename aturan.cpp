#include "aturan.h"
#include "pemain.h"
#include "meja.h"
#include "bola.h"

void Aturan::bolaMasuk(const Bola& b) {
    masukTembakanIni.push_back(b.jenis());
}

void Aturan::reset() {
    masukTembakanIni.clear();
}

Kelompok Aturan::kelompokDari(Jenis j) {
    if (j == Jenis::polos) return Kelompok::polos;
    if (j == Jenis::strip) return Kelompok::strip;
    return Kelompok::belum;
}

int Aturan::sisaBolaKelompok(const Pemain&, const Meja&) const {
    // TODO minggu ke-5 : Kondisi menang atau kalah
    return 0;
}

HasilGiliran Aturan::evaluasi(Pemain& penembak, Pemain& lawan, const Meja&) {
    std::vector<Jenis> masuk = masukTembakanIni;
    masukTembakanIni.clear();

    //Penentuan kelompok
    if (penembak.kelompok() == Kelompok::belum){
        for (Jenis j : masuk) {
            Kelompok k = kelompokDari(j);

            if (k != Kelompok::belum){
                penembak.setKelompok(k);
                lawan.setKelompok(k == Kelompok::polos ? Kelompok::strip : Kelompok::polos);
                break;
            }
        }
    }

    //Catat jumlah bola masuk
    bool bolaSendiriMasuk = false;
    for (Jenis j : masuk) {
        Kelompok k = kelompokDari(j);
        if (k == Kelompok::belum) continue;
        if (k == penembak.kelompok()) { penembak.tambahMasuk(); bolaSendiriMasuk = true; }

        else if (k == lawan.kelompok()) lawan.tambahMasuk();
    }

     // TODO minggu ke-5 : Pelanggaran, pergantian giliran, menang/kalah

    return bolaSendiriMasuk ? HasilGiliran::LanjutMain : HasilGiliran::GantiGiliran;
}