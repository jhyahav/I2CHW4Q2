
#include <stdio.h>
#include <stdbool.h>

#define MAX_WORD_LEN 32
#define N_NOUNS 6
#define N_VERBS 5
#define N_ADJS 6
#define PERIOD 1
#define ERROR -2

bool is_suffix_in_dict(char *str, char *dict[], int n);
int StringLength(char *str);
void ConvertToLowercase(char *string);
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

void StringCopy(char *source, char *destination);

int main()
{
    //Predefined suffix dictionaries.
    char *noun_suffixes[] = {"msi", "re", "sci", "ssen", "tnem", "tsi"};
    char *verb_suffixes[] = {"de", "eta", "ezi", "gni", "yfi"};
    char *adj_suffixes[] = {"elba", "evi", "hsi", "la", "luf", "suo"};

    int noun_dict_len = N_NOUNS, verb_dict_len = N_VERBS,
        adj_dict_len = N_ADJS;

    int num_of_nouns = 0, num_of_verbs = 0, num_of_adjs = 0;

    int *num_nouns_ptr = &num_of_nouns;
    int *num_verbs_ptr = &num_of_verbs;
    int *num_adjs_ptr = &num_of_adjs;

    ReadAllText(noun_suffixes, noun_dict_len, verb_suffixes, verb_dict_len,
                adj_suffixes, adj_dict_len, num_nouns_ptr, num_verbs_ptr,
                num_adjs_ptr);

    return 0;
}

//This function calculates the length of a given string.
int StringLength(char *string_ptr)
{
    int length = 0;
    while (*string_ptr != '\0')
    {
        length++;
        string_ptr++;
    }

    return length;
}

//This function copies the contents of one string to another.
void StringCopy(char *source, char *destination)
{
    for (; *source != '\0'; source++, destination++)
    {
        *destination = *source;
    }

    *(destination) = '\0';
}

/*This function converts each uppercase letter in a given string to the
corresponding lowercase letter.*/
void ConvertToLowercase(char *string)
{
    while (*string != '\0')
    {
        if (*string >= 'A' && *string <= 'Z')
        {
            char temp = *string;
            temp += ('z' - 'Z');
            *string = temp;
        }

        string++;
    }
}

/*This function removes any non-letter characters from a given string.
This function is only called after the function ConvertToLowercase;
it only considers lowercase letters.*/
void RemoveNonLetters(char *string)
{
    while (*string != '\0')
    {
        if ((*string > 'z' || *string < 'a') && (*string != '.' ))
        {
            DeleteChar(string);
        }
        else
        {
            string++;
        }
    }
}

/*This function, given a pointer to a character within a string,
deletes that character and shifts all characters that appear after the
deleted character to the left.*/
void DeleteChar(char *string)
{
    for (char *temp_ptr = string; *temp_ptr != '\0'; temp_ptr++)
    {
        *temp_ptr = *(temp_ptr + 1);
    }
}

/*This function, given a source string, a destination string, and the
length that corresponds to both of them, copies the source string into
the destination string from end to beginning (flipped).*/
void FlipString(char *orig_str, char *flipped_str, int str_len)
{
    for (int i = str_len; i > 0; i--)
    {
        *(flipped_str + (str_len - i)) = *(orig_str + i - 1);
    }
    *(flipped_str + str_len) = '\0';
}

/*This function, given a flipped word and a suffix, compares the two.
Positive return values indicate that the flipped word is lexicographically
greater than the suffix. 0 indicates that the flipped word and the suffix
are identical. Negative return values indicate that the suffix is
lexicographically greater than the flipped word.*/
//19 lines.
int WordCompare(char *flipped_word, char *suffix)
{

    char processed_suffix[MAX_WORD_LEN + 1];
    StringCopy(suffix, processed_suffix);
    ConvertToLowercase(processed_suffix);
    RemoveNonLetters(processed_suffix);
    char *suf_ptr = processed_suffix;

    while (*flipped_word != '\0')
    {
        //During comparison, ignore a period at the end of a flipped word.
        if (*flipped_word == '.')
        {
            flipped_word++;
        }

        /*If the suffix ends before the flipped word, i.e., the suffix is
        contained within the flipped word, 2 is returned.*/
        if (*suf_ptr == '\0')
        {
            return 2;
        }

        /*If a character in the flipped word is lexicographically greater
        than the corresponding character in the suffix, 1 is returned.*/
        if (*flipped_word > *suf_ptr)
        {
            return 1;
        }

        /*If a character in the flipped word is lexicographically less
        than the corresponding character in the suffix, -1 is returned.*/
        if (*suf_ptr > *flipped_word)
        {
            return -1;
        }

        flipped_word++;
        suf_ptr++;
    }
    /*If the while loop ends (i.e., string_1 ends) before suffix ends,
    suffix is greater.*/
    if (*suf_ptr != '\0')
    {
        return -2;
    }
    //If the strings are identical, 0 is returned.
    return 0;
}

/*This function, given a string, a dictionary of suffixes, and the length
of that dictionary (n) carries out a binary search for the string's suffix.
If the suffix is found, the function returns true.*/
bool is_suffix_in_dict(char *str, char *dict[], int n)
{
    int low = 0, high = n, mid, ret;
    while (low <= high && low < n)
    {

        mid = low + (high - low)/2;
        ret = WordCompare(str, *(dict + mid));
        if (ret == 0 || ret == 2)
        {
            return true;
        }

        if (ret == 1)
        {
            low = mid + 1;
        }

        if (ret == -1 || ret == -2)
        {
            high = mid - 1;
        }
    }

    return false;
}
/*This function scans the input word by word and reads each word.
It checks if the word has a suffix in each of the predefined suffix
dictionaries. If it does, the corresponding counter is incremented via
its pointer that is passed to the function. If the last char in the input
word is a period, the function returns PERIOD, if the EOF is reached,
EOF is returned, and if an error occurs while scanning, ERROR is returned.*/
//21 lines.
int ReadWord(char *noun_suffixes[], int noun_suffixes_len,
             char *verb_suffixes[], int verb_suffixes_len,
             char *adj_suffixes[], int adj_suffixes_len,
             int *num_of_nouns, int *num_of_verbs, int *num_of_adjs)
{

    char word[MAX_WORD_LEN + 1], flipped_word[MAX_WORD_LEN + 1];
    int scanf_ret_val = scanf("%s", word);
    if (scanf_ret_val == EOF)
    {
        return EOF;
    }

    if (scanf_ret_val != 1)
    {
        return ERROR;
    }

    ConvertToLowercase(word);
    RemoveNonLetters(word);
    int str_len = StringLength(word);

    FlipString(word, flipped_word, str_len);

    if (is_suffix_in_dict(flipped_word, noun_suffixes, noun_suffixes_len))
    {
        *(num_of_nouns) = *(num_of_nouns) + 1;
    }
    else if (is_suffix_in_dict(flipped_word, verb_suffixes,
                               verb_suffixes_len))
    {
        *(num_of_verbs) = *(num_of_verbs) + 1;
    }
    else if (is_suffix_in_dict(flipped_word, adj_suffixes,
                               adj_suffixes_len))
    {
        *(num_of_adjs) = *(num_of_adjs) + 1;
    }

    if (*flipped_word == '.')
    {
        return PERIOD;
    }

    return 0;
}

/*This function reads each word in a sentence until its end is reached.
If a period is found, it returns true. If the sentence ends without a
period, i.e., because of EOF or an error, it returns false.*/
bool read_sentence(char *noun_suffixes[], int noun_suffixes_len,
                   char *verb_suffixes[], int verb_suffixes_len,
                   char *adj_suffixes[], int adj_suffixes_len,
                   int *num_of_nouns, int *num_of_verbs, int *num_of_adjs)
{
    while (1)
    {
        int ret = ReadWord(noun_suffixes, noun_suffixes_len, verb_suffixes,
                           verb_suffixes_len, adj_suffixes, adj_suffixes_len,
                           num_of_nouns, num_of_verbs, num_of_adjs);

        if (ret == PERIOD)
        {
            printf("The sentence had %d nouns, %d verbs and %d adjectives.\n",
                   *num_of_nouns, *num_of_verbs, *num_of_adjs);
            *num_of_nouns = *num_of_verbs = *num_of_adjs = 0;
            return true;
        }

        if (ret == EOF || ret == ERROR)
        {
            return false;
        }
    }

}

/*This function reads sentences until the end of the input (EOF)
is reached.*/
void ReadAllText(char *noun_suffixes[], int noun_suffixes_len,
                 char *verb_suffixes[], int verb_suffixes_len,
                 char *adj_suffixes[], int adj_suffixes_len,
                 int *num_of_nouns, int *num_of_verbs, int *num_of_adjs)
{
    printf("Enter text to analyze:\n");
    bool EOF_not_reached = true;
    while (EOF_not_reached)
    {
        EOF_not_reached = read_sentence(noun_suffixes, noun_suffixes_len,
                                        verb_suffixes, verb_suffixes_len,
                                        adj_suffixes, adj_suffixes_len,
                                        num_of_nouns, num_of_verbs,
                                        num_of_adjs);
    }
}


