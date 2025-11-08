#ifndef TREE_H
#define TREE_H

typedef struct Node_t Node_t;
typedef struct Tree Tree;
typedef double TreeElem_t;

enum TreeErr {
  SUCCESS,
  ASSERTED,
  CREATE_FAILED,
  NULL_POINTER,
  DELETE_FAILED,
  ALLOC_FAILED,
  CALLOC_FAILED,
  REALLOC_FAILED,
  CHECK_FAILED,
  CYCLE_ERR
};

TreeErr treeInit(Tree ** tree);
TreeErr treeInsert(Tree * tree, TreeElem_t elem);
TreeErr subtreeDelete(Tree * tree, TreeElem_t elem);
void treeDestroy(Tree * tree);
TreeErr treeGetLen(Tree * tree, size_t * len);
TreeErr treePrint(Tree * tree, char * type);
TreeErr TreeToArray(Tree * tree, TreeElem_t ** array, size_t * len);
TreeErr ArrayToTree(Tree * tree, TreeElem_t * array, size_t len);

#endif // TREE_H
