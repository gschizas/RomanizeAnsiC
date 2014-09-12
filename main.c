#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>

const char *upperChars = "ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩΆΈΉΊΌΎΏΪΫΪΫΣABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char *lowerChars = "αβγδεζηθικλμνξοπρστυφχψωάέήίόύώϊϋΐΰςabcdefghijklmnopqrstuvwxyz";
const char *simple_translation_greek = "άβδέζήιίϊΐκλνξόπρσςτυύϋΰφωώ";
char const **simple_translation_latin = "avdeziiiiiklnxoprsstyyyyfoo";
const char *digraph_translation_greek = "θχψ";
const char *digraph_translation_latin = "thchps";
const char *digraph_ypsilon_greek = "αεη";
const char *digraph_ypsilon_latin = "aei";
const char *digraph_ypsilon_beta = "βγδζλμνραάεέηήιίϊΐοόυύϋΰωώ";
const char *digraph_ypsilon_phi = "θκξπστφχψ";

#ifndef _MSC_VER

#define strcat_s(src, len, dst) strncat(src, dst, len)
#define strcpy_s(src, len, dst) strncpy(src, dst, len)

#endif

#ifndef _WINDOWS_

#define SetConsoleOutputCP(cp) printf("Set code page to : %s", cp);

#endif


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
    int greekTextLength = (int) strlen(greekText);
    int result_length = greekTextLength * 3 + 1;
    char *result = malloc((size_t) result_length);
    memset(result, 0, (size_t) result_length);
    char letter, prev_letter, next_letter, third_letter;
    char newLetterBuff[4];
    char *newLetter = newLetterBuff;
    int is_upper, is_upper2;

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
            strcpy_s(newLetter, 4, simple_translation_latin[strpos((char *)simple_translation_greek, letter)]);
        } else if (charIn((char *) digraph_translation_greek, letter)) {
            int diphthong_index = strpos(digraph_translation_greek, letter);
            strcpy_s(newLetter, 4, (digraph_translation_latin[diphthong_index * 2] + digraph_translation_latin[diphthong_index * 2 + 1] + '\0'));
        } else if (charIn((char *) digraph_ypsilon_greek, letter)) {
            int ypsilonPosition = strpos((char *) digraph_ypsilon_greek, letter);
            strcpy_s(newLetter, 4, digraph_ypsilon_latin[ypsilonPosition]);
            if (next_letter == 'υ' || next_letter == 'ύ') {
                if (charIn((char *) digraph_ypsilon_beta, third_letter)) {
                    strcat_s(newLetter, 4, "v");
                    cursor++;
                } else if (charIn((char *) digraph_ypsilon_phi, third_letter)) {
                    strcat_s(newLetter, 4, "f");
                    cursor++;
                }
            }
        } else if (letter == 'γ') {
            if (next_letter == 'γ') {
                strcpy_s(newLetter, 4, "ng");
                cursor++;
            } else if (next_letter == 'ξ') {
                strcpy_s(newLetter, 4, "nx");
                cursor++;
            } else if (next_letter == 'χ') {
                strcpy_s(newLetter, 4, "nch");
                cursor++;
            } else {
                strcpy_s(newLetter, 4, "g");
            }
        } else if (letter == 'μ') {
            if (next_letter == 'π') {
                strcpy_s(newLetter, 4, isWhitespace(prev_letter) || isWhitespace(third_letter) ? "b" : "mp");
                cursor++;
            } else {
                strcpy_s(newLetter, 4, "m");
            }
        } else if (letter == 'ο') {
            strcpy_s(newLetter, 4, "o");
            if (next_letter == 'υ' || next_letter == 'ύ') {
                strcat_s(newLetter, 4, "u");
                cursor++;
            }
        } else {
            strcpy_s(newLetter, 4, (char *)(letter + '\0'));
        }

        /*
        char c1 = newLetterBuff[0];
        c1 = is_upper ? toUpper(c1) : toLower(c1);
        char c2 = newLetterBuff[1];
        c2 = is_upper ? toUpper(c2) : toLower(c2);
        char c3 = newLetterBuff[2];
        c3 = is_upper ? toUpper(c3) : toLower(c3);

        newLetterBuff[0] = c1;
        newLetterBuff[1] = c2;
        newLetterBuff[2] = c3;
        */

        strcat_s(result, greekTextLength, newLetter);
        cursor++;
    }
    return result;
}


int main() {
    SetConsoleOutputCP(1253);

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

    printf("\r\n");

    char* resultFinal = "Γειά σου κόσμε!\n"; //romanize("Γειά σου κόσμε!\n");
    printf("%s", resultFinal);
    // free(resultFinal);

    return 0;
}
