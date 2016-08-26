#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define FALSE 0
#define TRUE  1

typedef struct word_s {
    char          *word;
    struct word_s *next;
} word_t;

typedef struct trie_s {
    uint8_t         is_word;
    struct trie_s  *child[26];
} trie_t;

/* Add a word into the trie */
void
add_word_to_trie(trie_t *root, char *word)
{
    uint8_t index;

    if ((root == NULL) || (word == NULL) || (*word == '\0')) {
        return;
    }

    /*  Iterate over all letters in the word */
    while (*word != '\0') {
        index = *word - 'a';

        /* Malloc the child node if it doesn't exist */
        if (root->child[index] == NULL) {
            root->child[index] = (trie_t *) malloc(sizeof(trie_t));
            memset(root->child[index], 0, sizeof(trie_t));
        }

        root = root->child[index];
        word++;
    }

    /* The root is at the end of this word, flag it as such */
    root->is_word = TRUE;
}

/* Add a word to the list to be checked later */
word_t *
add_word_to_list(word_t *list, char *word)
{
    word_t *node = NULL;
    uint8_t length = 0;

    if ((word == NULL) || ((length = strlen(word)) < 2)) {
        return (node);
    }

    node = (word_t *) malloc(sizeof(word_t));
    node->word = (char *) malloc(length + 1);
    memcpy(node->word, word, length + 1);

    node->next = list;
    return (node);
}

uint8_t
is_compound_word(char *word, trie_t *trie_root, uint8_t is_substring)
{
    trie_t *trie_ptr = trie_root;
    uint8_t trie_index;

    if ((word == NULL) || (*word == '\0')) {
        return (FALSE);
    }

    /* Iterate over all letters in the word, comparing against the trie */
    while (*word != '\0') {
        trie_index = *word - 'a';

        if (trie_ptr->child[trie_index] == NULL) {
            return (FALSE);
        }

        trie_ptr = trie_ptr->child[trie_index];
        word++;

        /* We reached the end of a word */
        if (trie_ptr->is_word == TRUE) {
            /* If we're at the end of our string, we found a match.  This
             * case can't be considered a match if we're not a substring of
             * the original string, in that instance we just matched to
             * ourself which doesn't count.
             */
            if ((*word == '\0') && (is_substring == TRUE)) {
                return (TRUE);
            }

            /* If our remaining characters are a substring, we found a match */
            if (is_compound_word(word, trie_root, TRUE) == TRUE) {
                return (TRUE);
            }
        }
    }

    return (FALSE);
}

int
main(int argc, char **argv)
{
    FILE   *fp = NULL;
    char    buf[100];
    trie_t  trie_root;
    word_t *words = NULL;
    word_t *longest_word = NULL;
    uint8_t longest_word_length;

    /* Validate our command line inputs */
    if (argc != 2) {
        printf("Usage: %s <input file>\n", argv[0]);
        return (-1);
    }

    /* Open the file for reading */
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Couldn't open the file %s for reading\n", argv[1]);
        return (-1);
    }

    memset(&trie_root, 0, sizeof(trie_t));

    /* Walk the file list and stuff the words into the trie and the list */
    while (fscanf(fp, "%s\n", buf) != EOF) {
        add_word_to_trie(&trie_root, buf);
        words = add_word_to_list(words, buf);
    }

    fclose(fp);

    longest_word_length = 0;

    /* Go through every word in the list */
    while (words != NULL) {
        /* Only process the word if it's longer than our current best */
        if (strlen(words->word) > longest_word_length) {
            if (is_compound_word(words->word, &trie_root, FALSE) == TRUE) {
                longest_word_length = strlen(words->word);
                longest_word = words;
            }
        }
        words = words->next;
    }

    if (longest_word_length == 0) {
        printf("No compound word was found in the list\n");
    } else {
        printf("The longest compound word was '%s' with a length of %d\n",
               longest_word->word, longest_word_length);
    }

    return (0);
}
