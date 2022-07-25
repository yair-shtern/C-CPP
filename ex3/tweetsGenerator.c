//
// Created by yair shtern on 5/05/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define FOUR_ARGS 4
#define FIVE_ARGS 5
#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000
#define TRUE 1
#define FALSE 0
#define ALLOCATION_ERROR "Allocation failure: There was a problem \
with the allocation of memory."
#define ARG_USAGE "Usage: The program must get at least 3 arguments \
(seed, num of sentences and path to file)."
#define FILE_ERROR "Error: The path to the file given is invalid."
#define DECIMAL 10
static int counter = 1;
typedef struct WordStruct {
    char *word;
    struct WordProbability *prob_list;
    int prob_list_size;
    int appearance;
    int end_of_sentence;
    int *list;
    int size_of_list;
} WordStruct;

typedef struct WordProbability {
    struct WordStruct *word_struct_ptr;
    int app_after_word;
} WordProbability;

/************ LINKED LIST ************/

typedef struct Node {
    WordStruct *data;
    struct Node *next;
} Node;

typedef struct LinkList {
    Node *first;
    Node *last;
    int size;
} LinkList;

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkList *link_list, WordStruct *data)
{
  Node *new_node = malloc (sizeof (Node));
  if (new_node == NULL)
    {
      return 1;
    }
  *new_node = (Node) {data, NULL};

  if (link_list->first == NULL)
    {
      link_list->first = new_node;
      link_list->last = new_node;
    }
  else
    {
      link_list->last->next = new_node;
      link_list->last = new_node;
    }
  link_list->size++;
  return 0;
}

/*************************************/

WordStruct *create_word_struct (char *word, LinkList *dictionary);
void allocation_fail ();
bool word_end_of_sentence (char *word);
WordStruct *word_not_in_dict (char *word, LinkList *p_list);

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number (int max_number)
{
  return rand () % max_number;
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word (LinkList *dictionary)
{
  int count = 0;
  for (Node *p = dictionary->first; p != NULL; p = p->next)
    {
      if (word_end_of_sentence (p->data->word) == FALSE) count++;
    }
  int num = get_random_number (count);
  Node *node = dictionary->first;
  int i = 0;
  for (; i < num + 1; node = node->next)
    {
      if (word_end_of_sentence (node->data->word) == FALSE) i += 1;
      if (i == num + 1)break;
    }
  while (word_end_of_sentence (node->data->word) == TRUE) node = node->next;
  return node->data;
}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word (WordStruct *word_struct_ptr)
{
  int num = get_random_number (word_struct_ptr->size_of_list);
  int size = word_struct_ptr->list[num];
  return word_struct_ptr->prob_list[size].word_struct_ptr;
}

/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence (LinkList *dictionary)
{
  WordStruct *word_struct = get_first_random_word (dictionary);
  printf ("Tweet %d: %s ", counter, word_struct->word);
  WordStruct *next_word;
  int count = 1;
  while (count < MAX_WORDS_IN_SENTENCE_GENERATION)
    {
      next_word = get_next_random_word (word_struct);
      count++;
      if (next_word->end_of_sentence ||
          count == MAX_WORDS_IN_SENTENCE_GENERATION)
        break;
      printf ("%s ", next_word->word);
      word_struct = next_word;
    }
  printf ("%s\n", next_word->word);
  counter++;
  return count;
}

/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int add_word_to_probability_list (WordStruct *first_word,
                                  WordStruct *second_word)
{
  // Reallocate list of prob.
  int *tmp = realloc (first_word->list,
                      (first_word->size_of_list + 1) * sizeof (int));
  if (tmp == NULL)allocation_fail ();
  first_word->list = tmp;
  int size = first_word->prob_list_size;
  for (int i = 0; i < size; i++)
    {
      if (strcmp (first_word->prob_list[i].word_struct_ptr->word,
                  second_word->word) == 0)
        {
          first_word->list[first_word->size_of_list] = i;
          first_word->size_of_list++;
          first_word->prob_list[i].app_after_word++;
          return 0;
        }
    }
  first_word->list[first_word->size_of_list] = size;
  first_word->size_of_list += 1;

  // Reallocate prob list.
  WordProbability *pt = realloc (first_word->prob_list,
                                 sizeof (WordProbability) * (size + 1));
  if (pt == NULL)allocation_fail ();
  first_word->prob_list = pt;
  first_word->prob_list[size].word_struct_ptr = second_word;
  first_word->prob_list[size].app_after_word = 1;
  first_word->prob_list_size++;
  return 1;
}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary (FILE *fp, int words_to_read, LinkList *dictionary)
{
  char *word;
  char line[MAX_SENTENCE_LENGTH];
  int count = 0;
  int keep_reading = TRUE;
  while (keep_reading)
    {
      // Read every line.
      if (fgets (line, MAX_SENTENCE_LENGTH, fp) == NULL)break;
      word = strtok (line, " \n");
      WordStruct *first_word = create_word_struct (word, dictionary);

      count++;
      if (count == words_to_read) keep_reading = FALSE;
      while (keep_reading)
        {
          // Read every word in the line.
          word = strtok (NULL, " \n");
          if (word == NULL)break;
          WordStruct *second_word = create_word_struct (word, dictionary);
          if (first_word->end_of_sentence == FALSE)
            {
              // Add second word to the prob list of first word.
              add_word_to_probability_list (first_word, second_word);
            }
          first_word = second_word;
          count++;
          if (count == words_to_read) keep_reading = FALSE;
        }
    }
}

/**
 * Check if word in the Dictionary.
 * @param word - the word to check.
 * @param p_list dictionary.
 * @return WordStruct of word, if it's not in dictionary returns NULL.
 */
WordStruct *word_not_in_dict (char *word, LinkList *p_list)
{
  for (Node *p = p_list->first; p != NULL; p = p->next)
    {
      if (strcmp (p->data->word, word) == 0)
        {
          p->data->appearance++;
          return p->data;
        }
    }
  return NULL;
}

/**
 * Creates WordStruct of word.
 * @param word - word of WordStruct.
 * @param dictionary - dictionary to add the word to.
 * @return the WordStruct created.
 */
WordStruct *create_word_struct (char *word, LinkList *dictionary)
{
  WordStruct *word_struct;
  WordStruct *p = word_not_in_dict (word, dictionary);
  if (p == NULL)
    {
      word_struct = malloc (sizeof (WordStruct));
      if (word_struct == NULL)allocation_fail ();
      // Update word
      word_struct->word = malloc (sizeof (char) * MAX_WORD_LENGTH);
      if (word_struct->word == NULL)allocation_fail ();
      strcpy (word_struct->word, word);

      // Update prob list
      word_struct->prob_list = NULL;
      word_struct->prob_list_size = 0;
      word_struct->size_of_list = 0;
      if (word_end_of_sentence (word_struct->word))
        {
          word_struct->end_of_sentence = TRUE;
          word_struct->prob_list = NULL;
          word_struct->list = NULL;
        }
      else
        {
          word_struct->end_of_sentence = FALSE;
          word_struct->list = NULL;
        }
      add (dictionary, word_struct);
      word_struct->appearance = 1;
    }
  else
    {
      word_struct = p;
    }
  return word_struct;
}

/**
 * Checks if word end with ".".
 * @param word - the word to check.
 * @return True if it's end word and false otherwise.
 */
bool word_end_of_sentence (char *word)
{
  if (strcmp (&word[(int) strlen (word) - 1], ".") == 0)return TRUE;
  return FALSE;
}

/**
 * Exit program if there is an allocation failure.
 */
void allocation_fail ()
{
  printf (ALLOCATION_ERROR);
  exit (EXIT_FAILURE);
}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary (LinkList *dictionary)
{
  Node *p = dictionary->first;
  Node *tmp;
  while (p != NULL)
    {
      tmp = p->next;
      free (p->data->prob_list);
      free (p->data->word);
      free (p->data->list);
      free (p->data);
      free (p);
      p = tmp;
    }
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */

int main (int argc, char *argv[])
{
  // Check args
  if (argc != FOUR_ARGS && argc != FIVE_ARGS)
    {
      printf (ARG_USAGE);
      return EXIT_FAILURE;
    }
  // Initial srand()
  int long seed = strtol (argv[1], NULL, DECIMAL);
  srand (seed);

  // Initial num of tweets
  long int num_of_tweets = strtol (argv[2], NULL, DECIMAL);

  // open file to read
  FILE *file;
  if ((file = fopen (argv[3], "r")) == NULL)
    {
      printf (FILE_ERROR);
      return EXIT_FAILURE;
    }

  // Initial dictionary
  LinkList my_dictionary = {NULL, NULL, 0};

  // Initial words_to_read
  int long words_to_read;
  words_to_read =
      argc == FOUR_ARGS ? -1 : strtol (argv[4], NULL, DECIMAL);

  // Fill dictionary
  fill_dictionary (file, (int) words_to_read, &my_dictionary);

  // Generate sentences
  for (int i = 0; i < num_of_tweets; ++i)
    {
      generate_sentence (&my_dictionary);
    }

  // Free all allocations from dictionary
  free_dictionary (&my_dictionary);

  // Close file
  fclose (file);
  return EXIT_SUCCESS;
}