/* OPREA-GROZA Gabriel - 313CB */
#include "TStack.h"

//Functie ce distruge lista generica a stive
void DistrugeLGS(ACelSt* aL, TF free_elem)
{
    while(*aL != NULL)
    {
        ACelSt aux = *aL; //adresa celulei de eliminat
        if (!aux)
            return;

        free_elem(aux->info); //eliberez spatiul ocupat de info
        *aL = aux->urm; //deconectez celula din lista
        free(aux);
    }
}

//Fct ce aloca o stiva
void* InitS(size_t d)
{
    ASt s;
    s = (ASt)malloc(sizeof(TStack));
    if(!s) return NULL;

    //Setez campul dime si varful la NULL
    s->dime = d;
    s->vf = NULL;

    //Intorc o stiva generica
    return (void*)s;
}

//Functie ce adauga in stiva elementul de la adresa ae
void Push(void* a, void* ae)
{
    //Aloc o noua celula de stiva
    ACelSt aux = (ACelSt)malloc(sizeof(TCelSt));
    if(!aux)
        return;

    //Copiez info in noua celula
    aux->info = calloc(1, DIMES(a));
    memcpy(aux->info, ae, DIMES(a));

    //O adaug la inceputul listei(in vf stivei)
    aux->urm = VF(a);
    VF(a) = aux;
}

//Functie ce elimina din stiva elementul de la varf
int Pop(void* a, void* ae)
{
    if(VidaS(a))
    {
        VF(a) = NULL;
        return 0;
    }
    //Copiez in ae elemntul de la varf
    memcpy(ae, VF(a)->info, DIMES(a));
    ACelSt aux = VF(a);

    //Il elimin din lista
    VF(a) = aux->urm;

    //Eliberez elementul eliminat
    free(aux->info);
    free(aux);
    return 1;
}

//Functii ce obtine elementul de la vf stivei
int Top(void* a, void* ae)
{
    if(VidaS(a))
    {
        VF(a) = NULL;
        return 0;
    }
    memcpy(ae, VF(a)->info, DIMES(a));
    return 1;
}

/*Functie ce distruge o stiva, utilizand un pointer la o functie de eliberare
de memorie */
void DistrS(void** a, TF free_elem)
{
    //Dereferentiez adresa stivei
    void* s = *a;
    //Obtin inceputul listei generice de celule
    ACelSt aux = VF(s);

    //Distrug lista obtinuta
    DistrugeLGS(&aux, free_elem);

    //Eliberez si descriptorul pt stiva si setez zonele eliberate la NULL
    VF(s) = NULL;
    free(*a);
    a = NULL;
}
