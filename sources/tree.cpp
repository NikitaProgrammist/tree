#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tree.h"
#include "private_tree.h"
#include "check_tree.h"

static size_t Depth(Node_t * node);
static char Balance(Node_t * left, Node_t * right);
static Node_t * Right(Node_t * node, size_t * len);
static Node_t * invert(Node_t * node);
static void get(Node_t * node, Node_t * parent, TreeElem_t * arr, size_t n);
static Node_t * findElem(Node_t * current, TreeElem_t elem, Node_t ** parent);
static void DestroyNode(Node_t * node, size_t * len);
static void changeMin(Tree * tree, Node_t * current);
static void PrintNode(Node_t * node, char * type);
static void NodesToArray(Node_t * node, TreeElem_t * array);

TreeErr treeInit(Tree ** tree) {
  *tree = (Tree *) calloc(1, sizeof(Tree));
  if (*tree == NULL) {
    return CREATE_FAILED;
  }
  (*tree)->len = 0;
  (*tree)->root = NULL;
  treeVerify(*tree, "AFTER");
  return SUCCESS;
}

char treeEmpty(Tree * tree) {
  return tree == NULL || tree->root == NULL;
}

size_t treeDepth(Tree * tree) {
  if (tree == NULL) {
    return 0;
  }
  return Depth(tree->root);
}

static size_t Depth(Node_t * node) {
  if (node == NULL) {
    return 0;
  }
  size_t left_depth = Depth(node->left);
  size_t right_depth = Depth(node->right);
  return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

char checkBalance(Tree * tree) {
  if (tree == NULL || tree->root == NULL) {
    return 1;
  }
  return Balance(tree->root->left, tree->root->right);
}

static char Balance(Node_t * left, Node_t * right) {
  if (fabs(Depth(left) - Depth(right)) > 1) {
    return 0;
  }
  else if (left == NULL || right == NULL) {
    return 1;
  }
  char leftd = Balance(left->left, left->right);
  char rightd = Balance(right->left, right->right);
  return leftd && rightd;
}

size_t getRight(Tree * tree) {
  if (tree == NULL || tree->root == NULL) {
    return 0;
  }
  size_t len = 0;
  Right(tree->root, &len);
  return len;
}

static Node_t * Right(Node_t * node, size_t * len) {
  if (node == NULL) {
    return NULL;
  }
  Node_t * left = Right(node->left, len);
  Node_t * right = Right(node->right, len);
  if (right != NULL && right->left == NULL && right->right == NULL) {
    (*len)++;
  }
  return node;
}

size_t minCount(Tree * tree, TreeElem_t * min) {
  Node_t * min_node = tree->root;
  while (min_node->left != NULL) {
    min_node = min_node->left;
  }
  *min = min_node->data;
  size_t count = 0;
  Node_t * current = tree->root;
  while (current->left != NULL) {
    if (current->data == *min) {
      count++;
    }
    current = current->left;
  }
  if (current->data == *min) {
    count++;
  }
  return count;
}

void treeInvert(Tree * tree) {
  tree->root = invert(tree->root);
}

static Node_t * invert(Node_t * node) {
  if (node == NULL) {
    return NULL;
  }
  Node_t * right = invert(node->right);
  Node_t * left = invert(node->left);
  node->left = right;
  node->right = left;
  return node;
}

TreeErr getTree(Tree ** tree, TreeElem_t * arr, size_t n) {
  if (*tree != NULL) {
    return CREATE_FAILED;
  }
  treeInit(tree);
  (*tree)->len = n;
  (*tree)->root = (Node_t *) calloc(1, sizeof(Node_t));
  (*tree)->root->data = arr[0];
  get((*tree)->root, NULL, arr, n);
  return SUCCESS;
}

static void get(Node_t * node, Node_t * parent, TreeElem_t * arr, size_t n) {
  static size_t ind = 1;
  if (ind >= n) {
    return;
  }
  if (arr[ind] > arr[ind - 1]) {
    return;
  }
  node->left = (Node_t *) calloc(1, sizeof(Node_t));
  node->left->data = arr[ind++];
  get(node->left, node, arr, n);
  if (ind >= n) {
    return;
  }
  if (parent != NULL && parent->left == node && arr[ind] > parent->data) {
    return;
  }
  node->right = (Node_t *) calloc(1, sizeof(Node_t));
  node->right->data = arr[ind++];
  get(node->right, node, arr, n);
}

TreeErr treeInsert(Tree * tree, TreeElem_t elem) {
  treeVerify(tree, "BEFORE");
  tree->len++;
  Node_t * tree_elem = tree->root;
  if (tree_elem == NULL) {
    tree->root = (Node_t *) calloc(1, sizeof(Node_t));
    if (tree->root == NULL) {
      return CALLOC_FAILED;
    }
    tree->root->data = elem;
    tree->root->left = tree->root->right = NULL;
    return SUCCESS;
  }
  while (1) {
    if (elem <= tree_elem->data) {
      if (tree_elem->left == NULL) {
        tree_elem->left = (Node_t *) calloc(1, sizeof(Node_t));
        if (tree_elem->left == NULL) {
          return CALLOC_FAILED;
        }
        tree_elem->left->data = elem;
        tree_elem->left->left = tree_elem->left->right = NULL;
        break;
      }
      else {
        tree_elem = tree_elem->left;
      }
    }
    else {
      if (tree_elem->right == NULL) {
        tree_elem->right = (Node_t *) calloc(1, sizeof(Node_t));
        if (tree_elem->right == NULL) {
          return CALLOC_FAILED;
        }
        tree_elem->right->data = elem;
        tree_elem->right->left = tree_elem->right->right = NULL;
        break;
      }
      else {
        tree_elem = tree_elem->right;
      }
    }
  }
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treeDeleteElem(Tree * tree, TreeElem_t elem) {
  treeVerify(tree, "BEFORE");
  Node_t * parent = NULL;
  Node_t * current = findElem(tree->root, elem, &parent);
  if (current == NULL) {
    return DELETE_FAILED;
  }
  if (parent == NULL) {
    changeMin(tree, current);
    return SUCCESS;
  }
  Node_t * left = parent->left;
  Node_t * right = parent->right;
  if (current->left == NULL && current->right == NULL) {
    if (parent->left == current) {
      parent->left = NULL;
    }
    else {
      parent->right = NULL;
    }
    DestroyNode(current, &tree->len);
  }
  else if (current->left == NULL && current->right != NULL) {
    if (parent->left == current) {
      parent->left = current->right;
    }
    else {
      parent->right = current->right;
    }
    free(current);
    tree->len--;
  }
  else if (current->left != NULL && current->right == NULL) {
    if (parent->left == current) {
      parent->left = current->left;
    }
    else {
      parent->right = current->left;
    }
    free(current);
    tree->len--;
  }
  else {
    changeMin(tree, current);
  }
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

static void changeMin(Tree * tree, Node_t * current) {
  Node_t * min = current->right;
  if (min->left == NULL) {
    current->data = min->data;
    free(min);
    current->right = NULL;
    tree->len--;
    return;
  }
  while (min->left->left != NULL) {
    min = min->left;
  }
  current->data = min->left->data;
  free(min->left);
  min->left = NULL;
  tree->len--;
}

TreeErr subtreeDelete(Tree * tree, TreeElem_t elem) {
  treeVerify(tree, "BEFORE");
  Node_t * parent = NULL;
  Node_t * current = findElem(tree->root, elem, &parent);
  if (current == NULL) {
    return DELETE_FAILED;
  }
  if (parent == NULL) {
    treeDestroy(&tree);
    treeInit(&tree);
    return SUCCESS;
  }
  Node_t * left = parent->left;
  Node_t * right = parent->right;
  if (parent->left == current) {
    parent->left = NULL;
    DestroyNode(left, &tree->len);
  }
  else {
    parent->right = NULL;
    DestroyNode(right, &tree->len);
  }
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

static Node_t * findElem(Node_t * current, TreeElem_t elem, Node_t ** parent) {
  while (current != NULL && current->data != elem) {
    *parent = current;
    if (elem <= current->data) {
      current = current->left;
    }
    else {
      current = current->right;
    }
  }
  return current;
}

void treeDestroy(Tree ** tree) {
  DestroyNode((*tree)->root, &(*tree)->len);
  free(*tree);
  *tree = NULL;
}

static void DestroyNode(Node_t * node, size_t * len) {
  if (node->left != NULL) {
    DestroyNode(node->left, len);
  }
  if (node->right != NULL) {
    DestroyNode(node->right, len);
  }
  free(node);
  (*len)--;
  return;
}

TreeErr treeGetLen(Tree * tree, size_t * len) {
  treeVerify(tree, "BEFORE");
  *len = tree->len;
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

TreeErr treePrint(Tree * tree, char * type) {
  treeVerify(tree, "BEFORE");
  PrintNode(tree->root, type);
  printf("\n");
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

static void PrintNode(Node_t * node, char * type) {
  if (!strncmp(type, "PREORDER", 8)) {
    printf("%lg ", node->data);
  }
  if (node->left != NULL) {
    PrintNode(node->left, type);
  }
  if (!strncmp(type, "INORDER", 7)) {
    printf("%lg ", node->data);
  }
  if (node->right != NULL) {
    PrintNode(node->right, type);
  }
  if (!strncmp(type, "POSTORDER", 9)) {
    printf("%lg ", node->data);
  }
  return;
}

TreeErr TreeToArray(Tree * tree, TreeElem_t ** array, size_t * len) {
  treeVerify(tree, "BEFORE");
  *len = tree->len;
  *array = (TreeElem_t *) calloc(tree->len, sizeof(TreeElem_t));
  NodesToArray(tree->root, *array);
  treeVerify(tree, "AFTER");
  return SUCCESS;
}

static void NodesToArray(Node_t * node, TreeElem_t * array) {
  static size_t index = 0;
  if (node->left != NULL) {
    NodesToArray(node->left, array);
  }
  array[index++] = node->data;
  if (node->right != NULL) {
    NodesToArray(node->right, array);
  }
  return;
}

TreeErr ArrayToTree(Tree * tree, TreeElem_t * array, size_t len) {
  treeVerify(tree, "BEFORE");
  for (size_t i = 0; i < len; i++) {
    treeInsert(tree, array[i]);
  }
  treeVerify(tree, "AFTER");
  return SUCCESS;
}
