#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const char *upperChars = "ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩΆΈΉΊΌΎΏΪΫΪΫΣABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *lowerChars = "αβγδεζηθικλμνξοπρστυφχψωάέήίόύώϊϋΐΰςabcdefghijklmnopqrstuvwxyz";
const char *simple_translation_greek = "άβδέζήιίϊΐκλνξόπρσςτυύϋΰφωώ";
const char *simple_translation_latin = "avdeziiiiiklnxoprsstyyyyfoo";
const char *digraph_translation_greek = "θχψ";
const char *digraph_translation_latin = "thchps";
const char *digraph_ypsilon_greek = "αεη";
const char *digraph_ypsilon_latin = "aei";
const char *digraph_ypsilon_beta = "βγδζλμνραάεέηήιίϊΐοόυύϋΰωώ";
const char *digraph_ypsilon_phi = "θκξπστφχψ";

int strpos(char *haystack, char needle) {
    char *p = strchr(haystack, needle);
    if (p)
        return (int) (p - haystack);
    return -1;
}

bool charIn(char *haystack, char needle) {
    int idx = strpos(haystack, needle);
    return idx >= 0;
}

char toUpper(char inp) {
    int idx = strpos((char *) lowerChars, inp);
    if (idx >= 0) {
        return upperChars[idx];
    } else return inp;
}

char toLower(char inp) {
    int idx = strpos((char *) upperChars, inp);
    if (idx >= 0) {
        return lowerChars[idx];
    } else return inp;
}

bool isWhitespace(char third_letter) {
    return (third_letter <= 32 || third_letter == 160);
}

char *romanize(char *greekText) {

    int result_length = (int) (strlen(greekText) * 3 + 1);
    char *result = malloc((size_t) result_length);
    memset(result, 0, (size_t) result_length);
    char letter, prev_letter, next_letter, third_letter;
    char *newLetter = malloc(4);
    int is_upper, is_upper2;
    int greekTextLength = (int) strlen(greekText);

    int cursor = 0;
    while (cursor < greekTextLength) {
        letter = greekText[cursor];

        prev_letter = (char) ((cursor > 0) ? greekText[cursor - 1] : '\0');
        next_letter = (char) ((cursor < greekTextLength - 1) ? greekText[cursor + 1] : '\0');
        third_letter = (char) ((cursor < greekTextLength - 2) ? greekText[cursor + 2] : '\0');

        is_upper = toUpper(letter) == letter;
        is_upper2 = toUpper(next_letter) == next_letter;

        letter = toLower(letter);
        prev_letter = toLower(prev_letter);
        next_letter = toLower(next_letter);
        third_letter = toLower(third_letter);

        printf("%c", letter);
        if (charIn((char *) simple_translation_greek, letter)) {
            newLetter = (char *) simple_translation_latin[strpos((char *) simple_translation_greek, letter)];
        } else if (charIn((char *) digraph_translation_greek, letter)) {
            int diphthong_index = strpos((char *) digraph_translation_greek, letter);
            newLetter = (char *) (digraph_translation_latin[diphthong_index * 2] +
                    digraph_translation_latin[diphthong_index * 2 + 1] + '\0');
        } else if (charIn((char *) digraph_ypsilon_greek, letter)) {
            int ypsilonPosition = strpos((char *) digraph_ypsilon_greek, letter);
            newLetter = (char *) digraph_ypsilon_latin[ypsilonPosition];
            if (next_letter == 'υ' || next_letter == 'ύ') {
                if (charIn((char *) digraph_ypsilon_beta, third_letter)) {
                    strcat(newLetter, "v");
                    cursor++;
                } else if (charIn((char *) digraph_ypsilon_phi, third_letter)) {
                    strcat(newLetter, "f");
                    cursor++;
                }
            }
        } else if (letter == 'γ') {
            if (next_letter == 'γ') {
                newLetter = "ng";
                cursor++;
            } else if (next_letter == 'ξ') {
                newLetter = "nx";
                cursor++;
            } else if (next_letter == 'χ') {
                newLetter = "nch";
                cursor++;
            } else {
                newLetter = "g";
            }
        } else if (letter == 'μ') {
            if (next_letter == 'π') {
                newLetter = isWhitespace(prev_letter) || isWhitespace(third_letter) ? "b" : "mp";
                cursor++;
            } else {
                newLetter = "m";
            }
        } else if (letter == 'ο') {
            newLetter = "o";
            if (next_letter == 'υ' || next_letter == 'ύ') {
                strcat(newLetter, "u");
                cursor++;
            }
        } else {
            newLetter = (char *) (letter + '\0');
        }

        char c1 = newLetter[0];
        c1 = is_upper ? toUpper(c1) : toLower(c1);
        char c2 = newLetter[1];
        c2 = is_upper ? toUpper(c2) : toLower(c2);
        char c3 = newLetter[2];
        c3 = is_upper ? toUpper(c3) : toLower(c3);

        newLetter[0] = c1;
        newLetter[1] = c2;
        newLetter[2] = c3;

        strcat(result, newLetter);
        cursor++;
    }
    return result;
}


int main() {

    char resultUpper1 = toUpper((char) 'γ');
    printf("%c", resultUpper1);

    char resultUpper2 = toUpper((char) 'Γ');
    printf("%c", resultUpper2);

    char resultUpper3 = toUpper('g');
    printf("%c", resultUpper3);

    char resultLower1 = toLower((char) 'Γ');
    printf("%c", resultLower1);

    char resultLower2 = toLower((char) 'γ');
    printf("%c", resultLower2);

    char resultLower3 = toLower('G');
    printf("%c", resultLower3);

    printf("\n");

    char *resultFinal = romanize("Γειά σου κόσμε!\n");
    printf("%s", resultFinal);
    free(resultFinal);
    resultFinal = NULL;

    return 0;
}
