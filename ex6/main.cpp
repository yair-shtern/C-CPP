//
// Created by 97254 on 17/06/2021.
//
#include <iostream>
#include "vl_vector.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "vl_vector.h"
#include "helpers.h"
#include <assert.h>
#include "vl_string.h"
#define BONUS_IMPLEMENTED 1

#ifdef BONUS_IMPLEMENTED
//# include "vl_string.h"
#endif

#define PRESUBMISSION_ASSERT(testCase) __ASSERT_TEST("Presubmit", testCase)

//-------------------------------------------------------
// Tests
//-------------------------------------------------------

int TestCreateVector ()
{
  vl_vector<int, STATIC_CAP> int_vec{};
  vl_vector<std::string, STATIC_CAP> string_vec{};
  vl_vector<double, 4> double_vec{};

  ASSERT_VLA_PROPERTIES(int_vec, STATIC_CAP, 0)
  ASSERT_VLA_PROPERTIES(string_vec, STATIC_CAP, 0)
  ASSERT_VLA_PROPERTIES(double_vec, 4, 0)

  auto dynamic_allocated_vec = new vl_vector<int, STATIC_CAP> ();
  delete dynamic_allocated_vec;

  return 1;
}

int TestPushBack ()
{
  vl_vector<int, STATIC_CAP> vec;
  ASSERT_VLA_PROPERTIES(vec, STATIC_CAP, 0)
  vec.push_back (1);
  ASSERT_VLA_PROPERTIES(vec, STATIC_CAP, 1)

  return 1;
}

int TestSize ()
{
  vl_vector<int, STATIC_CAP> vec;
  ASSERT_SIZE(vec, 0)

  for (int i = 0; i < 5; ++i)
    {
      vec.push_back (i);
    }

  ASSERT_SIZE(vec, 5)

  for (int i = 0; i < 15; ++i)
    {
      vec.push_back (i);
    }

  ASSERT_SIZE(vec, 20)
  return 1;
}

int TestCapacity ()
{
  vl_vector<int, STATIC_CAP> vec;
  ASSERT_CAPACITY(vec, STATIC_CAP)

  // Inserting [0, 15) (15 items)
  for (int i = 0; i < 15; ++i)
    {
      vec.push_back (i);
    }

  ASSERT_CAPACITY(vec, STATIC_CAP)

  // Inserting [15, 30) (15 items)
  for (int i = 15; i < 30; ++i)
    {
      vec.push_back (i);
    }

  ASSERT_CAPACITY(vec, 39)

  // Inserting [30, 39) (15 items)
  for (int i = 30; i < 39; ++i)
    {
      vec.push_back (i);
    }

  ASSERT_CAPACITY(vec, 39)
  return 1;
}

int TestEmpty ()
{
  vl_vector<int, STATIC_CAP> vec;
  ASSERT_TRUE(vec.empty ())
  vec.push_back (1);
  RETURN_ASSERT_TRUE(!vec.empty ())
}

int TestClear ()
{
  vl_vector<int, STATIC_CAP> vec;

  vec.push_back (1);
  vec.clear ();

  ASSERT_SIZE(vec, 0)
  ASSERT_CAPACITY(vec, STATIC_CAP)

  return 1;
}

int TestPopBack ()
{
  vl_vector<int, STATIC_CAP> vec;
  if (!vec.empty ())
    {
      return 0;
    }

  vec.push_back (1);
  ASSERT_SIZE(vec, 1)
  ASSERT_CAPACITY(vec, STATIC_CAP)

  vec.pop_back ();
  ASSERT_SIZE(vec, 0)
  ASSERT_CAPACITY(vec, STATIC_CAP)

  return 1;
}

int TestGetElement ()
{
  vl_vector<int, STATIC_CAP> vec;
  vec.push_back (1);

  ASSERT_TRUE(vec.at (0) == 1)  // using vl_vector::At
  RETURN_ASSERT_TRUE(vec[0] == 1) // using vl_vector::operator[]
}

int TestData ()
{
  vl_vector<int, STATIC_CAP> vec;
  UNUSED(vec.data ());
  return 1;
}

int TestComparison ()
{
  vl_vector<int, STATIC_CAP> vec1{};
  vl_vector<int, STATIC_CAP> vec2{};

  RETURN_ASSERT_TRUE(vec1 == vec2)
}

int TestAssignment ()
{
  vl_vector<int, STATIC_CAP> vec1{};
  vec1.push_back (1);

  vl_vector<int, STATIC_CAP> vec2 = vec1;
  UNUSED(vec2);
  return 1;
}

int TestInsertSingleValue ()
{
  vl_vector<int, STATIC_CAP> vec;
  vec.push_back (0);
  vec.push_back (2);
  auto it = vec.begin ();
  ++it;
  vec.insert (it, 1);

  RETURN_ASSERT_TRUE(vec[1] == 1)
}

int TestInsertIteratorInStaticCapacity ()
{
  vl_vector<int, STATIC_CAP> vec;
  vec.push_back (0);

  std::vector<int> values{1, 2, 3, 4};
  auto it = vec.begin ();
  ++it;
  vec.insert (it, values.begin (), values.end ());
  RETURN_ASSERT_TRUE(vec.size () == 5)
}

int TestInsertIteratorInDynamicCapacity ()
{
  vl_vector<int, STATIC_CAP> vec;
  std::vector<int> values (30UL);
  auto it = vec.begin ();
  vec.insert (it, values.begin (), values.end ());
  RETURN_ASSERT_TRUE(vec.size () == 30)
}

int TestResize ()
{
  vl_vector<int> vec;
  ASSERT_VLA_PROPERTIES(vec, STATIC_CAP, 0)

  // Add 1 item
  vec.push_back (1);
  ASSERT_VLA_PROPERTIES(vec, STATIC_CAP, 1)

  // Add 16 item
  for (int i = 2; i <= 17; ++i)
    {
      vec.push_back (i);
    }
  ASSERT_VLA_PROPERTIES(vec, /* capacity: */ 25, /* size: */17)

  // Add another 12 items
  std::vector<int> entries (13, 1);
  vec.insert (vec.begin (), entries.cbegin (), entries.cend ());
  ASSERT_VLA_PROPERTIES(vec, /* capacity: */ 45, /* size: */30)

  // Remove those 12 items (note that the capacity won't be decreased).
  for (int i = 18; i <= 30; ++i)
    {
      vec.erase (vec.end () - 1);
    }
  ASSERT_VLA_PROPERTIES(vec, /* capacity: */ 45, /* size: */17)

  // Clear
  vec.clear ();
  ASSERT_VLA_PROPERTIES(vec, /* capacity: */ STATIC_CAP, /* size: */0)

  // Adding 17 items (note that we have a capacity of 25 instead of 39, as we did Clear, which returned
  // us to static memory and free'd the dynamic memory).
  for (int i = 1; i <= 17; ++i)
    {
      vec.push_back (i);
    }
  ASSERT_VLA_PROPERTIES(vec, /* capacity: */ 25, /* size: */17)

  return 1;
}

int TestCheckIterator ()
{
  vl_vector<int>::iterator it;
  vl_vector<int>::const_iterator it2;

  vl_vector<int> vec1;
  vl_vector<int> const vec2;

  it = vec1.begin ();
  it2 = vec2.begin ();

  it = vec1.end ();
  it2 = vec2.end ();

  UNUSED(it);
  UNUSED(it2);

  return 1;
}

int TestCheckReverseIterator ()
{
  vl_vector<int>::reverse_iterator it;
  vl_vector<int>::const_reverse_iterator it2;

  vl_vector<int> vec1;
  vl_vector<int> const vec2;

  it = vec1.rbegin ();
  it2 = vec2.crbegin ();

  it = vec1.rend ();
  it2 = vec2.crend ();

  UNUSED(it);
  UNUSED(it2);

  return 1;
}
//
////-------------------------------------------------------
////  Bonus
////-------------------------------------------------------
#ifdef BONUS_IMPLEMENTED

template<size_t StaticCapacity = 16ul>
int
VLStringEqual (const vl_string<StaticCapacity> &str, const char *expected_str, size_t expected_capacity)
{
  // Check the VLA properties
  size_t expected_str_len = strlen (expected_str);
  ASSERT_VLA_PROPERTIES(str, expected_capacity, expected_str_len);

  // Make sure that the actual values corresponds to the string values
  for (size_t i = 0; i < expected_str_len; ++i)
    {
      ASSERT_TRUE(str[i] == expected_str[i])
    }

  return 1;
}

int TestVLStringCreateString ()
{
  vl_string<> str1;
  ASSERT_TRUE(str1.size () == 0 && str1.capacity () == STATIC_CAP)

  vl_string<> str2 ("Hello");
  ASSERT_TRUE(str2.size () == 5 && str2.capacity () == STATIC_CAP)

  vl_string<> str2_copy (str2);
  ASSERT_TRUE(str2_copy.size () == 5 && str2_copy.capacity () == STATIC_CAP)

  return 1;
}

int TestVLStringConcat ()
{
  vl_string<> str;
  ASSERT_TRUE(str.size () == 0 && str.capacity () == STATIC_CAP)

  str += "Hello";
  ASSERT_TRUE(str.size () == 5 && str.capacity () == STATIC_CAP)

  str += ", World from C++";
  ASSERT_TRUE(str.size () == 21 && str.capacity () == 33)

  str += '!';
  ASSERT_TRUE(str.size () == 22 && str.capacity () == 33)

  return 1;
}

int TestVLStringContentStack ()
{
  const vl_string<> str ("Hello");
  const char *expected_str = "Hello";

  ASSERT_TRUE(VLStringEqual (str, expected_str, /* capacity: */ 16));

  return 1;
}

int TestVLStringContentHeap ()
{
  const vl_string<> str ("Hello my name is ...");
  const char *expected_str = "Hello my name is ...";

  ASSERT_TRUE(VLStringEqual (str, expected_str, /* capacity: */ 31));

  return 1;
}

#endif

//-------------------------------------------------------
//  The main entry point
//-------------------------------------------------------
int RunPresubmitChecks ()
{
  PRESUBMISSION_ASSERT(TestCreateVector)
  PRESUBMISSION_ASSERT(TestPushBack)
  PRESUBMISSION_ASSERT(TestSize)
  PRESUBMISSION_ASSERT(TestCapacity)
  PRESUBMISSION_ASSERT(TestEmpty)
  PRESUBMISSION_ASSERT(TestClear)
  PRESUBMISSION_ASSERT(TestPopBack)
  PRESUBMISSION_ASSERT(TestGetElement)
  PRESUBMISSION_ASSERT(TestData)
  PRESUBMISSION_ASSERT(TestComparison)
  PRESUBMISSION_ASSERT(TestAssignment)
  PRESUBMISSION_ASSERT(TestInsertSingleValue)
  PRESUBMISSION_ASSERT(TestInsertIteratorInStaticCapacity)
  PRESUBMISSION_ASSERT(TestInsertIteratorInDynamicCapacity)
  PRESUBMISSION_ASSERT(TestResize)
  PRESUBMISSION_ASSERT(TestCheckIterator)
  PRESUBMISSION_ASSERT(TestCheckReverseIterator)

#ifdef BONUS_IMPLEMENTED
  PRESUBMISSION_ASSERT(TestVLStringCreateString)
  PRESUBMISSION_ASSERT(TestVLStringConcat)
  PRESUBMISSION_ASSERT(TestVLStringContentStack)
  PRESUBMISSION_ASSERT(TestVLStringContentHeap)
#endif

  return 1;
}

void c ()
{
  const int arr_len = 10;
  int arr[10];
  int j = 0;
  for (; j < arr_len / 2; ++j)
    arr[j] = j;
  for (; j < arr_len; ++j)
    arr[j] = j + 10;

  vl_vector<int, arr_len> v (arr, arr + arr_len);
  int *static_vec = v.data ();

  j = 0;
  for (; j < arr_len / 2; ++j)
    arr[j] = j + 10;
  for (; j < arr_len; ++j)
    arr[j] = j;

  auto pos = v.insert (v.cbegin () + 5, arr, arr + 5);
  assert (static_vec != v.data ());
  assert (v.capacity () == (size_t) (3 * 15) / 2);

  int *dynamic_vec = v.data ();
  v.insert (pos, arr + 5, arr + 10);
  assert (v.size () == (size_t) 20);
  for (int i = 0; i < 20; ++i)
    assert (v[i] == i);

  assert (static_vec != v.data ());
  assert (dynamic_vec == v.data ());
  assert (v.capacity () == (size_t) (3 * 15) / 2);
}
//
//int main ()
//{
//  vl_vector<float> v (8, 0.25);
//  size_t s = v.size ();
//  int arr[5] = {-1, -2, -3, -4, -5};
//  int arr2[30];
//  for (int i = 0; i < 29; ++i)
//    {
//      arr2[i] = i;
//    }
//  vl_vector<int> v1 (arr, arr + 4);
//  for (auto item:v1) std::cout << item << " ";
//  std::cout << std::endl;
//  v1.insert (v1.begin ()+1, arr2, arr2 + 29);
//  for (auto item:v1) std::cout << item << " ";
//  std::cout << std::endl;
//
//  //v1.erase(v1.end()-1);
//  //std::cout << v1.data()[1] << " ";
//  //v1.clear();
//
//  for (auto item:v1) std::cout << item << " ";
//
//  std::cout << std::endl << "GOOD!";
//  return RunPresubmitChecks() ? EXIT_SUCCESS : EXIT_FAILURE;
//  c();
//}
/**
 *      C/CPP 2021 Summer - Ex06       *
 * Bonus part tests - vl_string - v1.2 *
 *           By Ohad Klein             *
 */

#include <iostream>
#include "vl_vector.h"
#include <cassert>
#include <cstring>

void test_def_ctor ()
{
  vl_string<> vl_str;
  assert (vl_str.capacity () == 16);
  assert (vl_str.size () == 0);
  assert(vl_str.empty ());
  assert(vl_str.at (0) == '\0');

  const vl_string<5> vl_str_const;
  assert (vl_str_const.capacity () == 5);
  assert (vl_str_const.size () == 0);
  assert (vl_str_const.empty ());
  assert(vl_str_const.at (0) == '\0');
}

void test_implicit_and_copy_ctor ()
{
  // implicit:
  vl_string<> vl_str_imp = "Hello, World!";
  assert (vl_str_imp.capacity () == 16);
  assert (vl_str_imp.size () == 13);
  assert(!vl_str_imp.empty ());
  assert(vl_str_imp.at (13) == '\0');

  const vl_string<5> vl_str_imp_const = "Hello";
  assert (vl_str_imp_const.capacity () == (int) ((5 + 1) * 3 / 2));
  assert (vl_str_imp_const.size () == 5);
  assert(!vl_str_imp_const.empty ());
  assert(vl_str_imp_const.at (5) == '\0');

  // copy:
  vl_string<> vl_str_copy = vl_str_imp;
  assert (vl_str_copy.capacity () == vl_str_imp.capacity ());
  assert (vl_str_copy.size () == 13);
  assert(!vl_str_copy.empty ());

  const vl_string<5> vl_str_copy_const = vl_str_imp_const;
  assert (vl_str_copy_const.capacity () == vl_str_imp_const.capacity ());
  assert (vl_str_copy_const.size () == 5);
  assert(!vl_str_copy_const.empty ());
}

void test_vl_vector_methods ()
{
  vl_string<> vl_str = "Never gonna";
  assert (vl_str.capacity () == 16);
  assert (vl_str.size () == 11);
  assert(vl_str.at (11) == '\0');

  vl_str.push_back (' ');
  assert (vl_str.capacity () == 16);
  assert (vl_str.size () == 12);

  // note that end() must point to after the '\0' character:
  const char *str = "give you up\n";
  vl_str.insert (vl_str.end () - 1, str, str + strlen (str));

  assert(vl_str.capacity () == (int) (25 * 3 / 2));
  assert (vl_str.size () == 24);
  assert(vl_str.at (24) == '\0');
  assert(vl_str.contains ("Never gonna give you up\n"));

  vl_str.pop_back ();
  assert(vl_str.capacity () == (int) (3 * 25 / 2));
  assert (vl_str.size () == 23);
  assert(!vl_str.contains ("\n"));
  assert(vl_str.at (23) == '\0');

  // the following test also checks if '\0' is at the end of the container
  vl_str.erase (vl_str.begin (), vl_str.begin () + 7);
  assert(vl_str.capacity () == (int) (3 * 25 / 2));
  assert (vl_str.size () == 16);
  vl_str.erase (vl_str.begin ());
  assert(vl_str.capacity () == 16);
  assert (vl_str.size () == 15);
  assert(!vl_str.contains ("Never"));

  vl_str.clear ();
  assert(vl_str.capacity () == 16);
  assert (vl_str.size () == 0);
  assert(vl_str.empty ());
  assert(vl_str.contains ("\0"));
}

void test_contains_method ()
{
  vl_string<> vl_str;
  assert(vl_str.contains (""));
  vl_str = "Never gonna let you down\n";
  assert(vl_str.contains ("Never "));
  assert(!vl_str.contains (" Never"));
  assert(vl_str.contains ("down\n"));
  assert(!vl_str.contains ("nwod"));
  assert(vl_str.contains ("a"));
  assert(!vl_str.contains ("b"));
  assert(vl_str.contains (""));
  assert(vl_str.contains ("\0"));
  assert(vl_str.contains ("Never gonna let you down\n"));
  vl_str.clear ();
  assert(vl_str.contains (""));
  assert(vl_str.contains ("\0"));
}

void test_vl_vector_operators ()
{
  vl_string<> vl_str = "Never gonna run around and desert you\n";
  // indexing operators:
  try
    {
      assert(vl_str.at (0) == 'N');
      assert(vl_str.at (37) == '\n');
    }
  catch (std::out_of_range &e)
    {
      assert(e.what () && false);
    }
  // at 38 ('\0'), at should be able to access
  bool exception_caught = false;
  try
    {
      vl_str.at (38);
    }
  catch (std::exception &e)
    {
      exception_caught = true;
    }
  assert(!exception_caught && "method .at threw exception at '\\0'");

  // at 39(out of range), at should not be able to access
  exception_caught = false;
  try
    {
      vl_str.at (39);
    }
  catch (std::exception &e)
    {
      exception_caught = true;
    }
  assert(exception_caught && "method .at didn't throw exception");
  assert(vl_str[0] == 'N');
  assert(vl_str[37] == '\n');
  vl_str[37] = '!';
  assert(vl_str[37] == '!');
  try
    {
      vl_str.at (0) = 'G';
    }
  catch (std::exception &e)
    {
      assert(e.what () && false);
    }
  assert(vl_str.at (0) == 'G');

  // copy assignment operator:
  vl_string<> vl_str_to_copy = "Never gonna make you cry\n";
  vl_str = vl_str_to_copy;
  assert(vl_str.capacity () == vl_str_to_copy.capacity ());
  assert(vl_str.size () == vl_str_to_copy.size ());
  assert(vl_str.contains ("Never gonna make you cry\n"));
  assert(!vl_str.contains ("Never gonna run around and desert you\n"));
  vl_str[0] = 'G';
  assert(vl_str.contains ("Gever"));
  assert(!vl_str_to_copy.contains ("Gever"));
  assert(vl_str != vl_str_to_copy);
  vl_str[0] = 'N';
  assert(vl_str == vl_str_to_copy);
}

void test_addition_operators ()
{
  vl_string<> vl_str;
  vl_string<> vl_str_to_add = "Never gonna";
  (((vl_str += vl_str_to_add) += ' ') += "say goodbye") += '\n';

  assert(vl_str.contains ("Never gonna say goodbye\n"));

  vl_string<> vl_str_2;
  vl_str_2 = vl_str_to_add + ' ' + "tell a lie and hurt you" + '\n';
  assert(vl_str_2.contains ("Never gonna tell a lie and hurt you\n"));
}

void test_cast ()
{
  vl_string<> vl_str = "Fifteen chars..";
  const char *str = vl_str;
  assert(strcmp (str, "Fifteen chars..") == 0);
  assert(strlen (str) == strlen ("Fifteen chars.."));
  const vl_string<> vl_str_const = "Sixteen chars...";
  const char *str_2 = vl_str_const;
  assert(strlen (str_2) == strlen ("Sixteen chars..."));
  assert(strcmp (str_2, "Sixteen chars...") == 0);
}

int main ()
{
  int arr_len = 20;
  int arr[20];
  for (int i = 0; i < arr_len; ++i)
    arr[i] = i - 2;

  vl_vector<int, 10> v (arr, arr + arr_len);

  assert (v.size () == (size_t) arr_len);
  int *dynamic_vec = v.data ();
  vl_vector<int, 10>::iterator pos;
  for (auto item:v) std::cout << item << " ";
  std::cout << " -before" << std::endl;
  pos = v.erase (v.begin (), v.begin () + 2);
  for (auto item:v) std::cout << item << " ";
  std::cout << " -after" << std::endl;
  std::cout<<*pos<<" ? "<<*v.begin();
  assert (pos == v.begin ());
  assert (v.data () == dynamic_vec);
  pos = v.erase (v.begin () + 10, v.begin () + 18);
  assert (pos == v.begin () + 10);

  assert (v.size () == (size_t) 10);
  assert (v.capacity () == (size_t) 10);
  assert (v.data () != dynamic_vec);
  for (int i = 0; i < 10; ++i)
    assert (v[i] == i);

  RunPresubmitChecks () ? EXIT_SUCCESS : EXIT_FAILURE;
  std::cout << "~~~~~~~~~~~~~~~~~ Testing ~~~~~~~~~~~~~~~~~" << std::endl;
  std::cout << std::endl;
  std::cout << "********* Starting test_def_ctor **********" << std::endl;
  test_def_ctor ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "*** Starting test_implicit_and_copy_ctor **" << std::endl;
  test_implicit_and_copy_ctor ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "***** Starting test_vl_vector_methods *****" << std::endl;
  test_vl_vector_methods ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "****** Starting test_contains_method ******" << std::endl;
  test_contains_method ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "**** Starting test_vl_vector_operators ****" << std::endl;
  test_vl_vector_operators ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "***** Starting test_addition_operators ****" << std::endl;
  test_addition_operators ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;

  std::cout << "*********** Starting test_cast ************" << std::endl;
  test_cast ();
  std::cout << "****************** Done *******************" << std::endl;
  std::cout << std::endl;
  std::cout << "~~~~~~~~~~~ No errors detected! ~~~~~~~~~~~" << std::endl;

  return 0;
}