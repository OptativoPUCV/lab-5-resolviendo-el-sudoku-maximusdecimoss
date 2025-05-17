#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
  int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
      for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
      }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
      for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
      printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n) {
  int i, j, k, p;
  int row[10], col[10], sub[10];

  for (i = 0; i < 9; i++) {
      for (j = 0; j < 10; j++) row[j] = 0;
      for (j = 0; j < 9; j++) {
          int num = n->sudo[i][j];
          if (num != 0) {
              if (row[num] == 1) return 0;
              row[num] = 1;
          }
      }
  }

  for (j = 0; j < 9; j++) {
      for (i = 0; i < 10; i++) col[i] = 0;
      for (i = 0; i < 9; i++) {
          int num = n->sudo[i][j];
          if (num != 0) {
              if (col[num] == 1) return 0;
              col[num] = 1;
          }
      }
  }

  for (k = 0; k < 9; k++) {
      for (p = 0; p < 10; p++) sub[p] = 0;
      for (p = 0; p < 9; p++) {
          int i = 3 * (k / 3) + (p / 3);
          int j = 3 * (k % 3) + (p % 3);
          int num = n->sudo[i][j];
          if (num != 0) {
              if (sub[num] == 1) return 0;
              sub[num] = 1;
          }
      }
  }

  return 1;
}



List* get_adj_nodes(Node* n) {
  List* list = createList();
  int i, j, found = 0;

  for (i = 0; i < 9 && !found; i++) {
      for (j = 0; j < 9; j++) {
          if (n->sudo[i][j] == 0) {
              found = 1;
              for (int num = 1; num <= 9; num++) {
                  Node* new = copy(n);
                  new->sudo[i][j] = num;
                  if (is_valid(new)) {
                      pushBack(list, new);
                  } else {
                      free(new);
                  }
              }
              break;
          }
      }
  

  return list;
}


int is_final(Node* n) {
  int i, j;
  for (i = 0; i < 9; i++) {
      for (j = 0; j < 9; j++) {
          if (n->sudo[i][j] == 0) return 0;
      }
  }
  return 1;
}


Node* DFS(Node* initial, int* cont) {
  Stack* stack = createStack();
  push(stack, initial);
  *cont = 0;

  while (!is_empty(stack)) {
      Node* current = top(stack);
      pop(stack);
      (*cont)++;

      if (is_final(current)) {
          clean(stack);
          return current;
      }

      List* adj_nodes = get_adj_nodes(current);
      Node* adj = first(adj_nodes);
      while (adj != NULL) {
          push(stack, adj);
          adj = next(adj_nodes);
      }

      free(current);
      free(adj_nodes);
  }

  clean(stack);
  return NULL;
}


/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/