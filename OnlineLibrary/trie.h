#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define EMPTY(trie) (trie == NULL)

typedef struct node
{
    int dim;
    char curent_l;
    void *info;
    struct node** urm;
}TNode, *Trie;

Trie init_trie(int dim);
void elib_trie(Trie *t, void(*Free)(void*));
void ins_in_trie(Trie t, char *word, void *info);
void* extr_trie(Trie t, char *word);
void del_el_trie(Trie t, char *word, void(*Free)(void*));
int if_empty(Trie t);
int flip_flop(Trie t, char *pre, FILE *f, int maxim);



#endif // TRIE_H_INCLUDED
