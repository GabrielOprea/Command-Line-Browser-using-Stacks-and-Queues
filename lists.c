/* OPREA-GROZA Gabriel - 313CB */
#include "functii.h"

//Functie ce transforma o lista de tab-uri intr-una vida
void ResetLDI(TLDI s)
{
    TLDI p = s->next, aux;
    while(p != s)
    {
        aux = p;
        p = p->next;
        freeTabCell(&aux);
    }
    s->next = s->pred = s;
}

//Elibereaza tot spatiul ocupat de lista de tab-uri, inclusiv santinela
void DistrLDI(TLDI* as)
{
    ResetLDI(*as);
    free(*as);
    *as = NULL;
}

//Functie ce reseteaza o lista de descarcari finalizate
void ResetLDI_Desc(TLDesc s)
{
    TLDesc p = s->next, aux;
    while(p != s)
    {
        aux = p;
        p = p->next;
        free(aux);
    }
    s->next = s->pred = s;
}

/*Elibereaza tot spatiul ocupat de lista de descarcari finalizate,
inclusiv santinela */
void DistrLDI_Desc(TLDesc* as)
{
    ResetLDI_Desc(*as);
    free(*as);
    *as = NULL;
}

//Initializeaza o lista de taburi dublu inlantuita circulara, cu santinela
TLDI InitLDI()
{
    //Aloc o celula de lista si verific alocarea
    TLDI aux = (TLDI)malloc(sizeof(TCelulaD));
    if (!aux) return NULL;

    //Setez nodul curent ca santinela
    aux->pred = aux->next = aux;

    return aux;
}

/*Initializeaz o lista dublu inlantuita circulara cu santinela pentru
descarcarile finalizate */
TLDesc InitLDI_Desc()
{
    TLDesc aux = (TLDesc)malloc(sizeof(TCelDesc));
    if (!aux) return NULL;

    aux->pred = aux->next = aux;

    return aux;     /* rezultatul este adresa santinelei sau NULL */
}
