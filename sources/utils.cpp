#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "tree.h"
#include "utils.h"

void clear() {
  system("rm -rf \"./img/\" && mkdir \"./img/\"");
  FILE *file = fopen("./log.html", "w");
  if (file != NULL) {
    fclose(file);
  }
}

void parseErrors(TreeErr error) {
  switch (error) {
    case SUCCESS:
      break;
    case CREATE_FAILED:
      printf("Ошибка при создании\n");
      break;
    case NULL_POINTER:
      printf("Нулевой указатель\n");
      break;
    case ALLOC_FAILED:
      printf("Ошибка при аллокации\n");
      break;
    case CALLOC_FAILED:
      printf("Ошибка при аллокации\n");
      break;
    case REALLOC_FAILED:
      printf("Ошибка при реаллокации\n");
      break;
    case DELETE_FAILED:
      printf("Ошибка при удалении элементов из дерева\n");
      break;
    case ASSERTED:
      break;
    case CHECK_FAILED:
      printf("Ошибка при проверке\n");
      break;
    case CYCLE_ERR:
      printf("Список некорректно зациклился\n");
      break;
    default:
      printf("Неизвестная ошибка\n");
      break;
  }
}
