/* OPREA-GROZA Gabriel - 313CB */
#include "utils.h"

#define ERROR 1 //cod de eroare returnat la alocari esuate
#define MAX_CMD_LEN 256 //dimensiunea maxima a unei comenzi
#define URL_Len 21 //dimensiunea unui url, 20 caractere + 1 terminator

//Structura paginii web
typedef struct{
    char url[URL_Len]; //contine un url
    unsigned int num_res; //nr de resurse
    Resource* resources; //un vector de resurse
}TWebPage;

//Structura unui Tab
typedef struct Tab{
    void* backStack; //contine o stiva de Back
    void* frontStack; //una de Forward
    TWebPage* currentPage; //si o referinta catre pagina curenta din Tab
}TTab;

//Structura listei dublu inlanuite a taburilor
typedef struct CelulaD{
    TTab info; //info din lista este data de structura tabului
    struct CelulaD* next; //referinta catre urmatoarea celula de lista
    struct CelulaD* pred; //si predecesoarea
}TCelulaD, *TLDI;

//Structura listei dublu inlantuite a descarcarilor finalizate
typedef struct DescFin{
    Resource info; //info din lista este data de resursa complet descarcata
    struct DescFin* next;
    struct DescFin* pred;
}TCelDesc, *TLDesc;
