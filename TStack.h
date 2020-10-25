/* OPREA-GROZA Gabriel - 313CB */
#include <stdlib.h>
#include <string.h>

#define VF(a) (((ASt)(a))->vf)
#define DIMES(a) (((ASt)(a))->dime)
#define VidaS(a) (VF(a) == NULL)

//Pointer la o functie de eliberare de memorie
typedef void (*TF)(void*);

//Structura celulei generice de stiva
typedef struct celst
{
    struct celst *urm;
    void* info;
}TCelSt, *ACelSt;

//Structura stivei
typedef struct stack
{
    size_t dime; //dimensiunea unui element
    ACelSt vf; //pointer catre varful stivei
}TStack, *ASt;

//Functie ce aloca memorie pentru o stiva goala, cu elemente de dimensiune d
void* InitS(size_t d);

//Functie ce adauga referinta data in ae in stiva a
void Push(void* a, void* ae);

/*Functie ce intoarce in adresa de memorie ae referinta la elementul de la
varful stivei, eliminand elementul din stiva */
int Pop(void* a, void* ae);

/* Functie ce intoarce in adresa de memorie ae referinta la elementul de la
varful stivei */
int Top(void* a, void* ae);

/* Functie ce primeste ca parametri adresa unei stive si un pointer la o
functie de eliberare de memorie pentru un element din stiva si distruge
lista generica a stivei, eliberand si memoria pentru stiva ulterior */
void DistrS(void** a, TF free_elem);
