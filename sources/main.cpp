#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "utils.h"

int main() {
  clear();
  TreeElem_t arr[] = {5, 3, 2, 0, 0, 1, 4, 8, 6, 9};
  size_t n = 10;
  Tree * tree = NULL;
  getTree(&tree, arr, n);

  Tree * tree2 = NULL;
  treeInit(&tree2);

  int len = 0;
  TreeElem_t elem = 0;
  scanf("%d", &len);
  for (int i = 0; i < len; i++) {
    scanf("%lg", &elem);
    treeInsert(tree2, elem);
  }

  printf("empty: %d depth: %zu balance: %d\n", treeEmpty(tree), treeDepth(tree), checkBalance(tree));
  TreeElem_t min = 0;
  size_t count = minCount(tree, &min);
  printf("right: %zu min_val: %lg min_count: %zu\n", getRight(tree), min, count);

  //subtreeDelete(tree, 8);
  //treeDeleteElem(tree, 10);

  treePrint(tree, "INORDER");
  treePrint(tree2, "INORDER");
  treeInvert(tree);
  treePrint(tree, "INORDER");

  treeDestroy(&tree);
  treeDestroy(&tree2);
  printf("%d %zu\n", treeEmpty(tree), treeDepth(tree));
  return 0;
}
