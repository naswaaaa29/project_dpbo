#pragma once
#include <string>
#include "jenis.h"

class Meja;
class Bola;
namespace sf { class RenderWindow; }

class Pemain {
public:
    Pemain(const std::string& nama) : namaPemain(nama) {}
    virtual ~Pemain() {}

    virtual bool ambilTembakan(const Meja& meja, const sf::RenderWindow& window, Tembakan& hasil) = 0;

    const std::string& nama() const {
        return namaPemain; 
    }

    Kelompok kelompok() const {
        return kel; 
    }
    
    void setKelompok(Kelompok k) {
        kel = k; 
    }

    int jumlahMasuk() const {
        return masuk; 
    }
    
    void tambahMasuk() {
         masuk++; 
    }

    bool giliran() const {
        return sedangGiliran; 
    }
    
    void setGiliran(bool g) { 
        sedangGiliran = g; 
    }

    const Tembakan& bidikSaatIni() const {
        return bidik;
    }

    void reset() {
        kel = Kelompok::belum;
        masuk = 0;
        sedangGiliran = false;
        bidik = {0.f, 0.f};
    }

    protected:
        Tembakan bidik{0.f, 0.f};    

    private:
        std::string namaPemain;
        Kelompok kel = Kelompok::belum;
        int masuk = 0;
        bool sedangGiliran = false;
};

class PemainManusia : public Pemain {
    public:
        PemainManusia(const std::string& nama) : Pemain(nama) {}

        bool ambilTembakan(const Meja& meja,
                        const sf::RenderWindow& window,
                        Tembakan& hasil) override;

    private:
        bool menahan = false; //ngecek apakah mouse lg ditahan or not
};

class PemainBot : public Pemain {
    public:
        PemainBot(const std::string& nama) : Pemain(nama) {}

        bool ambilTembakan(const Meja& meja, const sf::RenderWindow& window, Tembakan& hasil) override;

    private:
        const Bola* pilihTarget(const Meja& meja) const;
};