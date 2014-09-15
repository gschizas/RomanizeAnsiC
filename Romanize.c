#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#ifdef __unix__ 
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#endif

const unsigned char *upperChars = "ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩΆΈΉΊΌΎΏΪΫΪΫΣABCDEFGHIJKLMNOPQRSTUVWXYZ";
const unsigned char *lowerChars = "αβγδεζηθικλμνξοπρστυφχψωάέήίόύώϊϋΐΰςabcdefghijklmnopqrstuvwxyz";
const unsigned char *simple_translation_greek = "άβδέζήιίϊΐκλνξόπρσςτυύϋΰφωώ";
const unsigned char *simple_translation_latin = "avdeziiiiiklnxoprsstyyyyfoo";
const unsigned char *digraph_translation_greek = "θχψ";
const unsigned char *digraph_translation_latin = "thchps";
const unsigned char *digraph_ypsilon_greek = "αεη";
const unsigned char *digraph_ypsilon_latin = "aei";
const unsigned char *digraph_ypsilon_beta = "βγδζλμνραάεέηήιίϊΐοόυύϋΰωώ";
const unsigned char *digraph_ypsilon_phi = "θκξπστφχψ";

#ifndef _MSC_VER

#define strcat_s(src, len, dst) strncat(src, dst, len)
#define strcpy_s(src, len, dst) strncpy(src, dst, len)

#endif q

#ifndef _WINDOWS_

#define SetConsoleOutputCP(cp) printf("Set code page to : %s", cp);

#endif


int strpos(unsigned char *haystack, unsigned char needle) {
    unsigned char *p = strchr(haystack, needle);
    if (p)
        return (int) (p - haystack);
    return -1;
}

bool charIn(unsigned char *haystack, unsigned char needle) {
    int idx = strpos(haystack, needle);
    return idx >= 0;
}

unsigned char toUpper(unsigned char inp) {
    int idx = strpos(lowerChars, inp);
    if (idx >= 0) {
        return upperChars[idx];
    } else return inp;
}

unsigned char toLower(unsigned char inp) {
    int idx = strpos(upperChars, inp);
    if (idx >= 0) {
        return lowerChars[idx];
    } else return inp;
}

bool isWhitespace(unsigned char third_letter) {
    return (third_letter <= 32 || third_letter == 160);
}

unsigned char *substring(unsigned char *string, int position, int length)
{
    unsigned char *pointer;
    unsigned char *source = string;
    int c;

    pointer = malloc(length + 1);

    if (pointer == NULL)
    {
        printf("Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    for (c = 0; c < position; c++)
        source++;

    for (c = 0; c < length; c++)
    {
        *(pointer + c) = *source;
        source++;
    }

    *(pointer + c) = '\0';

    return pointer;
}

unsigned char *romanize(unsigned char *greekText) {
#ifdef _DEBUG
    printf("%s = ", greekText);
#endif
    int greekLength = (int) strlen(greekText);
    int latinLength = greekLength * 3 + 1;
    unsigned char *result = malloc((size_t)latinLength);
    memset(result, 0, (size_t)latinLength);
    unsigned char letter, prev_letter, next_letter, third_letter;
    unsigned char *newLetter = malloc(4);
    bool is_upper, is_upper2;

    int cursor = 0;
    while (cursor < greekLength) {
        memset(newLetter, 0, (size_t)4);
        letter = greekText[cursor];

        prev_letter = (char) ((cursor > 0) ? greekText[cursor - 1] : '\0');
        next_letter = (char)((cursor < greekLength - 1) ? greekText[cursor + 1] : '\0');
        third_letter = (char)((cursor < greekLength - 2) ? greekText[cursor + 2] : '\0');

        is_upper = toUpper(letter) == letter;
        is_upper2 = toUpper(next_letter) == next_letter;

        letter = toLower(letter);
        prev_letter = toLower(prev_letter);
        next_letter = toLower(next_letter);
        third_letter = toLower(third_letter);

        if (charIn(simple_translation_greek, letter)) {
            int letterPosition = strpos(simple_translation_greek, letter);
            strcpy_s(newLetter, 4, substring(simple_translation_latin, letterPosition, 1));
        } else if (charIn(digraph_translation_greek, letter)) {
            int diphthong_index = strpos(digraph_translation_greek, letter);
            strcpy_s(newLetter, 4, substring(digraph_translation_latin, diphthong_index * 2, 2));
        } else if (charIn(digraph_ypsilon_greek, letter)) {
            int ypsilonPosition = strpos(digraph_ypsilon_greek, letter);
            unsigned char *newLetter2 = substring(digraph_ypsilon_latin, ypsilonPosition, 1);
            strcpy_s(newLetter, 4, newLetter2);
            free(newLetter2);
            if (next_letter == (unsigned char)'υ' || next_letter == (unsigned char)'ύ') {
                if (charIn(digraph_ypsilon_beta, third_letter)) {
                    strcat_s(newLetter, 4, "v");
                    cursor++;
                } else if (charIn(digraph_ypsilon_phi, third_letter)) {
                    strcat_s(newLetter, 4, "f");
                    cursor++;
                }
            }
        } else if (letter == (unsigned char)'γ') {
            if (next_letter == (unsigned char)'γ') {
                strcpy_s(newLetter, 4, "ng");
                cursor++;
            } else if (next_letter == (unsigned char)'ξ') {
                strcpy_s(newLetter, 4, "nx");
                cursor++;
            } else if (next_letter == (unsigned char)'χ') {
                strcpy_s(newLetter, 4, "nch");
                cursor++;
            } else {
                strcpy_s(newLetter, 4, "g");
            }
        } else if (letter == (unsigned char)'μ') {
            if (next_letter == (unsigned char)'π') {
                strcpy_s(newLetter, 4, isWhitespace(prev_letter) || isWhitespace(third_letter) ? "b" : "mp");
                cursor++;
            } else {
                strcpy_s(newLetter, 4, "m");
            }
        } else if (letter == (unsigned char)'ο') {
            strcpy_s(newLetter, 4, "o");
            if (next_letter == (unsigned char)'υ' || next_letter == (unsigned char)'ύ') {
                strcat_s(newLetter, 4, "u");
                cursor++;
            }
        } else {
            char letterStringBuff[] = { letter, '\0' };
            char *letterString = letterStringBuff;
            strcpy_s(newLetter, 4, letterString); //letter
        }

        // fix case
        if (is_upper) {
            char c1 = newLetter[0];
            char c2 = newLetter[1];
            char c3 = newLetter[2];

            c1 = toUpper(c1);

            if (is_upper2) {
                c2 = toUpper(c2);
                c3 = toUpper(c3);
            }

            char fixedCaseBuff[] = { c1, c2, c3, '\0' };
            char *fixedCaseString = fixedCaseBuff;
            strcpy_s(newLetter, 4, fixedCaseString);
        }

        strcat_s(result, latinLength, newLetter);
        cursor++;
    }
    free(newLetter);
#ifdef _DEBUG
    printf("%s\n", result);
#endif
    return result;
}


int main() {
#ifdef _WINDOWS_
    SetConsoleOutputCP(1253);
#endif

    //printf("%c", toUpper('γ'));
    //printf("%c", toUpper('Γ'));
    //printf("%c", toUpper('g'));
    //printf("%c", toLower('Γ'));
    //printf("%c", toLower('γ'));
    //printf("%c", toLower('G'));
    //printf("%c", toLower('υ'));

    //printf("\r\n");
    //printf(substring("abcd", 0, 1));
    //printf(substring("abcd", 1, 1));
    //printf(substring("abcd", 2, 1));
    //printf(substring("abcd", 3, 1));
    //printf("\r\n");
    //printf(substring("abcd", 2, 2));

    //printf("\r\n");
    //printf("%d", strpos("αβγ", 'α'));
    //printf("\r\n");

    //// printf("%s", romanize("κόσμε!\n"));
    //printf("%s", romanize("Γειά σου κόσμε!\n"));

    assert(strcmp(romanize("Γειά σου κόσμε!"), "Geia sou kosme!") == 0);
    assert(strcmp(romanize("Γιώργος Σχίζας"), "Giorgos Schizas") == 0);
    assert(strcmp(romanize("Θανάσης ΘΑΝΑΣΗΣ θΑνάσης ΘΑνάσης"), "Thanasis THANASIS thAnasis THAnasis") == 0);
    assert(strcmp(romanize("Αντώνης Ψαράς με ψάρια"), "Antonis Psaras me psaria") == 0);
    assert(strcmp(romanize("Αυγά αύριο παύση"), "Avga avrio pafsi") == 0);
    assert(strcmp(romanize("Άγγελος αρχάγγελος"), "Angelos archangelos") == 0);
    assert(strcmp(romanize("Ξάδελφος εξ αγχιστείας"), "Xadelfos ex anchisteias") == 0);
    assert(strcmp(romanize("Ακούμπα κάτω τα μπαούλα Γιακούμπ"), "Akoumpa kato ta baoula Giakoub") == 0);
    assert(strcmp(romanize("Ζεύξη Ρίου-Αντιρρίου"), "Zefxi Riou-Antirriou") == 0);
    assert(strcmp(romanize("μεταγραφή"), "metagrafi") == 0);
    assert(strcmp(romanize("Ούτε το αγγούρι ούτε η αγκινάρα γράφονται με γξ"), "Oute to angouri oute i agkinara grafontai me nx") == 0);
    assert(strcmp(romanize("ΟΥΡΑΝΟΣ Ουρανός ουρανός οϋρανός"), "OURANOS Ouranos ouranos oyranos") == 0);
    assert(strcmp(romanize("Έχω ελέγξει το 100% της μεθόδου"), "Echo elenxei to 100% tis methodou") == 0);
    // free(resultFinal);

    return 0;
}
