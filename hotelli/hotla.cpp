// Meri Hulkkonen
// Harjoitustyö, C++ perusteet
// Hotellin varausjärjestelmä
// Tavoittelin 4p mutta ehkä 2-3p, puuttuu muutama syötteen tarkistus ja vähän sekava

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <sstream> 

using namespace std;

// Satunnaislukufunktio
int generateRandom(int min, int max) {
    int randomNumber = 0;
    do {
        randomNumber = rand() % (max - min + 1) + min;
    } while (randomNumber % 2 != 0); // parillinen luku
    return randomNumber;
}

//Varausnumeron arvonta
int arvoVarausnumero() {
    return generateRandom(10000, 99999); // Varausnumero arvotaan väliltä 10000-99999
}

// Huoneluokka
struct Huone {
    int numero = 0;
    bool onYksi = true; // true=yhden hengen huone, false=kahden hengen huone
    bool onVarattu = false; // true=varattu, false=vapaa
    string varaajanNimi;
    int varausnumero = 0;
};

// esitellään funktiot
void teeVaraus(vector<Huone>& huoneet, int& seuraavaVarausnumero, int huonemaara);
void tarkistaVaraus(const vector<Huone>& huoneet);
void poistaVaraus(vector<Huone>& huoneet);
void haeVaraus(const vector<Huone>& huoneet);
double laskeHinta(bool onYksi, int yot);
void listaaVapaatHuoneet(const vector<Huone>& huoneet, bool onYksi);

int main() {
    setlocale(LC_ALL, "fi_FI"); // ääkköset
    srand(time(0)); // Satunnaislukugeneraattorin siemen(?)

    int huonemaara = generateRandom(40, 300); // Satunnaistetaan huoneiden määrä (parillinen luku)
    vector<Huone> huoneet(huonemaara);

    // Alustetaan huoneet
    for (int i = 0; i < huonemaara / 2; ++i) {
        huoneet[i] = { i + 1, true, false, "", 0 };
    }
    for (int i = huonemaara / 2; i < huonemaara; ++i) {
        huoneet[i] = { i + 1, false, false, "", 0 };
    }

    int seuraavaVarausnumero = 10000; // Aloitetaan varausnumerolla 10000
    int valinta;
    bool virhe; // virheen tarkistusta varten

    cout << "Tervetuloa hotellin varausjärjestelmään!" << endl; // ohjelma alkaa

    while (true) {
        // Valikko
        cout << "\nValitse mitä haluat tehdä (valitse 1-5): " << endl;
        cout << "1. Tee varaus" << endl;
        cout << "2. Tarkista varaus" << endl;
        cout << "3. Poista varaus" << endl;
        cout << "4. Hae varaus (nimellä tai varausnumerolla)" << endl;
        cout << "5. Lopeta ohjelma" << endl;
        cout << "\nValintasi: ";

        // Syötteen tarkistus valikolle
        do {
            virhe = false;
            cin >> valinta;

            if (cin.fail() || valinta < 1 || valinta > 5) {
                cout << "Virhe! Anna luku väliltä 1-5!" << endl;
                virhe = true;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (virhe);

        // Käsitellään käyttäjän valinta
        switch (valinta) {
        case 1:
            teeVaraus(huoneet, seuraavaVarausnumero, huonemaara);
            break;
        case 2:
            tarkistaVaraus(huoneet);
            break;
        case 3:
            poistaVaraus(huoneet);
            break;
        case 4:
            haeVaraus(huoneet);
            break;
        case 5:
            cout << "Kiitos, että käytit varausjärjestelmää! Näkemiin!" << endl;
            return 0;
        default:
            cout << "Virheellinen valinta, yritä uudelleen." << endl;
            break;
        }
    }
}

void teeVaraus(vector<Huone>& huoneet, int& seuraavaVarausnumero, int huonemaara) {
    int tyyppi, yot, huoneValinta;
    string nimi;

    cout << "Valitse huonetyyppi (1=yhden hengen, 2=kahden hengen): ";

    // Syötteen tarkistus huonetyypille
    bool virhe=false;
    do {
        virhe = false;
        cin >> tyyppi;

        if (cin.fail() || tyyppi < 1 || tyyppi > 2) {
            cout << "Virhe! Valitse luku 1 tai 2!" << endl;
            virhe = true;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (virhe);

    cout << "Anna varaajan nimi: ";
    cin.ignore();
    getline(cin, nimi);

    cout << "Kuinka monta yötä haluat varata? ";
    cin >> yot;


    // Listataan vapaat huoneet valitulle tyypille
    listaaVapaatHuoneet(huoneet, (tyyppi == 1));

    cout << "Valitse haluamasi huone (huonenumero): ";
    cin >> huoneValinta;


    bool huoneValittu = false;
    for (auto& huone : huoneet) {
        if (huone.numero == huoneValinta) {
            if (huone.onVarattu) {
                cout << "Valitsemasi huone on jo varattu. Valitse toinen huone." << endl;
                huoneValittu = true;
                break;
            }
            else if (tyyppi == 1 && huoneValinta <= huonemaara / 2 || tyyppi == 2 && huoneValinta >= huonemaara / 2) {
                huone.onVarattu = true;
                huone.varaajanNimi = nimi;
                huone.varausnumero = arvoVarausnumero();

                // Satunnainen alennus
                int alennus = generateRandom(0, 2) * 10; // 0%, 10% tai 20%
                double hinta = laskeHinta(huone.onYksi, yot) * (1 - alennus / 100.0);

                cout << "\nHuone " << huone.numero << " on varattu! Varausnumero: " << huone.varausnumero << "." << endl;
                cout << "Yhteensä: " << hinta << " euroa (" << alennus << "% alennus).\n";
                huoneValittu = true;
                break;
            }
            else {
                cout << "Valitsit virheellisen huonenumeron." << endl;
            }
        }
    }

    if (!huoneValittu) {
        cout << "Yritä uudelleen. " << endl;
    }
}

double laskeHinta(bool onYksi, int yot) {
    return yot * (onYksi ? 100 : 150); // Yhden hengen huone = 100 €/yö, kahden hengen huone = 150 €/yö
}


void listaaVapaatHuoneet(const vector<Huone>& huoneet, bool onYksi) {
    stringstream vapaatHuoneet; // Käytetään stringstreamia huoneiden listaamiseen yhdelle riville
    bool loydettiinVapaita = false; // Tämä muuttuja tarkistaa, löytyykö vapaita huoneita

    for (const auto& huone : huoneet) {
        if (huone.onYksi == onYksi && !huone.onVarattu) {
            if (loydettiinVapaita) {
                vapaatHuoneet << ", "; // Lisätään pilkku välilyönnillä, jos huoneita on jo listattu
            }
            vapaatHuoneet << huone.numero; // Lisää huonenumeron
            loydettiinVapaita = true; // Jos löytyy vapaa huone, asetetaan tämä arvo todeksi
        }
    }

    // Jos löytyy vapaita huoneita, tulostetaan ne yhdellä rivillä
    if (loydettiinVapaita) {
        cout << "Vapaat huoneet ovat: " << vapaatHuoneet.str() << endl;
    }
    else {
        cout << "Ei vapaita huoneita valitulla tyypillä." << endl;
    }
}

// Varauksen tarkistaminen
void tarkistaVaraus(const vector<Huone>& huoneet) {
    int varausnumero;
    cout << "Anna varausnumero tarkistaaksesi varauksen: ";
    cin >> varausnumero;

    bool loydettiin = false;
    for (const auto& huone : huoneet) {
        if (huone.varausnumero == varausnumero) {
            cout << "Varausnumero " << varausnumero << " on huoneessa " << huone.numero
                << ". Varaaja: " << huone.varaajanNimi << endl;
            loydettiin = true;
            break;
        }
    }

    if (!loydettiin) {
        cout << "Varausta ei löytynyt." << endl;
    }
}

// Varauksen poistaminen
void poistaVaraus(vector<Huone>& huoneet) {
    int varausnumero;
    cout << "Anna varausnumero poistettavaksi: ";
    cin >> varausnumero;

    bool loydettiin = false;
    for (auto& huone : huoneet) {
        if (huone.varausnumero == varausnumero) {
            huone.onVarattu = false;
            huone.varaajanNimi = "";
            huone.varausnumero = 0;
            cout << "Varaus poistettu huoneesta " << huone.numero << endl;
            loydettiin = true;
            break;
        }
    }

    if (!loydettiin) {
        cout << "Varausta ei löytynyt." << endl;
    }
}

// Varauksen hakeminen
void haeVaraus(const vector<Huone>& huoneet) {
    string nimi;
    cout << "Anna varaajan nimi tai varausnumero: ";
    cin.ignore();
    getline(cin, nimi);

    bool loydettiin = false;
    for (const auto& huone : huoneet) {
        if (huone.varaajanNimi == nimi || to_string(huone.varausnumero) == nimi) {
            cout << "Varaus löytyi huoneesta " << huone.numero << ", varaajan nimi: " << huone.varaajanNimi
                << ", varausnumero: " << huone.varausnumero << endl;
            loydettiin = true;
            break;
        }
    }

    if (!loydettiin) {
        cout << "Varausta ei löytynyt." << endl;
    }
}