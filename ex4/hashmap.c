//
// Created by yair.shtern on 29/05/2021.
//

#include "hashmap.h"

int initialize_buckets (hashmap *hash_map);
void reset_map (hashmap *hash_map, size_t old_size,
                size_t old_cap, vector **old_buckets);
int reallocate_map (hashmap *hash_map, size_t old_size,
                    size_t old_cap, vector **old_buckets);
int rehashing_map (hashmap *hash_map, size_t old_size,
                   size_t old_cap, vector **old_buckets, size_t i, size_t j);
int update_and_insert (hashmap *hash_map, const pair *in_pair,
                       int changed, size_t old_size, size_t old_cap,
                       vector **old_buckets);
int update_and_erase (hashmap *hash_map, const void *key, int changed,
                      size_t old_size, size_t old_cap,
                      vector **old_buckets);
void free_buckets (vector **buckets, size_t capacity);
size_t get_ind_in_vec (const vector *vector, const void *key);
/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func)
{
  if (func == NULL)
    {
      return NULL;
    }
  hashmap *h = malloc (sizeof (*h));
  if (h == NULL)
    {
      return NULL;
    }
  h->capacity = HASH_MAP_INITIAL_CAP;
  h->size = 0;
  h->buckets = malloc (sizeof (vector *) * h->capacity);
  if (h->buckets == NULL)
    {
      free (h);
      h = NULL;
      return NULL;
    }
  if (initialize_buckets (h) == 0)
    {
      free (h);
      return NULL;
    }
  h->hash_func = func;
  return h;
}

/**
 * Initialize hash map buckets with vectors.
 * @param hash_map the hash map to be initialize with vectors.
 * @return returns 1 for successful initialization, 0 otherwise.
 */
int initialize_buckets (hashmap *hash_map)
{
  for (size_t i = 0; i < hash_map->capacity; i++)
    {
      hash_map->buckets[i] = vector_alloc (pair_copy, pair_cmp, pair_free);
      if (hash_map->buckets[i] == NULL)
        {

          for (size_t j = 0; j < hash_map->capacity; j++)
            {
              if (j < i)
                {
                  vector_free (&hash_map->buckets[j]);
                }
              hash_map->buckets[i] = NULL;
            }
          free (hash_map->buckets);
          return 0;
        }
    }
  return 1;
}

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map)
{
  if (!p_hash_map || !(*p_hash_map))
    {
      return;
    }
  for (size_t i = 0; i < (*p_hash_map)->capacity; i++)
    {
      vector_free (&(*p_hash_map)->buckets[i]);
      (*p_hash_map)->buckets[i] = NULL;
    }
  free ((*p_hash_map)->buckets);
  (*p_hash_map)->buckets = NULL;
  free (*p_hash_map);
  *p_hash_map = NULL;
}

/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  if (hash_map == NULL)
    {
      return 0;
    }
  if (hashmap_at (hash_map, in_pair->key) != NULL)
    {
      return 0;
    }
  int changed = 0;
  hash_map->size++;
  size_t old_size = hash_map->size - 1;
  size_t old_cap = hash_map->capacity;
  vector **old_buckets = hash_map->buckets;
  if (hashmap_get_load_factor (hash_map) > HASH_MAP_MAX_LOAD_FACTOR)
    {
      hash_map->capacity = old_cap * VECTOR_GROWTH_FACTOR;
      if (reallocate_map (hash_map, old_size, old_cap, old_buckets) == 0)
        {
          return 0;
        }
      changed = 1;
    }
  if (update_and_insert (hash_map, in_pair, changed,
                         old_size, old_cap, old_buckets) == 0)
    {
      return 0;
    }
  return 1;
}

/**
 * Updates and insert pair to hash map. If needed deletes unused buckets.
 * @param hash_map the hash map to update.
 * @param in_pair pair to insert.
 * @param changed if was change 1, else 0.
 * @param old_size the old size of the hash map.
 * @param old_cap the old capacity of the hash map.
 * @param old_buckets the old buckets of the hash map.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int update_and_insert (hashmap *hash_map, const pair *in_pair,
                       int changed, size_t old_size,
                       size_t old_cap, vector **old_buckets)
{
  size_t ind = hash_map->hash_func (in_pair->key) & (hash_map->capacity - 1);
  if (vector_push_back (hash_map->buckets[ind], in_pair) == 0)
    {
      if (changed == 1)
        {
          for (size_t i = 0; i < hash_map->capacity; i++)
            {
              vector_free (&hash_map->buckets[i]);
            }
        }
      reset_map (hash_map, old_size, old_cap, old_buckets);
      return 0;
    }
  if (changed == 1)
    {
      for (size_t i = 0; i < old_cap; i++)
        {
          vector_free (&old_buckets[i]);
        }
      free (old_buckets);
      old_buckets = NULL;
    }
  return 1;
}

/**
 * Initialize and rehashing map if there is change in the capacity.
 * @param hash_map hash map to update.
 * @param old_size the old size of the hash map.
 * @param old_cap the old capacity of the hash map.
 * @param old_buckets the old buckets of the hash map.
 * @return returns 1 for successful update, 0 otherwise.
 */
int reallocate_map (hashmap *hash_map,
                    size_t old_size, size_t old_cap, vector **old_buckets)
{
  hash_map->buckets = malloc (sizeof (vector *) * hash_map->capacity);
  if (hash_map->buckets == NULL)
    {
      reset_map (hash_map, old_size, old_cap, old_buckets);
      return 0;
    }
  if (initialize_buckets (hash_map) == 0)
    {
      reset_map (hash_map, old_size, old_cap, old_buckets);
      return 0;
    }
  for (size_t i = 0; i < old_cap; ++i)
    {
      for (size_t j = 0; j < old_buckets[i]->size; j++)
        {
          if (rehashing_map (hash_map, old_size,
                             old_cap, old_buckets, i, j) == 0)
            {
              return 0;
            }
        }
    }
  return 1;
}

/**
 * rehashing pairs from old buckets to hash map buckets.
 * @param hash_map hash map to rehash.
 * @param old_size the old size of the old buckets.
 * @param old_cap the old capacity of the old buckets.
 * @param old_buckets the old buckets to rehash from.
 * @param i vectors index.
 * @param j data index.
 * @return returns 1 for successful rehashing, 0 otherwise.
 */
int rehashing_map (hashmap *hash_map, size_t old_size,
                   size_t old_cap, vector **old_buckets, size_t i, size_t j)
{
  pair *p = vector_at (old_buckets[i], j);
  size_t idx =
      hash_map->hash_func (p->key) & (hash_map->capacity - 1);
  if (vector_push_back (hash_map->buckets[idx], p) == 0)
    {
      for (size_t k = 0; k < hash_map->capacity; k++)
        {
          vector_free (&hash_map->buckets[k]);
          reset_map (hash_map, old_size, old_cap, old_buckets);
        }
      return 0;
    }
  return 1;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully,
 * 0 otherwise. (if key not in map, considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL)
    {
      return 0;
    }
  if (hashmap_at (hash_map, key) == NULL)
    {
      return 0;
    }
  int changed = 0;
  hash_map->size--;
  size_t old_size = hash_map->size + 1;
  size_t old_cap = hash_map->capacity;
  vector **old_buckets = hash_map->buckets;
  if (hashmap_get_load_factor (hash_map) < HASH_MAP_MIN_LOAD_FACTOR)
    {
      hash_map->capacity = old_cap / VECTOR_GROWTH_FACTOR;
      if (reallocate_map (hash_map, old_size, old_cap, old_buckets) == 0)
        {
          return 0;
        }
      changed = 1;
    }
  if (update_and_erase (hash_map, key, changed,
                        old_size, old_cap, old_buckets) == 0)
    {
      return 0;
    }
  return 1;
}

/**
 * Updates and erase pair from hash map. If needed deletes unused buckets.
 * @param hash_map the hash map to update.
 * @param key the key of the pair to erase.
 * @param changed if was change 1, else 0.
 * @param old_size the old size of the hash map.
 * @param old_cap the old capacity of the hash map.
 * @param old_buckets the old buckets of the hash map.
 * @return returns 1 for successful erasing, 0 otherwise.
 */
int update_and_erase (hashmap *hash_map, const void *key,
                      int changed, size_t old_size, size_t old_cap,
                      vector **old_buckets)
{
  size_t ind = hash_map->hash_func (key) & (hash_map->capacity - 1);
  size_t ind_in_vec = get_ind_in_vec (hash_map->buckets[ind], key);
  if (vector_erase (hash_map->buckets[ind], ind_in_vec) == 0)
    {
      if (changed == 1)
        {
          free_buckets (hash_map->buckets, hash_map->capacity);
        }
      reset_map (hash_map, old_size, old_cap, old_buckets);
      return 0;
    }
  if (changed == 1)
    {
      free_buckets (old_buckets, old_cap);
      free (old_buckets);
      old_buckets = NULL;
    }
  return 1;
}

/**
 * Find the index of a given key in vector.
 * @param vector the vector to find the key in.
 * @param key the key to find.
 * @return the index where the key is located.
 */
size_t get_ind_in_vec (const vector *vector, const void *key)
{
  // We have already checked that it exists.
  for (size_t i = 0; i < vector->size; ++i)
    {
      pair *p = vector_at (vector, i);
      if (p->key_cmp (key, p->key) == 1)
        {
          return i;
        }
    }
  return 0; // Not going to happen.
}

/**
 * Frees oll vector in buckets.
 * @param buckets buckets to free.
 * @param capacity capacity of the buckets.
 */
void free_buckets (vector **buckets, size_t capacity)
{
  for (size_t i = 0; i < capacity; i++)
    {
      vector_free (&buckets[i]);
    }
}

/**
 * Reset hash map to old hash map.
 * @param hash_map the hash map to reset.
 * @param old_size the old size of the hash map.
 * @param old_cap the old capacity of the hash map.
 * @param old_buckets the old buckets of the hash map.
 */
void reset_map (hashmap *hash_map, size_t old_size,
                size_t old_cap, vector **old_buckets)
{
  hash_map->buckets = old_buckets;
  hash_map->size = old_size;
  hash_map->capacity = old_cap;
}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists,
 * NULL otherwise (the value itself,
 * not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL)
    {
      return NULL;
    }
  size_t ind = hash_map->hash_func (key) & (hash_map->capacity - 1);
  if (hash_map->buckets[ind]->size == 0)
    {
      return NULL;
    }
  for (size_t i = 0; i < hash_map->buckets[ind]->size; i++)
    {
      pair *p = vector_at (hash_map->buckets[ind], i);
      if (p->key_cmp (p->key, key) == 1)
        {
          return p->value;
        }
    }
  return NULL;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map)
{
  if (hash_map == NULL)
    {
      return -1;
    }
  return (double) hash_map->size / (double) hash_map->capacity;
}

/**
 * This function receives a hashmap and 2 functions,
 * the first checks a condition on the keys,
 * and the seconds apply some modification on the values.
 * The function should apply the modification
 * only on the values that are associated with keys that meet the condition.
 * Example: if the hashmap maps char->int,
 * keyT_func checks if the char is a capital letter (A-Z),
 * and val_t_func multiples the number by 2,
 * hashmap_apply_if will change the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)},
 * and the return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT
 * and return 1 if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values, If hash_map is NULL -1.
 */
int hashmap_apply_if (const hashmap *hash_map,
                      keyT_func keyT_func, valueT_func valT_func) //const
{
  if (hash_map == NULL || keyT_func == NULL || valT_func == NULL)
    {
      return -1;
    }
  int count = 0;
  for (size_t i = 0; i < hash_map->capacity; i++)
    {
      for (size_t j = 0; j < hash_map->buckets[i]->size; j++)
        {
          pair *p = vector_at (hash_map->buckets[i], j);
          if (keyT_func (p->key) == 1)
            {
              valT_func (p->value);
              count++;
            }
        }
    }
  return count;
}