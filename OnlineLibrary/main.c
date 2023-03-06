#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#define size 75
#define half_k 500

typedef struct _book
{
    char *title, *author;
    int rating, pages;
}Book;

Book* init_book()
{
Book *carte = (Book*)malloc(sizeof(Book));
carte -> author = (char*)calloc(size, sizeof(char));
carte -> title = (char*)calloc(size, sizeof(char));
return carte;
}

void elib_book(void *n)
{
    Book *carte = (Book*)n;
    free(carte -> title);
    free(carte -> author);
    free(carte);
}

void forempty(void* n)
{}

void elib_autor(void* n)
{
    Trie t = (Trie)n;
    elib_trie(&t, forempty);
}

void list_w_authors(Trie all_Aut, char *author, FILE *out)
{
    Trie Aut_lib = (Trie)extr_trie(all_Aut, author);
    if (Aut_lib == NULL)
        fprintf(out, "Autorul %s nu face parte din recomandarile tale.\n", author);
    else
        flip_flop(Aut_lib, "", out, half_k);
}

int main(int argc, char **argv)
{
    Trie books = init_trie(size);
    Trie authors = init_trie(size);
    FILE *in = fopen(argv[1], "rt");
    FILE *out = fopen(argv[2], "wt");

    char read[25];
    while( fscanf(in, "%s ", read) == 1 )
    {
        if(strcmp(read, "add_book") == 0 )
        {
            Book* carte = init_book();
            char info[400];
            fgets(info, 400, in);
            info[strlen(info) - 1] = '\0';

            char* condition;
            condition = strtok(info, ":");
            strcpy(carte -> title, condition);
            condition = strtok(NULL, ":");
            strcpy(carte -> author, condition);
            condition = strtok(NULL, ":");
            carte -> rating = atoi(condition);
            condition = strtok(NULL, ":");
            carte -> pages = atoi(condition);

            ins_in_trie(books, carte -> title, (void*)carte);

            void* autor = extr_trie(authors, carte -> author);
            if(autor != NULL)
                ins_in_trie((Trie)autor, carte -> title, (void*)carte);
            else
            {
                Trie t = init_trie(size);
                ins_in_trie(t, carte -> title, (void*)carte);
                ins_in_trie(authors, carte -> author, (void*)t);
            }
        }
        else if(strcmp(read, "search_book") == 0)
        {
            char titlu[size];
            fgets(titlu, size, in);
            titlu[strlen(titlu) - 1] = '\0';

            if(strchr(titlu, '~') == NULL)
            {
                void* carte = extr_trie(books, titlu);
                if(carte != NULL)
                {
                    Book* book = (Book*)carte;
                    fprintf(out, "Informatii recomandare: %s, %s, %d, %d\n", book->title, book->author, book->rating, book->pages);
                }
                else fprintf(out, "Cartea %s nu exista in recomandarile tale.\n", titlu);
            }
            else
            {
                titlu[strlen(titlu) - 1] = '\0';
                int aux = flip_flop(books, titlu, out, 3);

                if(aux <= 0)
                    fprintf(out, "Nicio carte gasita.\n");
            }
        }
        else if(strcmp(read, "list_author") == 0)
        {
            char autor[size];
            fgets(autor, size, in);
            autor[strlen(autor) - 1] = '\0';
            if(strchr(autor, '~') == NULL)
                list_w_authors(authors, autor, out);
            else
            {
                autor[strlen(autor) - 1] = '\0';
                int aux = flip_flop(authors, autor, out, 3);
                if(aux <= 0)
                    fprintf(out, "Niciun autor gasit.\n");
            }
        }
        else if(strcmp(read, "search_by_author") == 0)
        {
            char info[half_k];
            fgets(info, half_k, in);
            info[strlen(info) - 1] = '\0';
            char* x = strtok(info, ":");
            if( strchr(x, '~') != NULL )
            {
                x[strlen(x) - 1] = '\0';
                int aux = flip_flop(authors, x, out, 3);
                if(aux <= 0)
                        fprintf(out, "Niciun autor gasit.\n");
            }
            else
            {
                Trie t = (Trie)extr_trie(authors,x);
                if(t == NULL)
                {
                    fprintf(out, "Autorul %s nu face parte din recomandarile tale.\n", x);
                    continue;
                }
                char *y = strtok(NULL, ":");
                if(strchr(y, '~') != NULL)
                {
                    y[strlen(y) - 1] = '\0';
                    int aux = flip_flop(t, y, out, 3);
                    if(aux <= 0)
                            fprintf(out, "Nicio carte gasita.\n");
                }
                else
                {
                    void* carte = extr_trie(t, y);
                    if(carte != NULL)
                    {
                        Book* book = (Book*)carte;
                        fprintf(out, "Informatii recomandare: %s, %s, %d, %d\n", book->title, book->author, book->rating, book->pages);
                    }
                    else fprintf(out, "Cartea %s nu exista in recomandarile tale.\n", y);
                }
            }
        }
        else if(strcmp(read, "delete_book") == 0)
        {
            char titlu[size];
            fgets(titlu, size, in);
            titlu[strlen(titlu) - 1] = '\0';
            Book* book = extr_trie(books, titlu);
            if(book == NULL)
            {
                fprintf(out, "Cartea %s nu exista in recomandarile tale.\n", titlu);
                continue;
            }
            char autor[size];
            strcpy(autor, book -> author);
            del_el_trie(books, titlu, elib_book);
            Trie t = extr_trie(authors, autor);
            del_el_trie(t, titlu, forempty);
            if(if_empty(t))
                del_el_trie(authors, autor, elib_autor);

        }
    }
    elib_trie(&books, elib_book);
    elib_trie(&authors, elib_autor);
    return 0;
}