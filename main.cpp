#include "forca.h"
#include "demo.h"

void foo();

int main() {
    play();

    return 0;
}

void foo() {
    showWord(secret_word, false);
    showWord(guessHits, false);
}
