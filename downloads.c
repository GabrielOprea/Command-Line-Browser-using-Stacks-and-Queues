/* OPREA-GROZA Gabriel - 313CB */
#include "functii.h"

//Functie ce seteaza bandwidth-ul la valoarea val
void setBandwidth(int* bandwidth, int val)
{
    *bandwidth = val;
}

//Functie ce afiseaza toate resursele descarcabile dintr-un tab
void printDownloads(TLDI currentTab, FILE* output)
{

    //Verific ca tab-ul sa contina o pagina
    TWebPage* crtPage = currentTab->info.currentPage;
    if(!crtPage)
        return;
    unsigned int i;

    //Pt fiecare resursa, afisez indicele,numele si dimensiunea
    for(i = 0; i < crtPage->num_res; i++)
    {
        fprintf(output, "[%d - \"%s\" : %lu]\n", i,
        crtPage->resources[i].name, crtPage->resources[i].dimension);
    }
}

//Functie ce adauga o resursa in coada de prioritati a descarcarilor
void startDownload(void* priorities, TLDI currentTab, int index)
{
    if(!currentTab->info.currentPage)
        return;

    /*Obtin resursa cu indicele specificat, aloca memorie pentru un element
    din coada */
    Resource currentRes = currentTab->info.currentPage->resources[index];
    Resource* queueEl = calloc(1, sizeof(Resource));
    if(!queueEl) exit(ERROR);
    *queueEl = currentRes;

    //Adaug in coada in functie de prioritate
    EnqueuePriority(priorities, queueEl, remainingSize, freeResources);
    free(queueEl);
}

/* Functie ce afiseaza in fisier toate descarcarile, inclusiv cele finalizate
din lista completeDesc */
void showDownloads(void* priorities, TLDesc completeDesc, FILE* output)
{

    //Pentru a parcurge elementele din coada, folosesc o coada aux
    void* qaux = InitQ(sizeof(Resource));
    if(!qaux) return;

    Resource* queueEl = calloc(1, sizeof(Resource));
    if(!queueEl) exit(ERROR);

    while(!VidaQ(priorities))
    {
        //Obtin dimensiunea ramasa de descarcat a elementului curent
        Dequeue(priorities, queueEl);
        unsigned long remaining = queueEl->dimension -
        queueEl->currently_downloaded;

        //Afisez cu format
        fprintf(output,"[\"%s\" : %lu/%lu]\n", queueEl->name, remaining,
        queueEl->dimension);

        Enqueue(qaux, queueEl);
    }

    ConcatQ(priorities, qaux);

    //La sfarsit, afisez si descarcarile finalizate
    TLDesc p = completeDesc->next;
    while(p != completeDesc)
    {
        fprintf(output,"[\"%s\" : completed]\n", p->info.name);
        p = p->next;
    }
    DistrQ(&qaux, freeResources);
    free(queueEl);
}

/*Functie ce realizeaza trecerea fictiva a timpului, primind ca parametri
coada de prioritati a descarcarilor, nr de secunde, bandwidth-ul si lista
descarcarilor complete, urmand a extrage elemente din Priority Queue si a
le descarca in functie de viteza si nr de secunde */
void wait(void* priorities, int seconds, int bandwidth, TLDesc completeDesc)
{
    int totalBytes = seconds * bandwidth;
    void* qaux = InitQ(sizeof(Resource)); /*coada auxiliara folosita pentru
    reinserarea elementelor in coada initiala */
    if(!qaux) return;

    //Aloc memorie pt un element din coada
    Resource* queueEl = calloc(1, sizeof(Resource));
    if(!queueEl) exit(ERROR);

    /*Iterez cat timp coada de prioritati nu e vida si cat timp mai am bytes
    de descarcat */
    while(!VidaQ(priorities) && totalBytes > 0)
    {
        /*Extrag un element din coada si ii determin nr de bytes ramasi
        de descarcat */
        Dequeue(priorities, queueEl);

        int downloadLeft = queueEl->dimension - queueEl->currently_downloaded;

        //Determin cat am descarcat din resursa curenta
        int crtDown = (downloadLeft > totalBytes ? totalBytes : downloadLeft);
        queueEl->currently_downloaded += crtDown;

        //Scad din nr de bytes ramasi
        totalBytes -= downloadLeft;

        //Daca am descarcat toata resursa
        if(queueEl->dimension == queueEl->currently_downloaded)
        {
            /*Aloc o celula a listei de descarcari finalizate si o adaug
            la sfarsitul listei */
            TLDesc newDesc = calloc(1, sizeof(TCelDesc));
            if(!newDesc) exit(ERROR);
            newDesc->info = *queueEl;

            completeDesc->pred->next = newDesc;
            newDesc->pred = completeDesc->pred;
            newDesc->next = completeDesc;
            completeDesc->pred = newDesc;
        }
        else
            Enqueue(qaux, queueEl); /*Daca nu am descarcat toata resursa,
            o voi reinsera in coada */
    }
    //Pun ce a mai ramas din coada initiala in cea aux
    ConcatQ(qaux, priorities);
    //Copiez din coada aux in cea initiala
    ConcatQ(priorities, qaux);
    free(queueEl);
    //Distrug coada aux
    DistrQ(&qaux, freeResources);
}
