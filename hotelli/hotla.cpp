// Meri Hulkkonen
// Harjoitusty�, C++ perusteet
// Hotellin varausj�rjestelm�
// Tavoittelin 4p mutta ehk� 2-3p, puuttuu muutama sy�tteen tarkistus ja v�h�n sekava

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
    return generateRandom(10000, 99999); // Varausnumero arvotaan v�lilt� 10000-99999
}

// Huoneluokka
struct Huone {
    int numero = 0;
    bool onYksi = true; // true=yhden hengen huone, false=kahden hengen huone
    bool onVarattu = false; // true=varattu, false=vapaa
    string varaajanNimi;
    int varausnumero = 0;
};

// esitell��n funktiot
void teeVaraus(vector<Huone>& huoneet, int& seuraavaVarausnumero, int huonemaara);
void tarkistaVaraus(const vector<Huone>& huoneet);
void poistaVaraus(vector<Huone>& huoneet);
void haeVaraus(const vector<Huone>& huoneet);
double laskeHinta(bool onYksi, int yot);
void listaaVapaatHuoneet(const vector<Huone>& huoneet, bool onYksi);

int main() {
    setlocale(LC_ALL, "fi_FI"); // ��kk�set
    srand(time(0)); // Satunnaislukugeneraattorin siemen(?)

    int huonemaara = generateRandom(40, 300); // Satunnaistetaan huoneiden m��r� (parillinen luku)
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

    cout << "Tervetuloa hotellin varausj�rjestelm��n!" << endl; // ohjelma alkaa

    while (true) {
        // Valikko
        cout << "\nValitse mit� haluat tehd� (valitse 1-5): " << endl;
        cout << "1. Tee varaus" << endl;
        cout << "2. Tarkista varaus" << endl;
        cout << "3. Poista varaus" << endl;
        cout << "4. Hae varaus (nimell� tai varausnumerolla)" << endl;
        cout << "5. Lopeta ohjelma" << endl;
        cout << "\nValintasi: ";

        // Sy�tteen tarkistus valikolle
        do {
            virhe = false;
            cin >> valinta;

            if (cin.fail() || valinta < 1 || valinta > 5) {
                cout << "Virhe! Anna luku v�lilt� 1-5!" << endl;
                virhe = true;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (virhe);

        // K�sitell��n k�ytt�j�n valinta
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
            cout << "Kiitos, ett� k�ytit varausj�rjestelm��! N�kemiin!" << endl;
            return 0;
        default:
            cout << "Virheellinen valinta, yrit� uudelleen." << endl;
            break;
        }
    }
}

void teeVaraus(vector<Huone>& huoneet, int& seuraavaVarausnumero, int huonemaara) {
    int tyyppi, yot, huoneValinta;
    string nimi;

    cout << "Valitse huonetyyppi (1=yhden hengen, 2=kahden hengen): ";

    // Sy�tteen tarkistus huonetyypille
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

    cout << "Kuinka monta y�t� haluat varata? ";
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
                cout << "Yhteens�: " << hinta << " euroa (" << alennus << "% alennus).\n";
                huoneValittu = true;
                break;
            }
            else {
                cout << "Valitsit virheellisen huonenumeron." << endl;
            }
        }
    }

    if (!huoneValittu) {
        cout << "Yrit� uudelleen. " << endl;
    }
}

double laskeHinta(bool onYksi, int yot) {
    return yot * (onYksi ? 100 : 150); // Yhden hengen huone = 100 �/y�, kahden hengen huone = 150 �/y�
}


void listaaVapaatHuoneet(const vector<Huone>& huoneet, bool onYksi) {
    stringstream vapaatHuoneet; // K�ytet��n stringstreamia huoneiden listaamiseen yhdelle riville
    bool loydettiinVapaita = false; // T�m� muuttuja tarkistaa, l�ytyyk� vapaita huoneita

    for (const auto& huone : huoneet) {
        if (huone.onYksi == onYksi && !huone.onVarattu) {
            if (loydettiinVapaita) {
                vapaatHuoneet << ", "; // Lis�t��n pilkku v�lily�nnill�, jos huoneita on jo listattu
            }
            vapaatHuoneet << huone.numero; // Lis�� huonenumeron
            loydettiinVapaita = true; // Jos l�ytyy vapaa huone, asetetaan t�m� arvo todeksi
        }
    }

    // Jos l�ytyy vapaita huoneita, tulostetaan ne yhdell� rivill�
    if (loydettiinVapaita) {
        cout << "Vapaat huoneet ovat: " << vapaatHuoneet.str() << endl;
    }
    else {
        cout << "Ei vapaita huoneita valitulla tyypill�." << endl;
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
        cout << "Varausta ei l�ytynyt." << endl;
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
        cout << "Varausta ei l�ytynyt." << endl;
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
            cout << "Varaus l�ytyi huoneesta " << huone.numero << ", varaajan nimi: " << huone.varaajanNimi
                << ", varausnumero: " << huone.varausnumero << endl;
            loydettiin = true;
            break;
        }
    }

    if (!loydettiin) {
        cout << "Varausta ei l�ytynyt." << endl;
    }
}