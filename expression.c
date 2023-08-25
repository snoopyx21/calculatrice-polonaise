#include "expression.h"

/*
 * structure en commentaire
 * en rappel
 * typedef struct abin
 * {
 *   struct abin *fd;
 *   struct abin *fg;
 *   char * op;
 *   double val;
 *   char type; //if type='1' then op
 *	      //if type='0' then val
 * } arbre, *tree;
 *
 * typedef struct table_p
 * {
 *   char ** tab;
 *   int nb_elem;
 *   int pos;
 * } tabl, *table; 
 *
 */

// fonction d'enracinement d'un arbre
tree enrac(char type, double val, char * op)
{
    tree a=malloc(sizeof(struct abin));
    if (a==NULL)
    {
        free(a);
        fprintf(stderr, "memory allocation error \n");
        return NULL;
    }
    a->type=type;
    a->val=val;
    a->op=op;
    a->fd=NULL;
    a->fg=NULL;
    return a;
}

// fonction de libération d'un arbre
void liberer_arbre(tree a)
{
    if (a==NULL) return;
    liberer_arbre(a->fd);
    liberer_arbre(a->fg);
    free(a);
} 

// fonction d'initialisation de ma table
table init_table(table t)
{
    t=malloc(sizeof(struct table_p));
    if (t == NULL)
    {
        free(t);
        fprintf(stderr, "memory allocation error \n");
        return NULL;
    }
    t->pos=0;
    t->nb_elem=0;
    t->tab=(char**)malloc(MAX_ELEM*sizeof(char*));
    if(t->tab==NULL) 
    {
        free(t);
        fprintf(stderr, "memory allocation error \n");
        return NULL;
    }    
    return t;
}

// fonction de libération de ma table
void liberer_table(table t)
{
    if (t==NULL) return;
    free(t->tab);
    free(t);
}

// fonction récursive de la création de mon arbre
tree arborescence(table t)
{
    char *end;
    double val;
    tree a;
    // on vérifie qu'on ne touche pas un espace mémoire vide
    // pour éviter un SEGFAULT
    if (t->tab[t->pos] == NULL )
    {
        fprintf(stdout, "ERROR \n");
        return NULL;
    }
    // utilisation de strtod dans mon tableau 
    // arrêt uniquement lorsque qu'on rencontre un double
    val=strtod(t->tab[t->pos], &end);
    if (*end == '\0'  )
    // avec strtod, si on rencontre un double 
    // alors on recevra dans la valeur de end '\0'
    // on regarde alors si c'est le cas
    // on enracine alors un double = fin de ma fonction récursive
    {
        t->pos++;
        return a=enrac('0',val,NULL);
    }

    // sinon parcours de notre table
    // et construction de l'arbre en conséquence
    else
    {  
        if( strcmp(end, t->tab[t->pos])) 
        {
            fprintf(stdout,"ERROR \n");       
            return NULL;
        }
        else
        {
            // on compare l'élément x dans notre tableau si il est le même qu'un opérateur
            if ((strcmp("+", t->tab[t->pos])==0)  || (strcmp("-",t->tab[t->pos]) ==0) 
             || (strcmp("*",t->tab[t->pos]) ==0)  || (strcmp("/",t->tab[t->pos]) ==0) 
             || (strcmp("min",t->tab[t->pos]) ==0)  || (strcmp("max",t->tab[t->pos]) ==0) )
            {
                // ce sont ici des opérateurs nécessitant deux opérandes 
                // on va les chercher/placer dans le fg et fd
                a = enrac('1', 0 , t->tab[t->pos++]);
    
                if ((a->fg=arborescence(t))== NULL)
                {
                    fprintf(stderr, "maybe you go to a SEGFAULT.\n");
                    liberer_arbre(a);
                    return NULL;
                }
                if ((a->fd=arborescence(t)) == NULL)
                {
                    fprintf(stderr, "maybe you go to a SEGFAULT. \n");
                    liberer_arbre(a);
                    return NULL;
                }
                return a;
            }
            else
            {
                // de même que précédemment
                if ((strcmp("sqrt", t->tab[t->pos]) ==0 ) || (strcmp("exp",t->tab[t->pos]) ==0) 
                 || (strcmp("ln",t->tab[t->pos]) ==0)  || (strcmp("abs",t->tab[t->pos]) ==0) 
                 || (strcmp("floor",t->tab[t->pos]) ==0)  || (strcmp("ceil",t->tab[t->pos]) ==0) )
                // sauf qu'ici ce sont des opérateurs nécessitant une opérande
                // on place uniquement dans le fg
                {
                    a = enrac('1', 0, t->tab[t->pos++]);
                    if ((a->fg=arborescence(t)) == NULL)
                    {
                        fprintf(stderr, "maybe you go to a SEGFAULT");
                        liberer_arbre(a);
                        return NULL;
                    }
                    return a;
                }
                else
                // on retourne NULL alors (erreur)
                // au retour dans la foncion principale on sortira une erreur
                {
                    return NULL;
                }
            }
        }
    }
}

double calcul(tree a)
{
    // vérification que l'arbre ne soit pas inexistant
    if (a== NULL)
    {
        fprintf(stderr,"allocation error.\n"); 
    }
    if (a->type == '0')
    // retourne la valeur de la feuille 
    {  
        return a->val;
    }
    else if (a->type == '1') 
    // si c'est un opérateur, on va effectuer le calcul en récursif
    // dans le fils gauche et/ou avec le fils droit si nécessaire
    {
        if (strcmp("+", a->op) == 0)
            return calcul(a->fg)+calcul(a->fd);
        
        if (strcmp("-", a->op) == 0)
            return calcul(a->fg)-calcul(a->fd);

        if (strcmp("min", a->op) == 0)
            return fmin( calcul(a->fg) ,calcul(a->fd) );

        if (strcmp("max", a->op) == 0)
            return fmax( calcul(a->fg), calcul(a->fd) );

        if (strcmp("/", a->op) == 0)
            return calcul(a->fg)/calcul(a->fd);

        if (strcmp("*", a->op) == 0)
            return calcul(a->fg)*calcul(a->fd);

        if (strcmp("sqrt",a->op) == 0 )
            return sqrt(calcul(a->fg));

        if (strcmp("exp", a->op)==0)
            return exp(calcul(a->fg));

        if (strcmp("ln", a->op) == 0)
            return log(calcul(a->fg));

        if (strcmp("abs",a->op) == 0)
            return abs(calcul(a->fg));

        if (strcmp("floor", a->op) ==0)
            return floor(calcul(a->fg));

        if (strcmp("ceil", a->op) == 0)
            return ceil( calcul(a->fg) );
    }
}

void principal(table t)
{
    char buff[MAXSIZE]; // utilisé par fgets
    char * token;       // utilisé par strtok
    double res;         // stocke notre résultat final
    
    // fgets permet de lire l'entrée standard
    while((fgets(buff, MAXSIZE, stdin))!=NULL)
    {
        // je m'occupe immédiatement du commentaire et \n
        if (buff[0]!='#' && buff[0] !='\n') 
        {
            buff[strlen(buff)-1]='\0';
            t->nb_elem=0;
            t->pos=0;
            // strtok nous permet de passer de char en char
            // et on va les mémoriser 1 par 1
            // dans notre table
            token=strtok(buff, " ");
            // si il n'y a que des espaces, on ne fait rien
            if (token == NULL)
            {   
                continue;                
            }
            while(token != NULL)
            {
                t->tab[t->nb_elem]=token;
                t->nb_elem++;
                token=strtok(NULL, " ");
            }
            t->tab[t->nb_elem]=NULL;

            // creation de l'arbre via la table 
            // ou l'on n'a stocké l'entrée de l'utilisateur
            tree a = arborescence(t);
            // on vérifie que l'arbre renvoyé ne soit pas vide 
            // ou qu'il y ait eu une erreur
            if (a==NULL)
            {
                fprintf(stderr, "input error. \n");
                liberer_arbre(a);
                continue;
            }
        
            // calcul de l'arbre par un parcours récursif
            res=calcul(a);
            // on renvoie au final la valeur
            fprintf(stdout,"%f \n", res);
            // on libère l'arbre
            liberer_arbre(a);
        }
    }
}


