#include "billiard.h"
#include <algorithm>
#include <string>

static void teksTengah(sf::Text& t, float cx, float cy) {
    sf::FloatRect b = t.getLocalBounds();
    t.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
    t.setPosition(cx, cy);
}

static void gambarRoundRect(sf::RenderWindow& w, sf::FloatRect r, float rad, sf::Color col) {
    sf::RectangleShape h({r.width - 2.f * rad, r.height});
    h.setPosition(r.left + rad, r.top);
    h.setFillColor(col);
    w.draw(h);

    sf::RectangleShape v({r.width, r.height - 2.f * rad});
    v.setPosition(r.left, r.top + rad);
    v.setFillColor(col);
    w.draw(v);

    for (int i = 0; i < 4; ++i) {
        sf::CircleShape c(rad);
        c.setFillColor(col);
        float cx = (i % 2 == 0) ? r.left + rad : r.left + r.width  - rad;
        float cy = (i < 2)      ? r.top  + rad : r.top  + r.height - rad;
        c.setPosition(cx - rad, cy - rad);
        w.draw(c);
    }
}

static void gambarBolaHias(sf::RenderWindow& w, sf::Vector2f p, float r, sf::Color c) {
    sf::CircleShape b(r);
    b.setOrigin(r, r);
    b.setPosition(p);
    b.setFillColor(c);
    w.draw(b);

    sf::CircleShape hl(r * 0.35f);
    hl.setOrigin(r * 0.35f, r * 0.35f);
    hl.setPosition(p.x - r * 0.3f, p.y - r * 0.35f);
    hl.setFillColor(sf::Color(255, 255, 255, 90));
    w.draw(hl);
}

static const char* namaKelompok(Kelompok k){
    if (k == Kelompok::polos) return "POLOS";
    if (k == Kelompok::strip) return "STRIP";
    return "BELUM";
}

static void gambarIkonKelompok(sf::RenderWindow& w, sf::Vector2f p, float r, Kelompok k){
    sf::CircleShape c(r);
    c.setOrigin(r, r);
    c.setPosition(p);

    if (k == Kelompok::polos) {
        c.setFillColor(sf::Color(210, 40, 40));
        w.draw(c);
    } 
    
    else if (k == Kelompok::strip) {
        c.setFillColor(sf::Color(245, 245, 245));
        w.draw(c);
        sf::RectangleShape pita({1.7f * r, r});
        pita.setOrigin(0.85f * r, r / 2.f);
        pita.setPosition(p);
        pita.setFillColor(sf::Color(210, 40, 40));
        w.draw(pita);
    } 
    
    else {
        c.setFillColor(sf::Color(60, 64, 74));
        c.setOutlineThickness(2.f);
        c.setOutlineColor(sf::Color(110, 116, 128));
        w.draw(c);
    }
}

static void gambarTombol(sf::RenderWindow& w, const sf::Font& f, bool adaFont, sf::FloatRect r, const char* label, unsigned ukuran, sf::Color normal, sf::Color sorot, bool hover) {
    gambarRoundRect(w, r, 14.f, hover ? sorot : normal);
    if (adaFont) {
        sf::Text t(label, f, ukuran);
        t.setFillColor(sf::Color::White);
        teksTengah(t, r.left + r.width / 2.f, r.top + r.height / 2.f);
        w.draw(t);
    }
}

Billiard::Billiard()
    : window(sf::VideoMode(1000, 600), "Billiard - Kelompok 01"),
      meja({50.f, 75.f}, {900.f, 450.f}),
      pemain("Pemain 1"), pemain2("Pemain 2") {
    window.setFramerateLimit(60);
    meja.susunBola();

    tombolMulai = sf::FloatRect(380.f, 330.f, 240.f, 62.f);
    tombolLanjut = sf::FloatRect(530.f, 450.f, 200.f, 55.f);
	tombolKembali = sf::FloatRect(270.f, 450.f, 200.f, 55.f);
    
    tombolJeda = sf::FloatRect(912.f, 16.f, 44.f, 40.f);
    tombolLanjutkan = sf::FloatRect(350.f, 245.f, 300.f, 56.f);
    tombolKeluarJeda = sf::FloatRect(350.f, 325.f, 300.f, 56.f);
    tombolYa = sf::FloatRect(320.f, 320.f, 170.f, 52.f);
    tombolTidak = sf::FloatRect(510.f, 320.f, 170.f, 52.f);

    aktif = &pemain;
    
    adaFont =
        font.loadFromFile("C:/Windows/Fonts/segoeuib.ttf") ||
        font.loadFromFile("C:/Windows/Fonts/arialbd.ttf")  ||
        font.loadFromFile("C:/Windows/Fonts/segoeui.ttf")  ||
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");
}

void Billiard::jalan() {
    while (window.isOpen()) {
        float dt = jam.restart().asSeconds();
        if (dt > 0.05f) dt = 0.05f;
        prosesEvent();
        update(dt);
        render();
    }
}

void Billiard::prosesEvent() {
    sf::Event event;
    while (window.pollEvent(event)) {

        if (event.type == sf::Event::Closed) {
            window.close();
            continue;
        }

        if (status == Status::Menu) {
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f m(
                    (float)event.mouseMove.x,
                    (float)event.mouseMove.y
                );

                hoverTombol = tombolMulai.contains(m);
            }

            else if (event.type == sf::Event::MouseButtonReleased &&
                     event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2f m(
                    (float)event.mouseButton.x,
                    (float)event.mouseButton.y
                );

                if (tombolMulai.contains(m)) {
                    status = Status::Aturan;
                }
            }

            else if (event.type == sf::Event::KeyPressed &&
                     (event.key.code == sf::Keyboard::Return ||
                      event.key.code == sf::Keyboard::Space)) {

                status = Status::Aturan;
            }
        }

        else if (status == Status::Aturan) {
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f m(
                    (float)event.mouseMove.x,
                    (float)event.mouseMove.y
                );
                hoverLanjut = tombolLanjut.contains(m);
                hoverKembali = tombolKembali.contains(m);
            }

            else if (event.type == sf::Event::MouseButtonReleased &&
                     event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m(
                    (float)event.mouseButton.x,
                    (float)event.mouseButton.y
                );

                if (tombolKembali.contains(m)) {
                    status = Status::Menu;
                }

                else if (tombolLanjut.contains(m)) {
                    mulaiGame();
                }
            }
        }
    
        else if(status == Status::Bermain){
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
                bukaJeda();
            }

            else if (event.type == sf::Event::MouseMoved){
                sf::Vector2f m((float) event.mouseMove.x, (float) event.mouseMove.y);
                hoverJeda = tombolJeda.contains(m);
            }

            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m((float)event.mouseButton.x, (float)event.mouseButton.y);
                if (tombolJeda.contains(m)) bukaJeda();
            }
        }

        else if (status == Status::Jeda) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                lanjutDariJeda();
            }

            else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f m((float)event.mouseMove.x, (float)event.mouseMove.y);
                hoverLanjutkan  = tombolLanjutkan.contains(m);
                hoverKeluarJeda = tombolKeluarJeda.contains(m);
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m((float)event.mouseButton.x, (float)event.mouseButton.y);
                
                if (tombolLanjutkan.contains(m)) {
                    lanjutDariJeda();
                }
                
                else if (tombolKeluarJeda.contains(m)) {
                    hoverYa = hoverTidak = false;
                    status = Status::KonfirmasiKeluar;
                }
            }
        }

        else if (status == Status::KonfirmasiKeluar) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                status = Status::Jeda;
            }

            else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f m((float)event.mouseMove.x, (float)event.mouseMove.y);
                hoverYa = tombolYa.contains(m);
                hoverTidak = tombolTidak.contains(m);
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m((float)event.mouseButton.x, (float)event.mouseButton.y);
                
                if (tombolYa.contains(m)) {
                    keluarKeMenu();
                }
                
                else if (tombolTidak.contains(m)) {
                    status = Status::Jeda;
                }
            }
        }
    }
}

void Billiard::mulaiGame() {
    meja.susunBola();
    pemain.reset();
    pemain2.reset();
    pemain.batalkanTembakan();
    pemain2.batalkanTembakan();
    aturan.reset();

    aktif = &pemain;
    pemain.setGiliran(true);
    pemain2.setGiliran(false);

    tembakanBerjalan = false;
    notifSisa = 0.f;

    status = Status::Bermain;
    jam.restart();
}

void Billiard::bukaJeda() {
    aktif -> batalkanTembakan();
    hoverJeda = false;
    hoverLanjutkan = hoverKeluarJeda = false;
    status = Status::Jeda;
}

void Billiard::lanjutDariJeda(){
    status = Status::Bermain;
    jam.restart();
}

void Billiard::keluarKeMenu() {
    aktif -> batalkanTembakan();
    tembakanBerjalan = false;
    notifSisa = 0.f;
    hoverTombol = false;
    status = Status::Menu;
}

void Billiard::gantiGiliran() {
    Pemain* lawan = (aktif == &pemain) ? &pemain2 : &pemain;
    aktif -> setGiliran(false);
    lawan->setGiliran(true);
    aktif = lawan;
}

void Billiard::selesaiTembakan() {
    tembakanBerjalan = false;

    for (int nomor : meja.ambilBolaMasuk()){
        for(const Bola& b : meja.daftarBola()){
            if (b.nomor() == nomor) {
                aturan.bolaMasuk(b);
                break;
            }
        }
    }

    Pemain* lawan = (aktif == &pemain) ? &pemain2 : &pemain;
    bool belumPunyaKelompok = (aktif ->kelompok() == Kelompok::belum);

    HasilGiliran hasil = aturan.evaluasi(*aktif, *lawan, meja);

    if (belumPunyaKelompok && aktif->kelompok() != Kelompok::belum) {
        notifTeks = aktif->nama() + " mendapat bola " + namaKelompok(aktif->kelompok()) + "  |  " + lawan->nama() + " mendapat bola " + namaKelompok(lawan ->kelompok());

        notifSisa = 3.5f;
    }

    if (hasil != HasilGiliran::LanjutMain) gantiGiliran();
}

void Billiard::update(float dt) {
    if (status != Status::Bermain) return;

    meja.update(dt);

    if (notifSisa > 0.f) notifSisa -= dt;

    if (!meja.semuaBolaBerhenti()) return;

    if(tembakanBerjalan) selesaiTembakan();

    Tembakan t;
    bool menembak = aktif-> ambilTembakan(meja, window, t);

    stik.aturBidik(aktif->bidikSaatIni());

    if (menembak) {
        if (Bola* putih = meja.bolaPutih()) {
            stik.aturBidik(t);
            stik.pukul(*putih);
        }
    }
}


void Billiard::render() {
    if (status == Status::Menu) {
        renderMenu();
        return;
    }
    if (status == Status::Aturan) {
        renderAturan();
        return;
    }

    window.clear(sf::Color(40, 42, 48));
    meja.gambar(window);

    if (status == Status::Bermain && meja.semuaBolaBerhenti()) {
        if (Bola* putih = meja.bolaPutih())
            stik.gambar(window, *putih);
    }

    gambarPanelKelompok();
    if(status == Status::Bermain) gambarTombolJeda();
    gambarNotif();

    if (status == Status::Jeda) renderJeda();
    else if (status == Status::KonfirmasiKeluar) renderKonfirmasi();

    window.display();
}


void Billiard::gambarTombolJeda() {
    sf::Color warna = hoverJeda ? sf::Color (46, 204, 113) : sf::Color(75, 85, 95);
    gambarRoundRect(window, tombolJeda, 10.f, warna);

    float cx = tombolJeda.left + tombolJeda.width / 2.f;
    float cy = tombolJeda.top + tombolJeda.height / 2.f;
    for (int i = 0; i < 2; ++i){
        sf::RectangleShape batang({6.f, 18.f});
        batang.setOrigin(3.f, 9.f);
        batang.setPosition(cx + (i == 0 ? -6.f : 6.f), cy);
        batang.setFillColor(sf::Color::White);
        window.draw(batang);
    }
}

void Billiard::gambarPanelKelompok() {
    const Pemain* daftar[2] = { &pemain, &pemain2 };

    for (int i = 0; i < 2; ++i) {
        const Pemain& p = *daftar[i];
        sf::FloatRect kartu(i == 0 ? 50.f : 650.f, 543.f, 300.f, 44.f);

        if (aktif == &p) {
            gambarRoundRect(window,
                sf::FloatRect(kartu.left - 2.f, kartu.top - 2.f, kartu.width + 4.f, kartu.height + 4.f),
                14.f, sf::Color(46, 204, 113));
        }
        gambarRoundRect(window, kartu, 12.f, sf::Color(32, 35, 45));
        gambarIkonKelompok(window, {kartu.left + 26.f, kartu.top + 22.f}, 12.f, p.kelompok());

        if (adaFont) {
            sf::Text nama(p.nama(), font, 15);
            nama.setFillColor(sf::Color::White);
            nama.setPosition(kartu.left + 48.f, kartu.top + 4.f);
            window.draw(nama);

            std::string ket = (p.kelompok() == Kelompok::belum)
                ? std::string("Kelompok belum ditentukan")
                : std::string(namaKelompok(p.kelompok())) + "   " + std::to_string(p.jumlahMasuk()) + "/7";
            sf::Text t2(ket, font, 13);
            t2.setFillColor(sf::Color(170, 176, 188));
            t2.setPosition(kartu.left + 48.f, kartu.top + 23.f);
            window.draw(t2);
        }
    }
}

void Billiard::gambarNotif() {
    if (notifSisa <= 0.f || !adaFont) return;

    float a = std::min(1.f, notifSisa / 0.6f);
    sf::FloatRect kartu(180.f, 12.f, 640.f, 44.f);
    gambarRoundRect(window, kartu, 14.f, sf::Color(32, 35, 45, (sf::Uint8)(235 * a)));

    sf::Text t(notifTeks, font, 16);
    t.setFillColor(sf::Color(46, 204, 113, (sf::Uint8)(255 * a)));
    teksTengah(t, 500.f, 34.f);
    window.draw(t);
}

void Billiard::renderJeda() {
    sf::RectangleShape gelap({1000.f, 600.f});
    gelap.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(gelap);

    gambarRoundRect(window, sf::FloatRect(300.f, 140.f, 400.f, 320.f), 24.f, sf::Color(32, 35, 45, 245));

    if (adaFont) {
        sf::Text judul("JEDA", font, 44);
        judul.setFillColor(sf::Color(236, 240, 244));
        teksTengah(judul, 500.f, 195.f);
        window.draw(judul);

        sf::Text hint("Tekan ESC untuk melanjutkan", font, 14);
        hint.setFillColor(sf::Color(150, 156, 168));
        teksTengah(hint, 500.f, 420.f);
        window.draw(hint);
    }

    gambarTombol(window, font, adaFont, tombolLanjutkan, "LANJUTKAN", 22,
                 sf::Color(39, 174, 96), sf::Color(46, 204, 113), hoverLanjutkan);
    gambarTombol(window, font, adaFont, tombolKeluarJeda, "KELUAR", 22,
                 sf::Color(150, 60, 60), sf::Color(190, 75, 75), hoverKeluarJeda);
}

void Billiard::renderKonfirmasi() {
    sf::RectangleShape gelap({1000.f, 600.f});
    gelap.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(gelap);

    gambarRoundRect(window, sf::FloatRect(280.f, 170.f, 440.f, 240.f), 24.f, sf::Color(32, 35, 45, 248));

    if (adaFont) {
        sf::Text judul("KELUAR KE MENU?", font, 30);
        judul.setFillColor(sf::Color(236, 240, 244));
        teksTengah(judul, 500.f, 225.f);
        window.draw(judul);

        sf::Text ket("Permainan saat ini akan berakhir.", font, 16);
        ket.setFillColor(sf::Color(170, 176, 188));
        teksTengah(ket, 500.f, 270.f);
        window.draw(ket);
    }

    gambarTombol(window, font, adaFont, tombolYa, "YA", 22,
                 sf::Color(150, 60, 60), sf::Color(190, 75, 75), hoverYa);
    gambarTombol(window, font, adaFont, tombolTidak, "TIDAK", 22,
                 sf::Color(75, 85, 95), sf::Color(100, 110, 120), hoverTidak);
}

void Billiard::renderAturan() {
    window.clear(sf::Color(24, 26, 34));
    sf::FloatRect kartu(180.f, 80.f, 640.f, 440.f);

    gambarRoundRect(
        window,
        kartu,
        24.f,
        sf::Color(32, 35, 45, 240)
    );

    if (adaFont) {
        sf::Text judul("GAME RULES", font, 42);
        judul.setFillColor(sf::Color(46, 204, 113));
        teksTengah(judul, 500.f, 135.f);
        window.draw(judul);

        sf::Text r1("OBJECTIVE", font, 22);
        r1.setFillColor(sf::Color::White);
        r1.setPosition(250.f, 195.f);
        window.draw(r1);

        sf::Text d1(
            "Masukkan bola ke dalam lubang untuk memenangkan permainan.",
            font, 15
        );
        d1.setFillColor(sf::Color(190, 195, 205));
        d1.setPosition(250.f, 228.f);
        window.draw(d1);

        sf::Text r2("HOW TO PLAY", font, 22);
        r2.setFillColor(sf::Color::White);
        r2.setPosition(250.f, 280.f);
        window.draw(r2);

        sf::Text d2(
            "Arahkan stik dan lakukan tembakan untuk memasukkan bola.",
            font, 15
        );
        d2.setFillColor(sf::Color(190, 195, 205));
        d2.setPosition(250.f, 313.f);
        window.draw(d2);

        sf::Text r3("GAME TIPS", font, 22);
        r3.setFillColor(sf::Color::White);
        r3.setPosition(250.f, 365.f);
        window.draw(r3);

        sf::Text d3(
            "Atur arah dan kekuatan tembakan dengan hati-hati.",
            font, 15
        );
        d3.setFillColor(sf::Color(190, 195, 205));
        d3.setPosition(250.f, 398.f);
        window.draw(d3);
    }

    sf::Color warnaBack = hoverKembali
        ? sf::Color(100, 110, 120)
        : sf::Color(75, 85, 95);

    gambarRoundRect(
        window,
        tombolKembali,
        12.f,
        warnaBack
    );

    sf::Color warnaLanjut = hoverLanjut
        ? sf::Color(46, 204, 113)
        : sf::Color(39, 174, 96);

    gambarRoundRect(
        window,
        tombolLanjut,
        12.f,
        warnaLanjut
    );

    if (adaFont) {
        sf::Text back("BACK", font, 20);
        back.setFillColor(sf::Color::White);

        teksTengah(
            back,
            tombolKembali.left + tombolKembali.width / 2.f,
            tombolKembali.top + tombolKembali.height / 2.f
        );

        window.draw(back);

        sf::Text lanjut("START GAME", font, 20);
        lanjut.setFillColor(sf::Color::White);

        teksTengah(
            lanjut,
            tombolLanjut.left + tombolLanjut.width / 2.f,
            tombolLanjut.top + tombolLanjut.height / 2.f
        );

        window.draw(lanjut);
    }

    window.display();
}

void Billiard::renderMenu() {
    sf::VertexArray bg(sf::Quads, 4);
    bg[0].position = {0.f, 0.f};      bg[0].color = sf::Color(24, 26, 34);
    bg[1].position = {1000.f, 0.f};   bg[1].color = sf::Color(24, 26, 34);
    bg[2].position = {1000.f, 600.f}; bg[2].color = sf::Color(10, 30, 21);
    bg[3].position = {0.f, 600.f};    bg[3].color = sf::Color(10, 30, 21);
    window.clear();
    window.draw(bg);

    gambarBolaHias(window, {120.f, 110.f}, 22.f, sf::Color(240, 200, 0));
    gambarBolaHias(window, {175.f,  92.f}, 20.f, sf::Color(210, 40, 40));
    gambarBolaHias(window, {880.f, 110.f}, 22.f, sf::Color(0, 70, 200));
    gambarBolaHias(window, {828.f,  92.f}, 20.f, sf::Color(20, 140, 70));
    gambarBolaHias(window, {110.f, 500.f}, 22.f, sf::Color(230, 120, 20));
    gambarBolaHias(window, {162.f, 540.f}, 20.f, sf::Color(120, 40, 160));
    gambarBolaHias(window, {890.f, 500.f}, 22.f, sf::Color(210, 40, 40));
    gambarBolaHias(window, {838.f, 540.f}, 20.f, sf::Color(0, 70, 200));

    sf::FloatRect kartu(240.f, 130.f, 520.f, 340.f);
    gambarRoundRect(window,
        sf::FloatRect(kartu.left - 3.f, kartu.top - 3.f, kartu.width + 6.f, kartu.height + 6.f),
        26.f, sf::Color(46, 204, 113, 70));
    gambarRoundRect(window, kartu, 24.f, sf::Color(32, 35, 45, 240));

    if (adaFont) {
        sf::Text judul("GAME BILLIARD", font, 60);
        judul.setFillColor(sf::Color(236, 240, 244));
        teksTengah(judul, 500.f, 212.f);
        window.draw(judul);

        sf::Text sub("Kelompok 01", font, 22);
        sub.setFillColor(sf::Color(46, 204, 113));
        teksTengah(sub, 500.f, 262.f);
        window.draw(sub);
    }

    gambarRoundRect(window,
        sf::FloatRect(tombolMulai.left, tombolMulai.top + 4.f, tombolMulai.width, tombolMulai.height),
        16.f, sf::Color(0, 0, 0, 90));
    sf::Color warnaTombol = hoverTombol ? sf::Color(46, 204, 113) : sf::Color(39, 174, 96);
    gambarRoundRect(window, tombolMulai, 16.f, warnaTombol);

    if (adaFont) {
        sf::Text labelTombol("MULAI", font, 26);
        labelTombol.setFillColor(sf::Color::White);
        teksTengah(labelTombol,
                   tombolMulai.left + tombolMulai.width / 2.f,
                   tombolMulai.top + tombolMulai.height / 2.f);
        window.draw(labelTombol);

        sf::Text i1("Selamat datang di Game Billiard dan Selamat Bermain!", font, 15);
        i1.setFillColor(sf::Color(150, 156, 168));
        teksTengah(i1, 500.f, 424.f);
        window.draw(i1);
    }

    window.display();
}
