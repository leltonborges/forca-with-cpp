#pragma once
#ifndef FORCA_FORCA_H
#define FORCA_FORCA_H

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "readFile.h"
#include <optional>
#include <stdexcept>

using namespace std;
const string &pathfile = "../secret.txt";

string secret_word = randWord(pathfile);
string guessHits(secret_word.size(), '\0');
bool isWinner = false;
bool isHang = false;

typedef enum OptionMenu {
    TRY_AGAIN = 1, NEW_WORD = 2, FINISH = 3, DEFAULT = 0
} Option;

typedef struct L {
private:
    int qtdErKick{};
    int totalKick{};
public:
    L(): qtd(0), qtdErKick(0), c('\0'), kick(false), totalKick(0) {}

    L(int qtd, char c, bool kick): qtd(qtd), qtdErKick(0), c(c), kick(kick), totalKick(0) {
        this->incrementKick();
    }

    int qtd{};
    char c{};
    bool kick{};

    void incrementQtdEr();

    void incrementKick();

    [[nodiscard]] bool erKick() const;

    [[nodiscard]] int qtdEr() const;

    [[nodiscard]] int qtdTotalKick() const;
} Letter;

bool L::erKick() const {
    return this->kick && this->qtd == 0;
}

void L::incrementQtdEr() {
    if (this->erKick()) this->qtdErKick++;
}

int L::qtdEr() const {
    return this->qtdErKick;
}

int L::qtdTotalKick() const {
    return this->totalKick;
}

void L::incrementKick() {
    this->totalKick++;
}

struct MapLetter {
    Letter letter;
};

map<char, MapLetter> kickLetter;

void isVerifyKick(char guess, Letter *letter);

void showWord(string word, bool winner);

void verifyCaracter(char caracter, Letter *letter);

bool isKickLetter(char letter);

Letter tryHit();

bool isHits(Letter letter);

void showWinner();

void showErKick();

void showMessageErrorKick(Letter *letter);

void play();

char upperWord(char guess);

void menuOption();

void executeSubMenu(int op);

void tryAgain();

void reset();

template<Option _t>
class OpSubMenu {
public:
    void execute();
};

template<>
void OpSubMenu<NEW_WORD>::execute() {
    string newWord;
    cout << "Informe a nova palavra" << endl;
    cin >> newWord;
    addNewWord(pathfile, newWord);
}

template<>
void OpSubMenu<FINISH>::execute() {
    cout << "Fim de jogo" << endl;
    exit(EXIT_SUCCESS);
}

template<>
void OpSubMenu<TRY_AGAIN>::execute() {
    tryAgain();
    isWinner = false;
}

template<>
void OpSubMenu<DEFAULT>::execute() {
    cout << "DEfault" << endl;
    isWinner = false;
    menuOption();
}

template<Option _t>
void execute() {
    OpSubMenu<_t> opSubMenu;
    opSubMenu.execute();
}

void verifyCaracter(char caracter, Letter *letter) {
    for (char i: secret_word)
        if (i == caracter)
            letter->qtd++;

    kickLetter[letter->c].letter = {letter->qtd, letter->c, true};
    showMessageErrorKick(letter);
}

bool isKickLetter(char letter) {
    return kickLetter[letter].letter.kick;
}

Letter tryHit() {
    Letter letter;
    char guess{};
    cout << "Digite uma letra" << endl;
    cin >> guess;

    guess = upperWord(guess);

    isVerifyKick(guess, &letter);

    return letter;
}

char upperWord(char guess) { return char(toupper((unsigned char) guess)); }

void isVerifyKick(char guess, Letter *letter) {
    if (isKickLetter(guess)) {
        kickLetter[guess].letter.incrementQtdEr();
        kickLetter[guess].letter.incrementKick();
        endl(cout);
        cout << "====== Letra já chutada ======" << endl;
        endl(cout);
    } else {
        letter->c = guess;
        verifyCaracter(guess, letter);
    }

}

bool isHits(Letter letter) {
    return letter.qtd > 0;
}

void replaceHits(Letter letter) {
    for (int i = 0; i < secret_word.size(); ++i) {
        char caracter = upperWord(letter.c);
        if (secret_word[i] == caracter)
            guessHits[i] = caracter;
    }
}

void showWord(string word, bool winner) {
    long wordSize = long(word.size());
    int i = 0;

    do {
        if (winner)
            cout << word[i];
        else {
            if (!word[i])
                cout << "_";
            else
                cout << word[i];
        }
        cout << " ";
        i++;
    } while (wordSize > i);
    endl(cout);
}

void showWinner() {
    cout << "Parabésn você ganhou" << endl;
    showWord(secret_word, true);
}

void verifyWinner() {

    if (secret_word == guessHits) {
        isWinner = true;
        showWinner();
        menuOption();
    }
}

void showErKick() {
    auto start = kickLetter.begin();
    int totalEr = 0;
    int totalKick = 0;
    endl(cout);
    cout << "==================" << endl;
    cout << "Tentativas erradas" << endl;

    while (start != kickLetter.end()) {
        if (start->second.letter.erKick()) {
            totalEr += start->second.letter.qtdEr();
            cout << start->first << ": " << start->second.letter.qtdEr() << "X - ";
        }
        totalKick += start->second.letter.qtdTotalKick();
        ++start;
    }

    endl(cout);
    cout << "Total de erros: " << totalEr << endl;
    cout << "Total chutes: " << totalKick << endl;
}

void showMessageErrorKick(Letter *letter) {
    if (letter->qtd == 0) {
        kickLetter[letter->c].letter.incrementQtdEr();
        endl(cout);
        cout << "-:-: Você erro :-:-" << endl;
    }
}

void play() {
    showWord(guessHits, false);

    while (!isHang && !isWinner) {
        Letter letter = tryHit();
        showErKick();
        if (isHits(letter))
            replaceHits(letter);

        verifyWinner();

        if (!isWinner && !isHang) {
            cout << "Palavra sorteada: " << endl;
            showWord(guessHits, false);
        }
    }
}

void menuOption() {
    int op;
    cout << "Opções" << endl;
    cout << "1 - TRY_AGAIN" << endl;
    cout << "2 - ADD NEW WORD" << endl;
    cout << "3 - FINISH" << endl;
    cin >> op;

    executeSubMenu(op);
}

void executeSubMenu(int op) {
    switch (op) {
        case static_cast<int>(Option::TRY_AGAIN) :
            execute<TRY_AGAIN>();
            break;
        case static_cast<int>(Option::FINISH):
            execute<FINISH>();
            break;
        case static_cast<int>(Option::NEW_WORD):
            execute<NEW_WORD>();
            break;
        default:
            execute<DEFAULT>();
    }
}

void tryAgain() {
    secret_word = randWord(pathfile);
    string newGuessHist(secret_word.size(), '\0');
    guessHits = newGuessHist;
    reset();
}

void reset() {
    auto start = kickLetter.begin();

    while (start != kickLetter.end()) {
        start->second.letter = L();
        ++start;
    }
}

#endif
