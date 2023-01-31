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
//const string &filePath = "secret.txt";

typedef enum OptionMenu {
    TRY_AGAIN = 1, NEW_WORD = 2, FINISH = 3, DEFAULT = 0
} Option;

typedef struct LetterKickStruct {
private:
    int qtdErKick{};
    int qtdKick{};
public:
    LetterKickStruct(): qtd(0), qtdErKick(0), c('\0'), kick(false), qtdKick(0) {
        this->incrementKick();
    }

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
    string filePath;

    void showFinish();

public:
    [[maybe_unused]] explicit wordStruct(const string& filePath);

    [[maybe_unused]] explicit wordStruct(const string& filePath, int qtdHang);

    string secretWord{};
    string guessWord{};

    [[nodiscard]] bool isWinner() const;

    [[nodiscard]] bool isHanged() const;

    [[nodiscard]] bool isWinOrHang() const;

    void showWinnerWord();

    void showGuessWord();

    void showGuessOrWinnerWord();

    void hanged();

    string getPathFile();
} Word;

[[maybe_unused]] wordStruct::wordStruct(const string& filePath) {
    this->secretWord =  randWord(filePath);
    this->guessWord = string(this->secretWord.size(), '\0');
    this->qtdHang = 5;
    this->filePath = filePath;
}


[[maybe_unused]] wordStruct::wordStruct(const string& filePath, int qtdHang) {
    this->secretWord =  randWord(filePath);
    string sec = string(this->secretWord.size(), '\0');
    this->guessWord = sec;
    this->qtdHang = qtdHang;
    this->filePath = filePath;
}

bool wordStruct::isWinner() const {
    return this->guessWord == this->secretWord;
}

bool wordStruct::isHanged() const {
    return this->qtdHang <= 0;
}

void wordStruct::hanged() {
    this->qtdHang--;
}

void wordStruct::showFinish() {
    if (this->isWinner()) {
        cout << "Parabésn você ganhou" << endl;
        this->showWinnerWord();
    } else {
        cout << "Você perdeu" << endl;
        this->showWinnerWord();
    }
}

void wordStruct::showWinnerWord() {
    for (char s: this->secretWord) {
        cout << s << " ";
    }
    endl(cout);
}

void wordStruct::showGuessWord() {
    for (char s: this->guessWord) {
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

string wordStruct::getPathFile() {
    return this->filePath;
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

void menuOption(Word &word);

void executeSubMenu(int op, Word &word);

Word tryAgainAndReset(Word &word);

void reset(MapperLetter &letter);

void replaceHits(Letter &letter, Word &word);

bool isKick(char guess);

void wordGuess(Word &wordGame);

template<Option _t>
class OpSubMenu {
public:
    void execute(Word &word);
};

template<>
void OpSubMenu<NEW_WORD>::execute(Word &word) {
    string newWord;
    cout << "Informe a nova palavra" << endl;
    cin >> newWord;
    addNewWord(word.getPathFile(), newWord);
}

template<>
void OpSubMenu<FINISH>::execute(Word &word) {
    cout << "Fim de jogo" << endl;
    word.showWinnerWord();
    exit(EXIT_SUCCESS);
}

template<>
void OpSubMenu<TRY_AGAIN>::execute(Word &word) {
    word = tryAgainAndReset(word);
    wordGuess(word);
}

template<>
void OpSubMenu<DEFAULT>::execute(Word &word) {
    cout << "Default" << endl;
    menuOption(word);
}

template<Option _t>
void execute(Word &word) {
    OpSubMenu<_t> opSubMenu;
    opSubMenu.execute(word);
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
    const string filePath = "secret.txt";
    Word wordGame{filePath};
    wordGuess(wordGame);
}

void wordGuess(Word &wordGame) {
    wordGame.showGuessOrWinnerWord();
    while (!wordGame.isWinOrHang()) {
        Letter letter = tryHitAndVerify(wordGame);
        showErKick();
        if (isHits(letter))
            replaceHits(letter, wordGame);

        wordGame.showGuessOrWinnerWord();
        if (wordGame.isWinOrHang()) {
            menuOption(wordGame);
        }
    }
}

void menuOption(Word &word) {
    int op;
    cout << "Opções" << endl;
    cout << "1 - TRY_AGAIN" << endl;
    cout << "2 - ADD NEW WORD" << endl;
    cout << "3 - FINISH" << endl;
    cin >> op;

    executeSubMenu(op, word);
}

void executeSubMenu(int op, Word &word) {
    switch (op) {
        case static_cast<int>(Option::TRY_AGAIN) :
            execute<TRY_AGAIN>(word);
            break;
        case static_cast<int>(Option::FINISH):
            execute<FINISH>(word);
            break;
        case static_cast<int>(Option::NEW_WORD):
            execute<NEW_WORD>(word);
            break;
        default:
            execute<DEFAULT>(word);
    }
}

Word tryAgainAndReset(Word &word) {
    reset(kickLetter);
    return Word{word.getPathFile()};
}

void reset(MapperLetter &letter) {
    auto start = letter.begin();

    while (start != letter.end()) {
        start->second.letter = LetterKickStruct();
        ++start;
    }
}

#endif
