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
//const string &filePath = "../secret.txt";
const string &filePath = "secret.txt";

typedef enum OptionMenu {
    TRY_AGAIN = 1, NEW_WORD = 2, FINISH = 3, DEFAULT = 0
} Option;

typedef struct LetterKickStruct {
private:
    int qtdErKick{};
    int qtdKick{};
public:
    LetterKickStruct(): qtd(0), qtdErKick(0), c('\0'), kick(false), qtdKick(0) {}

    [[maybe_unused]] LetterKickStruct(int qtd, char c, bool kick): qtd(qtd), qtdErKick(0), c(c), kick(kick), qtdKick(
            0) {
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

struct LetterStruct {
    Letter letter;
};

typedef map<char, LetterStruct> MapperLetter;
MapperLetter kickLetter;

typedef struct wordStruct {
private:
    int qtdHang;

    void showFinish();

public:
    wordStruct();

    [[maybe_unused]] explicit wordStruct(int qtdHang);

    const string secretWord = randWord(filePath);
    string guessWord{};

    [[nodiscard]] bool isWinner() const;

    [[nodiscard]] bool isHanged() const;

    [[nodiscard]] bool isWinOrHang() const;


    void showWinnerWord();

    void showGuessWord();

    void showGuessOrWinnerWord();

    void hanged();
} Word;

wordStruct::wordStruct() {
    string sec = string(this->secretWord.size(), '\0');
    this->guessWord = sec;
    this->qtdHang = 5;
}


[[maybe_unused]] wordStruct::wordStruct(int qtdHang) {
    string sec = string(this->secretWord.size(), '\0');
    this->guessWord = sec;
    this->qtdHang = qtdHang;
}

bool wordStruct::isWinner() const {
    return this->guessWord == this->secretWord;
}

bool wordStruct::isHanged() const {
    return this->qtdHang == 0;
}

void wordStruct::hanged() {
    this->qtdHang--;
}

void wordStruct::showFinish() {
    if (this->isWinOrHang()) {
        cout << "Parabésn você ganhou" << endl;
        this->showWinnerWord();
    } else {
        cout << "Você perdeu" << endl;
    }
}

void wordStruct::showWinnerWord() {
    for (char s: this->secretWord) {
        cout << s << " ";
    }
    endl(cout);
}

void wordStruct::showGuessWord() {
//    cout << "Guess word" << endl;
    for (char s: this->secretWord) {
        if (s == '\0')
            cout << " _ ";
        else
            cout << s << " ";
    }
    endl(cout);
}

bool wordStruct::isWinOrHang() const {
    return this->isWinner() || this->isHanged();
}

void wordStruct::showGuessOrWinnerWord() {
    if (this->isWinOrHang())
        this->showFinish();
    else
        this->showGuessWord();
}


bool LetterKickStruct::erKick() const {
    return this->kick && this->qtd == 0;
}

void LetterKickStruct::incrementQtdEr() {
    if (this->erKick()) this->qtdErKick++;
}

int LetterKickStruct::qtdEr() const {
    return this->qtdErKick;
}

int LetterKickStruct::qtdTotalKick() const {
    return this->qtdKick;
}

void LetterKickStruct::incrementKick() {
    this->qtdKick++;
}

void isVerifyKick(char guess, Letter &letter, Word &word);

void verifyCaracter(Letter &letter, Word &word);

Letter tryHitAndVerify(Word &word);

bool isHits(const Letter &letter);

void showErKick();

void showMessageErrorKick(Letter &letter);

inline void play();

char upperWord(char guess);

inline void menuOption();

void executeSubMenu(int op);

Word tryAgain();

Word tryAgainAndReset();

void reset(MapperLetter &letter);

void replaceHits(Letter &letter, Word &word);

bool isKick(char guess);

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
    addNewWord(filePath, newWord);
}

template<>
void OpSubMenu<FINISH>::execute() {
    cout << "Fim de jogo" << endl;
    exit(EXIT_SUCCESS);
}

template<>
void OpSubMenu<TRY_AGAIN>::execute() {
    tryAgainAndReset();
}

template<>
void OpSubMenu<DEFAULT>::execute() {
    cout << "DEfault" << endl;
    menuOption();
}

template<Option _t>
void execute() {
    OpSubMenu<_t> opSubMenu;
    opSubMenu.execute();
}

void verifyCaracter(Letter &letter, Word &word) {
    for (char i: word.secretWord)
        if (i == letter.c)
            letter.qtd++;

    kickLetter[letter.c].letter = {letter.qtd, letter.c, true};
    showMessageErrorKick(letter);
}

Letter tryHitAndVerify(Word &word) {
    Letter letter;
    char guess{};
    cout << "Digite uma letra" << endl;
    cin >> guess;

    guess = upperWord(guess);
    isVerifyKick(guess, letter, word);
    return letter;
}

char upperWord(char guess) { return char(toupper((unsigned char) guess)); }

void isVerifyKick(char guess, Letter &letter, Word &word) {
    if (isKick(guess)) {
        kickLetter[guess].letter.incrementQtdEr();
        kickLetter[guess].letter.incrementKick();
        word.hanged();
        endl(cout);
        cout << "====== Letra já chutada ======" << endl;
        endl(cout);
    } else {
        letter.c = guess;
        verifyCaracter(letter, word);
    }

}

bool isKick(char guess) {
    return kickLetter[guess].letter.kick;
}

bool isHits(const Letter &letter) {
    return letter.qtd > 0;
}

void replaceHits(Letter &letter, Word &word) {
    const string &secretWord = word.secretWord;
    string &guessWord = word.guessWord;

    for (int i = 0; i < secretWord.size(); ++i) {
        char caracter = upperWord(letter.c);
        char secWord = secretWord[i];
        if (secWord == caracter)
            guessWord[i] = caracter;

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

void showMessageErrorKick(Letter &letter) {
    if (letter.qtd == 0) {
        kickLetter[letter.c].letter.incrementQtdEr();
        endl(cout);
        cout << "-:-: Você erro :-:-" << endl;
    }
}

inline void play() {
    Word wordGame;

    wordGame.showGuessOrWinnerWord();

    while (!wordGame.isWinOrHang()) {
        Letter letter = tryHitAndVerify(wordGame);
        showErKick();
        if (isHits(letter))
            replaceHits(letter, wordGame);

        wordGame.showGuessOrWinnerWord();

        if (!wordGame.isWinOrHang()) {
            cout << "Palavra sorteada: " << endl;
            wordGame.showGuessWord();
        }
    }
}

inline void menuOption() {
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

Word tryAgain() {
    return {};
}

Word tryAgainAndReset() {
    reset(kickLetter);
    return tryAgain();
}

void reset(MapperLetter &letter) {
    auto start = letter.begin();

    while (start != letter.end()) {
        start->second.letter = LetterKickStruct();
        ++start;
    }
}

#endif
