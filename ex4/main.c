#include "test_suite.h"
#include "stdio.h"
int main ()
{
  test_hash_map_insert();
  test_hash_map_erase();
  test_hash_map_at();
  test_hash_map_get_load_factor();
  test_hash_map_apply_if();
  printf ("DONE!");
  return 0;
}
