//
// Created by yair shtern on 15/04/2021.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BEST "best"
#define BUBBLE "bubble"
#define QUICK "quick"
#define ID_LEN 10
#define DECIMAL 10
#define MAX_INPUT_LEN 60
#define MIN_GRADE 0
#define MAX_GRADE 100
#define MIN_AGE 18
#define MAX_AGE 120
#define MIN_ID 1000000000
#define MAX_ID 9999999999
#define MSG_1 "Enter number of students. Then enter\n"
#define MSG_2 "Enter student info. Then enter\n"
#define ID_ERROR "ERROR: Id must have 10 digits and can not start with 0.\n"
#define GRADE_ERROR "ERROR: Grade should be an integer" \
                        " between 0 and 100 (includes).\n"
#define AGE_ERROR "ERROR: Age should be an integer between " \
                        "18 and 120 (includes).\n"
#define NUM_ARG_ERROR "ERROR: Num of students should be an integer " \
                        "bigger then 0.\n"
#define OP_USAGE "USAGE: The program must get one of " \
                        "<best|bubble|quick> operations.\n"
#define NUM_ARG_USAGE "USAGE: The program must get one arguments.\n"
#define MAX_ITEM_LEN 20

/*
 * This is Student struct with id (long int), grade (int) and age (int).
 */
typedef struct Student {
    long int _id;
    int _grade, _age;
} Student;

/*
 * This function gets pointers to start and the end of the array and prints
 * the information of the best student (grade/age).
 */
void best_student (Student *start, Student *end)
{
  float best_grade = 0;
  Student *curr_student = start;
  Student *best_student = start;
  while (curr_student <= end)
    {
      float curr_grade = ((float) curr_student->_grade) / (curr_student->_age);
      if (best_grade < curr_grade)
        {
          best_grade = curr_grade;
          best_student = curr_student;
        }
      curr_student += 1;
    }
  printf ("best student info is: %ld,%d,%d\n", best_student->_id,
          best_student->_grade, best_student->_age);
}

/*
 * This function gets two pointers to Student structs and swap
 * their addresses.
 */
void swap (Student *student_1, Student *student_2)
{
  Student temp = *student_1;
  *student_1 = *student_2;
  *student_2 = temp;
}

/*
 * This function gets pointers to start and the end of the array and print it.
 */
void print_arr (Student *start, Student *end)
{
  while (start <= end)
    {
      printf ("%ld,%d,%d\n", start->_id, start->_grade, start->_age);
      start += 1;
    }
}

/*
 * This function gets pointers to start and the end of the array and sort it
 * by the grade of the students using Bubble Sort algorithm.
 */
void bubble_sort (Student *start, Student *end)
{
  Student *curr_student = end;
  while (curr_student >= start)
    {
      Student *curr_student_2 = start;
      while (curr_student_2 < curr_student)
        {
          if (curr_student_2->_grade > (curr_student_2 + 1)->_grade)
            {
              swap (curr_student_2, curr_student_2 + 1);
            }
          curr_student_2 += 1;
        }
      curr_student -= 1;
    }
}

/*
 *  This function gets pointers to start and the end of the array and
 *  returns A pointer to Student struct when all the left side of the array
 *  is smaller (in age) then this Student age and the right side is bigger.
 */
Student *partition (Student *start, Student *pointer)
{
  Student *i = start - 1;
  Student *j = start;
  while (j <= pointer - 1)
    {
      if (j->_age <= pointer->_age)
        {
          i += 1;
          swap (i, j);
          j += 1;
        }
      swap (i + 1, pointer);
    }
  return i + 1;
}

/*
 * This function gets pointers to start and the end of the array and sort it
 * by the age of the students using Quick Sort algorithm.
 */
void quick_sort (Student *start, Student *end)
{
  if (start < end)
    {
      Student *pointer = partition (start, end);
      quick_sort (start, pointer - 1);
      quick_sort (pointer + 1, end);
    }
}

/*
 * This function gets pointer to the array and makes the operation.
 */
void make_op (const char *op, int students_num, Student *students_arr)
{
  if (strcmp (op, BEST) == 0)
    {
      best_student (students_arr, &students_arr[students_num - 1]);
    }
  if (strcmp (op, BUBBLE) == 0)
    {
      bubble_sort (students_arr, &students_arr[students_num - 1]);
      print_arr (students_arr, &students_arr[students_num - 1]);
    }
  if (strcmp (op, QUICK) == 0)
    {
      quick_sort (students_arr, &students_arr[students_num - 1]);
      print_arr (students_arr, &students_arr[students_num - 1]);
    }
}

/*
 * This function creates the array for every operation.
 */
int create_arr (const char *op)
{
  char str[MAX_INPUT_LEN];
  long int students_num = 0;
  while (students_num <= 0)
    {
      printf (MSG_1);
      if (fgets (str, MAX_INPUT_LEN, stdin) == NULL)
        {
          return EXIT_FAILURE;
        }
      char *students_num_ptr;
      students_num = strtol (str, &students_num_ptr, DECIMAL);
      if ((students_num <= 0) || strcmp (students_num_ptr, "\n") != 0)
        {
          students_num = 0;
          printf (NUM_ARG_ERROR);
          continue;
        }
    }
  Student *students_arr = malloc (students_num * sizeof (Student));
  if (students_arr == NULL)
    {
      return EXIT_FAILURE;
    }
  int count = 0;
  while (count < students_num)
    {
      printf (MSG_2);
      char student_info[MAX_INPUT_LEN];
      if (fgets (student_info, MAX_INPUT_LEN, stdin) == NULL)
        {
          free (students_arr);
          students_arr = NULL;
          return EXIT_FAILURE;
        }

      char id_char[MAX_ITEM_LEN], grade_char[MAX_ITEM_LEN],
          age_char[MAX_ITEM_LEN];
      int a = sscanf (student_info, "%[^,],%[^,],%[^,]",
                      id_char, grade_char, age_char);
      if (a == EOF)
        {
          free (students_arr);
          students_arr = NULL;
          return EXIT_FAILURE;
        }
      long int id, grade, age;
      char *id_ptr, *grade_ptr, *age_ptr;
      id = strtol (id_char, &id_ptr, DECIMAL);
      grade = strtol (grade_char, &grade_ptr, DECIMAL);
      age = strtol (age_char, &age_ptr, DECIMAL);
      if ((strlen (id_char) != ID_LEN) || (strcmp (id_ptr, "") != 0) ||
          MIN_ID > id || id > MAX_ID)
        {
          printf (ID_ERROR);
          continue;
        }
      if ((strcmp (grade_ptr, "") != 0) || MIN_GRADE > grade
          || grade > MAX_GRADE)
        {
          printf (GRADE_ERROR);
          continue;
        }
      if ((strcmp (age_ptr, "\n") != 0) || MIN_AGE > age || age > MAX_AGE)
        {
          printf (AGE_ERROR);
          continue;
        }
      students_arr[count]._id = id;
      students_arr[count]._grade = grade;
      students_arr[count]._age = age;
      count += 1;
    }
  make_op (op, students_num, students_arr);
  free (students_arr);
  students_arr = NULL;
  return EXIT_SUCCESS;
}

/*
 * The main program. Runs the program according to the given arguments.
 */
int main (int argc, char *argv[])
{
  if (argc != 2)
    {
      printf (NUM_ARG_USAGE);
      return EXIT_FAILURE;
    }
  char *op = argv[1];
  if ((strcmp (op, BEST) == 0) || (strcmp (op, BUBBLE) == 0)
      || (strcmp (op, QUICK) == 0))
    {
      return create_arr (op);
    }
  else
    {
      printf (OP_USAGE);
      return EXIT_FAILURE;
    }
}