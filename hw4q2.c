
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_WORD_LEN 32
#define N_NOUNS 6
#define N_VERBS 5
#define N_ADJS 6
#define N_SUFFIXES (N_NOUNS + N_VERBS + N_ADJS)
#define PERIOD 1
#define ERROR -2

bool is_suffix_in_dict(char *str, char *dict[], int n);
int StringLength(char *str);
void SelfConvertToLowercase(char *string);
void ConvertToLowercase(char *string, char *lowercase_str);
void RemoveNonLetters(char *string_ptr);
void CopyString(char *source, char *destination);
void DeleteChar(char *string);
int WordCompare(char *flipped_word, char *suffix);
void MergeSuffixLists(char *list_1[], int len_1, char *list_2[], int len_2,
                       char *merged_list[]);
void FlipString(char *orig_str, char *flipped_str, int str_len);
int ReadWord(char *noun_suffixes[], int noun_suffixes_len,
                char *verb_suffixes[], int verb_suffixes_len,
                char *adj_suffixes[], int adj_suffixes_len,
                int *num_of_nouns, int *num_of_verbs, int *num_of_adjs);
bool read_sentence(char *noun_suffixes[], int noun_suffixes_len,
                   char *verb_suffixes[], int verb_suffixes_len,
                   char *adj_suffixes[], int adj_suffixes_len,
                   int *num_of_nouns, int *num_of_verbs, int *num_of_adjs);
void ReadAllText(char *noun_suffixes[], int noun_suffixes_len,
                   char *verb_suffixes[], int verb_suffixes_len,
                   char *adj_suffixes[], int adj_suffixes_len,
                   int *num_of_nouns, int *num_of_verbs, int *num_of_adjs);

int main()
{
    char *noun_suffixes[] = {"msi", "re", "sci", "ssen", "tnem", "tsi"};
    char *verb_suffixes[] = {"de", "eta", "ezi", "gni", "yfi"};
    char *adj_suffixes[] = {"elba", "evi", "hsi", "la", "luf", "suo"};

    ///necessary?
    int noun_dict_len = N_NOUNS, verb_dict_len = N_VERBS, adj_dict_len = N_ADJS;

    int num_of_nouns = 0, num_of_verbs = 0, num_of_adjs = 0;

    int *num_nouns_ptr = &num_of_nouns;
    int *num_verbs_ptr = &num_of_verbs;
    int *num_adjs_ptr = &num_of_adjs;

    ReadAllText(noun_suffixes, noun_dict_len, verb_suffixes, verb_dict_len,
             adj_suffixes, adj_dict_len, num_nouns_ptr, num_verbs_ptr,
             num_adjs_ptr);

    return 0;
}

int StringLength(char *string_ptr) {
    int length = 0;
    while (*string_ptr != '\0') {
        length++;
        string_ptr++;
    }

    return length;
}

void SelfConvertToLowercase(char *string) {
    while (*string != '\0') {
        if (*string >= 'A' && *string <= 'Z') {
            char temp = *string;
            temp += ('z' - 'Z');
            *string = temp;
        }

        string++;
    }
}

void RemoveNonLetters(char *string) {
    while (*string != '\0') {
        if ((*string > 'z' || *string < 'a') && (*string != '.' )) {
            DeleteChar(string);
        } else {
            string++;
        }
    }
}


void DeleteChar(char *string) {
    for (char *temp_ptr = string; *temp_ptr != '\0'; temp_ptr++) {
        *temp_ptr = *(temp_ptr + 1);
    }
}

void FlipString(char *orig_str, char *flipped_str, int str_len) {
    for (int i = str_len; i > 0; i--) {
        *(flipped_str + (str_len - i)) = *(orig_str + i - 1);
    }
    *(flipped_str + str_len) = '\0';
}

/*void CopyString(char *source, char *destination) {
    if (source != NULL && destination != NULL) {
        while (*source != '\0') {
            *(destination++) = *(source++);
        }
    }
}*/


void MergeSuffixLists(char *list_1[], int len_1, char *list_2[], int len_2,
                       char *merged_list[]) {
    int ia, ib, ic, comp_return_val;
    ia = ib = ic = 0;
    for (ia = ib = ic = 0; ia < len_1 && ib < len_2; ic++) {
        comp_return_val = WordCompare(*(list_1 + ia), *(list_2 + ib));
        if (comp_return_val == -1 || comp_return_val == -2) {
            *(merged_list + ic) = *(list_1 + ia);
            ia++;
        } else {
            *(merged_list + ic) = *(list_2 + ib);
            ib++;
        }
    }

    for(; ia < len_1; ia++, ic++) {
        *(merged_list + ic) = *(list_1 + ia);
    }

    for (; ib < len_2; ib++, ic++) {
        *(merged_list + ic) = *(list_2 + ib);
    }
}


int WordCompare(char *flipped_word, char *suffix) {

    char processed_suffix[MAX_WORD_LEN + 1];
    ///Change to own implementation
    strcpy(processed_suffix, suffix);
    SelfConvertToLowercase(processed_suffix);
    RemoveNonLetters(processed_suffix);
    char *suf_ptr = processed_suffix;

    while (*flipped_word != '\0') {
        if (*flipped_word == '.') {
            flipped_word++;
        }


        if (*suf_ptr == '\0') {
            return 2;
        }

        if (*flipped_word > *suf_ptr) {
            return 1;
        }

        if (*suf_ptr > *flipped_word) {
            return -1;
        }

        flipped_word++;
        suf_ptr++;
    }
    /*If the while loop ends (i.e., string_1 ends) before suffix ends,
    suffix is greater.*/
    if (*suf_ptr != '\0') {
        return -2;
    }

    return 0;
}

//N is dict length
bool is_suffix_in_dict(char *str, char *dict[], int n) {
    int low = 0, high = n, mid, ret;
    ///WHY LESS AND NOT LESS/EQUALS?
    while (low <= high && low < n) {

        mid = low + (high - low)/2;
        ret = WordCompare(str, *(dict + mid));
        if (ret == 0 || ret == 2) {
            return true;
        }

        if (ret == 1) {
            low = mid + 1;
        }

        if (ret == -1 || ret == -2) {
            high = mid - 1;
        }
    }

    return false;
}

///Error when last three letters are 't' or greater???
int ReadWord(char *noun_suffixes[], int noun_suffixes_len,
                char *verb_suffixes[], int verb_suffixes_len,
                char *adj_suffixes[], int adj_suffixes_len,
                int *num_of_nouns, int *num_of_verbs, int *num_of_adjs) {

    char word[MAX_WORD_LEN + 1], flipped_word[MAX_WORD_LEN + 1];
    int scanf_ret_val = scanf("%s", word);
    if (scanf_ret_val == EOF) {
        return EOF;
    }

    if (scanf_ret_val != 1) {
        return ERROR;
    }

    SelfConvertToLowercase(word);
    RemoveNonLetters(word);
    int str_len = StringLength(word);

    FlipString(word, flipped_word, str_len);

    if (is_suffix_in_dict(flipped_word, noun_suffixes, noun_suffixes_len)) {
        *(num_of_nouns) = *(num_of_nouns) + 1;
    } else if (is_suffix_in_dict(flipped_word, verb_suffixes, verb_suffixes_len)) {
        *(num_of_verbs) = *(num_of_verbs) + 1;
    } else if (is_suffix_in_dict(flipped_word, adj_suffixes, adj_suffixes_len)) {
        *(num_of_adjs) = *(num_of_adjs) + 1;
    }

    if (*flipped_word == '.') {
        return PERIOD;
    }

    return 0;
}

bool read_sentence(char *noun_suffixes[], int noun_suffixes_len,
                   char *verb_suffixes[], int verb_suffixes_len,
                   char *adj_suffixes[], int adj_suffixes_len,
                   int *num_of_nouns, int *num_of_verbs, int *num_of_adjs) {
    while (1) {
        int ret = ReadWord(noun_suffixes, noun_suffixes_len, verb_suffixes,
                 verb_suffixes_len, adj_suffixes, adj_suffixes_len,
                 num_of_nouns, num_of_verbs, num_of_adjs);

        if (ret == PERIOD) {
            printf("The sentence had %d nouns, %d verbs and %d adjectives.\n",
           *num_of_nouns, *num_of_verbs, *num_of_adjs);
           *num_of_nouns = *num_of_verbs = *num_of_adjs = 0;
           return true;
        }

        if (ret == EOF || ret == ERROR) {
           return false;
        }
    }

}

void ReadAllText(char *noun_suffixes[], int noun_suffixes_len,
                   char *verb_suffixes[], int verb_suffixes_len,
                   char *adj_suffixes[], int adj_suffixes_len,
                   int *num_of_nouns, int *num_of_verbs, int *num_of_adjs) {
    printf("Enter text to analyze:\n");
    bool EOF_not_reached = true;
    while (EOF_not_reached) {
        EOF_not_reached = read_sentence(noun_suffixes, noun_suffixes_len,
                    verb_suffixes, verb_suffixes_len, adj_suffixes,
                    adj_suffixes_len, num_of_nouns, num_of_verbs,
                    num_of_adjs);
    }
}
