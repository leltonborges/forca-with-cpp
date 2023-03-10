#ifndef FORCA_READFILE_H
#define FORCA_READFILE_H

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

vector<string> readFile(const string &filePath);

ifstream openFileRead(const string &pathfile);

long countLineFile(const string &pathfile);

string randWord(const string &pathfile);

void addNewWord(const string &filePath, const string& newWord);

void isOpen(const ifstream &file);

void isOpen(const ofstream &file);

ofstream openFileWriter(const string &path);

string randWord(const string &pathfile) {
    const vector<string> &words = readFile(pathfile);
    srand(time(NULL));
    long randNum = rand() % words.size();
    return words[randNum];
}

long countLineFile(const string &pathfile) {
    ifstream file(pathfile);
    if (!file.is_open()) {
        cerr << "file is close" << endl;
        exit(EXIT_FAILURE);
    }
    return std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
}

ifstream openFileRead(const string &pathfile) {
    ifstream file(pathfile);
    if (!file) {
        cerr << "Can't open input file" << endl;
        exit(EXIT_FAILURE);
    }
    return file;
}

ofstream openFileWriter(const string &pathfile) {
    ofstream file(pathfile);
    if (!file) {
        cerr << "Can't open input file" << endl;
        exit(EXIT_FAILURE);
    }
    return file;
}

vector<string> readFile(const string &filePath) {
    vector<string> words;
    string line;
    ifstream file = openFileRead(filePath);

    while (getline(file, line)) {
        words.push_back(line);
    }
    file.close();

    return words;
}

void addNewWord(const string &filePath, const string& newWord) {
    vector<string> words = readFile(filePath);
    ofstream file = openFileWriter(filePath);
    isOpen(file);
    words.push_back(newWord);
    for (const string &s: words) {
        file << s << endl;
    }
}

void isOpen(const ifstream &file) {
    if (!file.is_open()) {
        cerr << "file is close" << endl;
        exit(EXIT_FAILURE);
    }
}

void isOpen(const ofstream &file) {
    if (!file.is_open()) {
        cerr << "file is close" << endl;
        exit(EXIT_FAILURE);
    }
}

#endif //FORCA_READFILE_H
