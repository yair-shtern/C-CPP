//
// Created by yair.shtern on 29/05/2021.
//

#include "vector.h"

int reallocate_data (vector *vector, size_t old_cap);
void reorganize_data (void **data, size_t ind, size_t size);
/**
 *
 * Dynamically allocates a new vector.
 * @param elem_copy_func func which copies the element
 * stored in the vector (returns
 * dynamically allocated copy).
 * @param elem_cmp_func func which is used to compare elements
 * stored in the vector.
 * @param elem_free_func func which frees elements stored in the vector.
 * @return pointer to dynamically allocated vector.
 * @if_fail return NULL.
 */
vector *
vector_alloc (vector_elem_cpy elem_copy_func, vector_elem_cmp elem_cmp_func,
              vector_elem_free elem_free_func)
{
  if ((elem_copy_func == NULL) || (elem_cmp_func == NULL)
      || (elem_free_func == NULL))
    {
      return NULL;
    }
  vector *v = malloc (sizeof (*v));
  if (v == NULL)
    {
      return NULL;
    }
  v->capacity = VECTOR_INITIAL_CAP;
  v->size = 0;
  v->data = malloc (sizeof (void *) * v->capacity);
  if (v->data == NULL)
    {
      free (v);
      v = NULL;
      return NULL;
    }
  v->elem_copy_func = elem_copy_func;
  v->elem_cmp_func = elem_cmp_func;
  v->elem_free_func = elem_free_func;
  return v;
}

/**
 * Frees a vector and the elements the vector itself allocated.
 * @param p_vector pointer to dynamically allocated pointer to vector.
 */
void vector_free (vector **p_vector)
{
  if (p_vector && *p_vector)
    {
      vector_clear (*p_vector);
      free ((*p_vector)->data);
      (*p_vector)->data = NULL;
      free (*p_vector);
      *p_vector = NULL;
    }
}

/**
 * Adds a new value to the back (index vector_size) of the vector.
 * @param vector a pointer to vector.
 * @param value the value to be added to the vector.
 * @return 1 if the adding has been done successfully, 0 otherwise.
 */
int vector_push_back (vector *vector, const void *value)
{
  if (vector == NULL)
    {
      return 0;
    }
  vector->size++;
  size_t old_cap = vector->capacity;
  if (vector_get_load_factor (vector) > VECTOR_MAX_LOAD_FACTOR)
    {
      vector->capacity = old_cap * VECTOR_GROWTH_FACTOR;
      if(reallocate_data (vector, old_cap) == 0)
        {
          return 0;
        }
    }
  vector->data[vector->size - 1] = vector->elem_copy_func (value);
  return 1;
}

/**
 * Reallocate data if.
 * @param vector vector to reallocate.
 * @param old_cap the old capacity of the vector
 * @return 1 if the realoc has been done successfully, 0 otherwise.
 */
int reallocate_data (vector *vector, size_t old_cap)
{
  void **tmp_data = realloc (vector->data,
                             sizeof (void *) * vector->capacity);
  if (tmp_data == NULL)
    {
      vector->size--;
      vector->capacity = old_cap;
      return 0;
    }
  vector->data = tmp_data;
  return 1;
}

/**
 * Removes the element at the given index from the vector.
 * alters the indices of the remaining
 * elements so that there are no empty indices in
 * the range [0, size-1] (inclusive).
 * @param vector a pointer to vector.
 * @param ind the index of the element to be removed.
 * @return 1 if the removing has been done successfully, 0 otherwise.
 */
int vector_erase (vector *vector, size_t ind)
{
  if (vector == NULL)
    {
      return 0;
    }
  if (ind >= vector->size)
    {
      return 0;
    }
  vector->size--;
  size_t old_cap = vector->capacity;
  if (vector_get_load_factor (vector) < VECTOR_MIN_LOAD_FACTOR)
    {
      vector->capacity = old_cap / VECTOR_GROWTH_FACTOR;
      if(reallocate_data (vector, old_cap) == 0)
        {
          return 0;
        }
    }
  vector->elem_free_func (&vector->data[ind]);
  (vector)->data[ind] = NULL;
  reorganize_data (vector->data, ind, vector->size);
  return 1;
}

/**
 * Reorganize the data after erase.
 * @param data the data to reorganize.
 * @param ind the index erased.
 * @param size data size.
 */
void reorganize_data (void **data, size_t ind, size_t size)
{
  for (; ind < size; ind++)
    {
      void *temp = data[ind];
      data[ind] = data[ind + 1];
      data[ind + 1] = temp;
    }
}

/**
 * Returns the element at the given index.
 * @param vector pointer to a vector.
 * @param ind the index of the element we want to get.
 * @return the element at the given index if exists
 * (the element itself, not a copy of it),
 * NULL otherwise.
 */
void *vector_at (const vector *vector, size_t ind)
{
  if (vector == NULL)
    {
      return NULL;
    }
  if (ind >= vector->size)
    {
      return NULL;
    }
  return vector->data[ind];
}

/**
 * Gets a value and checks if the value is in the vector.
 * @param vector a pointer to vector.
 * @param value the value to look for.
 * @return the index of the given value if it is in the vector
 * ([0, vector_size - 1]).
 * Returns -1 if no such value in the vector.
 */
int vector_find (const vector *vector, const void *value)
{
  if (vector == NULL)
    {
      return -1;
    }
  for (size_t i = 0; i < vector->size; i++)
    {
      if (vector->elem_cmp_func (vector->data[i], value) == 1)
        {
          return (int) i;
        }
    }
  return -1;
}

/**
 * This function returns the load factor of the vector.
 * @param vector a vector.
 * @return the vector's load factor, -1 if the function failed.
 */
double vector_get_load_factor (const vector *vector)
{
  if (vector == NULL)
    {
      return -1;
    }
  return (double) vector->size / (double) vector->capacity;
}

/**
 * Deletes all the elements in the vector.
 * @param vector vector a pointer to vector.
 */
void vector_clear (vector *vector)
{
  if (vector)
    {
      for (size_t i = 0; i < vector->size; i++)
        {
          vector->elem_free_func (&vector->data[i]);
        }
      vector->size = 0;
    }
}