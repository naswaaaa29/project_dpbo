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

Kelompok Aturan::kelompokDari(Jenis) {
    // TODO minggu ke-5 : Penentuan kelompok bola
    return Kelompok::belum;
}

int Aturan::sisaBolaKelompok(const Pemain&, const Meja&) const {
    // TODO minggu ke-5 : Kondisi menang atau kalah
    return 0;
}

HasilGiliran Aturan::evaluasi(Pemain&, Pemain&, const Meja&) {
    // TODO minggu ke-5 : Pelanggaran, pergantian giliran, menang/kalah
    masukTembakanIni.clear();
    return HasilGiliran::GantiGiliran;
}