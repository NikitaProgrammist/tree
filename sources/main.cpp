#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "utils.h"

int main() {
  clear();
  Tree * tree = NULL;
  treeInit(&tree);
  treeInsert(tree, 10);
  treeInsert(tree, 12);
  treeInsert(tree, 5);
  treeInsert(tree, 2);
  treeInsert(tree, 8);
  treeInsert(tree, 4);
  treeInsert(tree, 9);
  treeInsert(tree, 11);
  treeInsert(tree, 15);
  TreeElem_t * arr = NULL;
  size_t len = 0;
  TreeToArray(tree, &arr, &len);
  for (size_t i = 0; i < len; i++) {
    printf("%lg ", arr[i]);
  }
  printf("\n");
  treeDestroy(tree);
  free(arr);
  return 0;
}
