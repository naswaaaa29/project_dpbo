#include "billiard.h"

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

Billiard::Billiard()
    : window(sf::VideoMode(1000, 600), "Billiard - Kelompok 01"),
      meja({50.f, 75.f}, {900.f, 450.f}),
      pemain("Pemain 1") {
    window.setFramerateLimit(60);
    meja.susunBola();

    tombolMulai = sf::FloatRect(380.f, 330.f, 240.f, 62.f);
    tombolLanjut = sf::FloatRect(530.f, 450.f, 200.f, 55.f);
	tombolKembali = sf::FloatRect(270.f, 450.f, 200.f, 55.f);

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
    }
}

void Billiard::mulaiGame() {
    meja.susunBola();
    pemain.reset();
    status = Status::Bermain;
    jam.restart();
}

void Billiard::update(float dt) {
    if (status != Status::Bermain) return;

    meja.update(dt);

    if (meja.semuaBolaBerhenti()) {
        Tembakan t;
        bool menembak = pemain.ambilTembakan(meja, window, t);

        stik.aturBidik(pemain.bidikSaatIni());   

        if (menembak) {
            if (Bola* putih = meja.bolaPutih()) {
                stik.aturBidik(t);
                stik.pukul(*putih);
            }
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

    if (meja.semuaBolaBerhenti()) {
        if (Bola* putih = meja.bolaPutih())
            stik.gambar(window, *putih);
    }

    window.display();
}


void Billiard::renderAturan() {

    window.clear(sf::Color(24, 26, 34));

    // Kartu utama
    sf::FloatRect kartu(180.f, 80.f, 640.f, 440.f);

    gambarRoundRect(
        window,
        kartu,
        24.f,
        sf::Color(32, 35, 45, 240)
    );

    if (adaFont) {

        // Judul
        sf::Text judul("GAME RULES", font, 42);
        judul.setFillColor(sf::Color(46, 204, 113));
        teksTengah(judul, 500.f, 135.f);
        window.draw(judul);

        // Rules 1
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

        // Rules 2
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

        // Rules 3
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

    // Tombol BACK
    sf::Color warnaBack = hoverKembali
        ? sf::Color(100, 110, 120)
        : sf::Color(75, 85, 95);

    gambarRoundRect(
        window,
        tombolKembali,
        12.f,
        warnaBack
    );

    // Tombol START GAME
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
