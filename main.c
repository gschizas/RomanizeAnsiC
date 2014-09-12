#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* upper_greek = "ΑΒΓΔΕΖΗΘΙΚΛΜΝΞΟΠΡΣΤΥΦΧΨΩΆΈΉΊΌΎΏΪΫΪΫΣ";
const char* lower_greek = "αβγδεζηθικλμνξοπρστυφχψωάέήίόύώϊϋΐΰς";

int strpos(char *haystack, char needle)
{
    char *p = strchr(haystack, needle);
    if (p)
        return p - haystack;
    return -1;
}

int charIn(char *haystack, char needle) {
    int idx = strpos(haystack, needle);
    return idx >= 0;
}

char toUpperGreek(char inp) {
    int idx = strpos(lower_greek, inp);
    if (idx >= 0) {
        return upper_greek[idx];
    }
    else return inp;
}

char toLowerGreek(char inp) {
    int idx = strpos(upper_greek, inp);
    if (idx >= 0) {
        return lower_greek[idx];
    }
    else return inp;
}


char* romanize(char* input_text) {
    const simple_translation_greek = "άβδέζήιίϊΐκλνξόπρσςτυύϋΰφωώ";
    const simple_translation_latin = "avdeziiiiiklnxoprsstyyyyfoo";
    const digraph_translation_greek = "θχψ";
    const digraph_translation_latin = "thchps";
    const digraph_ypsilon_greek = "αεη";
    const digraph_ypsilon_latin = "aei";
    const digraph_ypsilon_beta = "βγδζλμνραάεέηήιίϊΐοόυύϋΰωώ";
    const digraph_ypsilon_phi = "θκξπστφχψ";

    int result_length = strlen(input_text) * 3 + 1;
    char* result = malloc(result_length);
    memset(result, 0, result_length);
    char lett, prev_letter, next_letter, third_letter;
    char* newlett = malloc(3);
    int is_upper, is_upper2;

    int cursor = 0;
    while (input_text[cursor] != 0) {
        char letter = input_text[cursor];
        // if cursor char prev_letter = input_text[cursor + 1];
        printf("%c", letter);
        /*if (strchr(simple_translation_greek, letter)):
        newlett = simple_translation_latin[simple_translation_greek.index(letter)];
        else:
        */
        newlett[0] = toUpperGreek(letter);
        newlett[1] = '\0';

        // strcpy_s(newlett, 100, (char const *)letter);

        strcat(result, newlett);
        cursor++;
    }
    // while cursor < len(input_text)
    return result;
}


int main()
{

    char resultUpper1 = toUpperGreek('γ');
    printf("%c", resultUpper1);

    char resultUpper2 = toUpperGreek('Γ');
    printf("%c", resultUpper2);

    char resultUpper3 = toUpperGreek('g');
    printf("%c", resultUpper3);

    char resultLower1 = toLowerGreek('Γ');
    printf("%c", resultLower1);

    char resultLower2 = toLowerGreek('γ');
    printf("%c", resultLower2);

    char resultLower3 = toLowerGreek('G');
    printf("%c", resultLower3);

    printf("\n");

    char* resultFinal = romanize("Γειά σου κόσμε!\n");
    printf("%s", resultFinal);
    free(resultFinal);
    resultFinal = NULL;

    return 0;
}
