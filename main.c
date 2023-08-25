#include "expression.h"

int main( void)
{
    table t=NULL;
    
    // initialisation de la table ou on stockera tout ce que
    // l'on va récupérer avec strtok()
    t=init_table(t);
    if(t == NULL)
    {
        printf("table construction error \n");
        exit(1);
    }
    principal(t);
    liberer_table(t);
    return 0;
}

