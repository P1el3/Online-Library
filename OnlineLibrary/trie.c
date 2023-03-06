#include "trie.h"
#define size 75

TNode* create_node(int dim, char curent_l, void* info)
{
    TNode* nod = (TNode*)malloc(sizeof(TNode));
    nod -> dim = dim;
    nod -> curent_l = curent_l;
    nod -> info = info;
    nod -> urm = malloc(dim*sizeof(TNode*));
    int i;
    for(i = 0; i < dim; i++ )
        nod -> urm[i] = NULL;

    return nod;
}
Trie init_trie(int dim)
    {return create_node(dim, '\0', NULL);}

void elib_trie(Trie *t, void (*Free)(void*))
{
    while(*t != NULL)
    {
        int i = 0;
        for(i = 0; i < (*t) -> dim; i++ )
            elib_trie(&((*t) -> urm[i]), Free);

        if((*t) -> info != NULL)
            Free((*t) -> info);
        free((*t) -> urm);
        free(*t);
        *t = NULL;
    }
}
int find_pos(char c)
{
    char alfabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-'?! ";
    int i;
    for(i = 0; i < 68; i++ )
        if( alfabet[i] == c )
            return i;
    return 0;
}

void ins_in_trie(Trie t, char* word, void* info)
{
    TNode *nod = t;
    int i;
    int lenght = strlen(word);
    for(i = 0; i < lenght; i++)
    {
        int aux = find_pos(word[i]);
        if(nod -> urm[aux] == NULL)
           nod -> urm[aux] = create_node(nod -> dim, word[i], NULL);

        nod = nod -> urm[aux];
    }
    nod->info = info;
}

void* extr_trie(Trie t, char *word)
{
    TNode *nod = t;
    int len = strlen(word);
    int i;
    for(i = 0; i < len; i++)
    {
        int aux = find_pos(word[i]);
        if(nod -> urm[aux] == NULL)
            break;
        else nod = nod -> urm[aux];
    }

    if(i < len)
        return NULL;
    else
    {
        if(nod -> info != NULL)
            return nod -> info;
        else
            return NULL;
    }
}

void del_el_trie(Trie t, char *word, void(*Free)(void*))
{
    TNode *nod = t;
    int len = strlen(word);
    int i;
    for(i = 0; i < len; i++)
    {
        int aux = find_pos(word[i]);
        if(nod -> urm[aux] == NULL)
            break;
        else nod = nod -> urm[aux];
    }
    if(i == len)
    {
        Free(nod -> info);
        nod -> info = NULL;
    }
}


int if_empty(Trie t)
{
    if(t == NULL)
        return 1;
    int i, check = 1;
    for(i = 0; i < t -> dim; i++)
        check = check && if_empty(t -> urm[i]);
    if(t -> info != NULL)
        return 0 && check;
    else return 1 && check;
}

void find_n_put(Trie t, char *word, FILE *f, char *pre, int *max_print, int maxim)
{
    if(t == NULL)
        return;
    if(t->info != NULL && *max_print < maxim)
    {
        fprintf(f, "%s%s\n", pre, word);
        (*max_print)++;
    }
    int i;
    for(i = 0; i < t -> dim; i++)
        if(t -> urm[i] != NULL )
        {
            strncat(word, &(t -> urm[i]) -> curent_l, 1);
            find_n_put(t -> urm[i], word, f, pre, max_print, maxim);
            word[strlen(word) - 1] = '\0';
        }
}

int flip_flop(Trie t, char *pre, FILE *f, int maxim)
{
    TNode *nod = t;
    int len = strlen(pre);
    int i = 0;
     for(i = 0; i < len; i++)
    {
        int aux = find_pos(pre[i]);
        if(nod -> urm[aux] == NULL)
            break;
        else nod = nod -> urm[aux];
    }

    if(i < len)
        return -1;
    int max_print = 0;
    char word[size];
    word[0] = '\0';
    find_n_put(nod, word, f, pre, &max_print, maxim);
    return max_print;
}

