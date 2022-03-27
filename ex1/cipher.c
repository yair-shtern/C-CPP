//
// Created by yair.shtern
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE "encode"
#define DECODE "decode"
#define CHECK "check"
#define BIG_A 65
#define BIG_Z 90
#define SMALL_A 97
#define SMALL_Z 122
#define A_Z 26
#define CHECK_ARGS 4
#define ENCODE_DECODE_ARGS 5

/**
 * Encrypt file_1 into file_2 by a given encryption key.
 * @param k The encryption key
 * @param file_1 A pointer to file_1 to encrypt.
 * @param file_2 A pointer to file_2 write the encryption.
 */
int encode (int k, char file_1[], char file_2[])
{
  k = k % A_Z;
  if (k < 0)
    {
      k = k + A_Z;
    }
  FILE *file_to_read;
  if ((file_to_read = fopen (file_1, "r")) == NULL)
    {
      fprintf (stderr, "The given file is invalid\n");
      return EXIT_FAILURE;
    }
  FILE *file_to_write;
  if ((file_to_write = fopen (file_2, "w")) == NULL)
    {
      fprintf (stderr, "The given file is invalid\n");
      fclose (file_to_read);
      return EXIT_FAILURE;
    }
  else
    {
      char c;
      while ((c = fgetc (file_to_read)) != EOF)
        {
          int int_c = (int) (c);
          if (BIG_A <= int_c && int_c <= BIG_Z)
            {
              fputc ((char) ((int_c + k - BIG_A)
                             % A_Z + BIG_A), file_to_write);
            }
          else if (SMALL_A <= int_c && int_c <= SMALL_Z)
            {
              fputc ((char) ((int_c + k - SMALL_A)
                             % A_Z + SMALL_A), file_to_write);
            }
          else
            {
              fputc (c, file_to_write);
            }
        }
      fclose (file_to_read);
      fclose (file_to_write);
      return EXIT_SUCCESS;
    }
}

/**
 * Decrypt file_2 into file_1 by a given decryption key.
 * @param k The decryption key
 * @param file_1 A pointer to file_1 write the decryption.
 * @param file_2 A pointer to file_2 to decrypt.
 */
int decode (int k, char file_1[], char file_2[])
{
  k = k % A_Z;
  if (k > 0)
    {
      k = k - A_Z;
    }
  FILE *file_to_read;
  if ((file_to_read = fopen (file_1, "r")) == NULL)
    {
      fprintf (stderr, "The given file is invalid\n");
      return EXIT_FAILURE;
    }
  FILE *file_to_write;
  if ((file_to_write = fopen (file_2, "w")) == NULL)
    {
      fprintf (stderr, "The given file is invalid\n");
      fclose (file_to_read);
      return EXIT_FAILURE;
    }
  else
    {
      char c;
      while ((c = fgetc (file_to_read)) != EOF)
        {
          int int_c = (int) (c);
          if (BIG_A <= int_c && int_c <= BIG_Z)
            {
              fputc ((char) ((int_c - k - BIG_A)
                             % A_Z + BIG_A), file_to_write);
            }
          else if (SMALL_A <= int_c && int_c <= SMALL_Z)
            {
              fputc ((char) ((int_c - k - SMALL_A)
                             % A_Z + SMALL_A), file_to_write);
            }
          else
            {
              fputc (c, file_to_write);
            }
        }
      fclose (file_to_read);
      fclose (file_to_write);
      return EXIT_SUCCESS;
    }
}

/**
 * Checks if file_2 it is encryption of file_1
 * and prints an informative message.
 */
int check (char file_1[], char file_2[])
{
  FILE *file1;
  if ((file1 = fopen (file_1, "r")) == NULL)
    {
      fprintf (stderr, "The given file is invalid\n");
      return EXIT_FAILURE;
    }
  FILE *file2;
  if ((file2 = fopen (file_2, "r")) == NULL)
    {
      fprintf (stderr, "The given file is invalid\n");
      fclose (file1);
      return EXIT_FAILURE;
    }
  else
    {
      char c = fgetc (file1);
      char h = fgetc (file2);
      int k;
      for (k = 0; k < A_Z; k++)
        {
          while ((c != EOF) || (h != EOF))
            {
              int int_c = (int) c;
              int int_h = (int) h;
              if ((BIG_A <= int_c && int_c <= BIG_Z) &&
                  (BIG_A <= int_h && int_h <= BIG_Z))
                {
                  if (((int_c + k - BIG_A) % A_Z) + BIG_A != int_h)
                    {
                      break;
                    }
                }
              else if (SMALL_A <= int_c && int_c <= SMALL_Z)
                {
                  if (((int_c + k - SMALL_A) % A_Z + SMALL_A) != int_h)
                    {
                      break;
                    }
                }
              else if (c != h)
                {
                  break;
                }
              (c = fgetc (file1));
              (h = fgetc (file2));
            }
          if (c == EOF && h == EOF)
            {
              fprintf (stdout, "Valid encrypting with k = %d\n", k);
              fclose (file1);
              fclose (file2);
              return EXIT_SUCCESS;
            }
          else
            {
              c = fseek (file1, 0, SEEK_SET);
              h = fseek (file2, 0, SEEK_SET);
            }
        }
    }
  fprintf (stdout, "Invalid encrypting\n");
  fclose (file1);
  fclose (file2);
  return EXIT_SUCCESS;
}

/**
 * The main program.
 * @param argc - Num of arguments from the commend line.
 * @param argv - the commend line arguments.
 * @return 0.
 */
int main (int argc, char *argv[])
{
  if (argc == 1)
    {
      fprintf (stderr, "The given command is invalid\n");
      return EXIT_FAILURE;
    }
  if (strcmp (argv[1], ENCODE) == 0)
    {
      if (argc != ENCODE_DECODE_ARGS)
        {
          fprintf (stderr, "Usage: cipher <encode|decode> <k> "
                           "<source path file> <output path file>\n");
          return EXIT_FAILURE;
        }
      else
        {
          if (encode (atoi (argv[2]), argv[3], argv[4]) == 0)
            {
              return EXIT_SUCCESS;
            }
          else
            {
              return EXIT_FAILURE;
            }
        }
    }
  else if (strcmp (argv[1], DECODE) == 0)
    {
      if (argc != ENCODE_DECODE_ARGS)
        {
          fprintf (stderr, "Usage: cipher <encode|decode> <k> "
                           "<source path file> <output path file>\n");
          return EXIT_FAILURE;
        }
      else
        {
          if (decode (atoi (argv[2]), argv[3], argv[4]) == 0)
            {
              return EXIT_SUCCESS;
            }
          else
            {
              return EXIT_FAILURE;
            }
        }
    }
  else if (strcmp (argv[1], CHECK) == 0)
    {
      if (argc != CHECK_ARGS)
        {
          fprintf (stderr, "Usage: cipher <check> "
                           "<source path file> <output path file>\n");
          return EXIT_FAILURE;
        }
      else
        {
          if (check (argv[2], argv[3]) == 0)
            {
              return EXIT_SUCCESS;
            }
          else
            {
              return EXIT_FAILURE;
            }
        }
    }
  else
    {
      fprintf (stderr, "The given command is invalid\n");
      return EXIT_FAILURE;
    }
}