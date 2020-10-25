/* OPREA-GROZA Gabriel - 313CB */
#include "functii.h"

//Functie ce elibereaza un pointer la un sir de caractere de la adresa info
void freeString(void* info)
{
    //Fac castul corespunzator
    char** val = (char**)info;
    free(*val); //eliberez sirul
    //Eliberez pointerul
    free(val);
}

/*Functie ce elibereaza un pointer la o pagina web de la adresa info, impreuna
cu pagina Web asociata */
void freeWeb(void* info)
{
    TWebPage** val = (TWebPage**)info;
    Resource* res = (*val)->resources;
    free(res);
    free(*val);
    free(val);
}

//Functie ce elibereaza resursele de la adresa info
void freeResources(void* info)
{
    Resource* res = (Resource*)info;
    free(res);
}

//Functie ce elibereaza memorie pentru o celula din lista de taburi
void freeTabCell(TLDI* delTab)
{
    TLDI del = *delTab; //Obtin tabul de eliminat
    //Elibereaza memorie stive
    DistrS(&del->info.backStack, freeWeb);
    DistrS(&del->info.frontStack, freeWeb);

    //Eliberez resursele si pagina curenta din tab, daca exista
    if(del->info.currentPage != NULL)
    {
        free(del->info.currentPage->resources);
        free(del->info.currentPage);
    }
    free(del);

    //Setez zona eliberata la null
    *delTab = NULL;
}

/*Functie ce primeste o lista de taburi si pointer la tabul curent si
adauga un nou tab in lista, setand tabul curent la cel nou creat */
void openTab(TLDI tabList, TLDI* currentTab)
{
    TTab newTab;

    newTab.backStack = InitS(sizeof(TWebPage*));
    if(!newTab.backStack) exit(ERROR);
    newTab.frontStack = InitS(sizeof(TWebPage*));
    if(!newTab.frontStack) exit(ERROR);
    newTab.currentPage = NULL;

    TLDI newCell = calloc(1, sizeof(TCelulaD));
    if(!newCell) exit(ERROR);
    newCell->info = newTab;

    tabList->pred->next = newCell;
    newCell->pred = tabList->pred;
    newCell->next = tabList;
    tabList->pred = newCell;

    *currentTab = newCell;
}

/*Functie ce primeste un nod ce corespunde cu tab-ul curent, coada istoricului
si un sir de caractere al url-ului si deschide in tab-ul curent o noua pagina*/
void openPage(TLDI currentTab, void* history, char* url)
{
    //Daca tab-ul curent este gol, aloc memorie pentru pagina
    if(!currentTab->info.currentPage)
    {
        currentTab->info.currentPage = calloc(1, sizeof(TWebPage));

        if(!currentTab->info.currentPage)
            exit(ERROR);
    }
    else
    {
        /*Daca avea o pagina, inseamna ca trebuie sa adaug pagina anterioara
        in stiva de back */
        //Aloc pointer pentru pagina veche(cea care era in tab)
        TWebPage** oldPage = calloc(1, sizeof(TWebPage*));
        if(!oldPage) exit(ERROR);
        //Aloc memorie pentru pagina
        *oldPage = calloc(1, sizeof(TWebPage));
        if(!oldPage) exit(ERROR);

        //Copiez in oldPage pagina care era deja in tab
        memcpy(*oldPage, currentTab->info.currentPage, sizeof(TWebPage));
        //Adaug referinta la pointer-ul la structura de tip pagina in stiva
        Push(currentTab->info.backStack,oldPage);

        //Eliberez referinta adaugata(stiva va face o copie)
        free(oldPage);
    }

    //Aloc memorie pentru un intreg care retine nr de resurse din pagina noua
    int* n = calloc(1, sizeof(int));
    if(!n) exit(ERROR);
    //Copiez in pagina curenta url-ul si obtin resursele si nr lor
    strcpy(currentTab->info.currentPage->url, url);
    currentTab->info.currentPage->resources = get_page_resources(url, n);
    currentTab->info.currentPage->num_res = (unsigned int)(*n);

    //Eliberez memoria pentru intregul alocat anterior
    free(n);

    //Aloc o referinta la un sir de caractere
    char** newUrl = calloc(1, sizeof(char*));
    if(!newUrl) exit(ERROR);
    //Aloc in referinta noua creata memorie pentru un url si copiez url-ul
    *newUrl = calloc(strlen(url) + 1, sizeof(char));
    if(!*newUrl) exit(ERROR);
    strcpy(*newUrl, url);

    //Adaug referinta nou alocata pentru url-ul curent in coada istoricului
    Enqueue(history, newUrl);

    free(newUrl);
}

//Functie ce afiseaza toate tab-urile din lista dublu inlantuita
void printTabs(TLDI tabList, FILE* output)
{
    //Pentru fiecare tab afisez index-ul sau si url-ul paginii curente
    TLDI p = tabList->next;
    int index = 0;
    while(p != tabList)
    {
        if(p->info.currentPage == NULL)
            fprintf(output, "(%d: empty)\n", index);
        else
            fprintf(output, "(%d: %s)\n", index, p->info.currentPage->url);
        p = p->next;
        index++;
    }
}

//Functie ce schimba tab-ul curent la cel cu indice index din lista
void changeTab(TLDI tabList, TLDI* currentTab, int index)
{
    int i = 0;
    TLDI p = tabList->next;
    //Parcurg lista pana gasesc indicele
    while(p != tabList && i < index)
    {
        p = p->next;
        i++;
    }

    //Setez tab-ul curent la celula gasita
    *currentTab = p;
}

//Functie ce realizeaza operatie de Undo intr-un tab
void undoPage(TLDI currentTab)
{
    /*Aloc memorie pentru un element de stiva, ce contine un pointer
    catre o pagina Web */
    TWebPage** stackEl = calloc(1, sizeof(TWebPage*));
    if(!stackEl) exit(ERROR);
    //Copiez in acea referinta informatia din stiva de back
    Pop(currentTab->info.backStack, stackEl);

    //Aloc inca un element de stiva pentru cea de forward
    TWebPage** currentPage = calloc(1, sizeof(TWebPage*));
    if(!currentPage) exit(ERROR);
    *currentPage = calloc(1, sizeof(TWebPage));
    if(!*currentPage) exit(ERROR);

    memcpy(*currentPage, currentTab->info.currentPage, sizeof(TWebPage));
    //Adaug pagina curenta in stiva de forward
    Push(currentTab->info.frontStack, currentPage);

    //Copiez in pagina curenta pe cea extrasa din stiva de back
    memcpy(currentTab->info.currentPage, *stackEl, sizeof(TWebPage));

    //Eliberez elementele alocate
    free(*stackEl);
    free(stackEl);
    free(currentPage);
}

//Functie ce va realiza operatia de Redo intr-un tab
void redoPage(TLDI currentTab)
{
    TWebPage** stackEl = calloc(1, sizeof(TWebPage*));
    if(!stackEl) exit(ERROR);
    Pop(currentTab->info.frontStack, stackEl);

    TWebPage** currentPage = calloc(1, sizeof(TWebPage*));
    if(!currentPage) exit(ERROR);
    *currentPage = calloc(1, sizeof(TWebPage));
    if(!*currentPage) exit(ERROR);
    memcpy(*currentPage, currentTab->info.currentPage, sizeof(TWebPage));
    Push(currentTab->info.backStack, currentPage);

    memcpy(currentTab->info.currentPage, *stackEl, sizeof(TWebPage));

    free(*stackEl);
    free(stackEl);
    free(currentPage);
}

//Functie ce elimina tab-ul de la sfarsitul listei
void deleteTab(TLDI tabList, TLDI* currentTab)
{
    TLDI ant = tabList->pred->pred;
    TLDI del = tabList->pred;

    /*Daca tab-ul sters este cel curent, setez tab-ul curent la
    cel mai recent tab deschis */
    if(*currentTab == del)
        *currentTab = ant;

    ant->next = tabList;
    tabList->pred = ant;

    freeTabCell(&del);
}

//Functie ce sterge nrEntries elemente din coada history
void deleteHistory(void* history, int nrEntries)
{

    int nrDeleted = 0;
    /*Aloc memorie pentru un element din coada de istoric(referinta catre
    sirul de caractere al url-ului) */
    char** queueEl = calloc(1, sizeof(char*));
    if(!queueEl) exit(ERROR);

    /*Extrag din coada pana cand extrag suficiente pagini, sau pana cand
    aceasta devine vida */
    while(!VidaQ(history))
    {
        Dequeue(history, queueEl);
        nrDeleted++;
        free(*queueEl); //eliberez memorie pentru elementul din coada
        if(nrDeleted == nrEntries)
            break;
    }
    free(queueEl);
}

/* Functie de comparatie ce va intoarce val pozitiva daca la prima adresa se
afla o resursa ce are nr de octeti ramasi de descarcat mai mare, respectiv
valoare negativa in caz contrar */
int remainingSize(void* el1, void* el2)
{
    Resource res1 = *(Resource*)el1;
    Resource res2 = *(Resource*)el2;

    //Obtin dimensiunea ramasa de descarcat din fiecare resursa
    unsigned long remaining1 = res1.dimension - res1.currently_downloaded;
    unsigned long remaining2 = res2.dimension - res2.currently_downloaded;

    return remaining1 - remaining2;
}

//Functie ce afiseaza coada istoricului in fisierul output
void printHistory(void* history, FILE* output)
{
    //Folosesc o coada auxiliara
    void* qaux = InitQ(sizeof(char*));
    if(!qaux) exit(ERROR);
    char** queueEl = calloc(1, sizeof(char*));
    if(!queueEl) exit(ERROR);

    //Obtin elementele din coada initiala, iar la final le adaug in cea aux
    while(!VidaQ(history))
    {
        Dequeue(history, queueEl);
        fprintf(output,"%s\n", *queueEl);
        Enqueue(qaux, queueEl);
    }
    //Golesc coada aux, adaugand inapoi in cea initiala
    ConcatQ(history, qaux);
    DistrQ(&qaux, freeString);
    free(queueEl);
}
