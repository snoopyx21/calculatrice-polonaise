#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <tgmath.h>

#define MAX_ELEM 50
#define MAXSIZE 1024

// arbre 
typedef struct abin
{
    struct abin *fd;
    struct abin *fg;
    char * op;
    double val;
    char type; 
    //if type='1' then op
    //if type='0' then val
} arbre, *tree;

// récuperer tous les éléments de fgets dans un 
// tableau et contenant le nombre d'éléments
typedef struct table_p
{
    char ** tab;
    int nb_elem;
    int pos;
} tabl, *table;

tree enrac(char type, double val, char * op);
void liberer_arbre(tree a);
table init_table(table t);
void liberer_table(table t);
tree arborescence(table t);
double calcul(tree a);
void principal(table t);
