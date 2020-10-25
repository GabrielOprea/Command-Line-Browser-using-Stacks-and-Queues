/* OPREA-GROZA Gabriel - 313CB */
#include "TStack.h"     //Implementare stiva generica cu liste generice
#include "TQueue.h"     //Implementare coada generica cu liste generice
#include "utils.h"      //Resources
#include "structures.h" //Structurile utilizate pentru liste, pagina Web si Tab


//Functii generice de eliberare de memorie
void freeString(void* info);
void freeWeb(void* info);
void freeResources(void* info);
void freeTabCell(TLDI* delTab);

//Functii de parcurgere si prelucrare a listelor de descarcari/tab-uri
TLDI InitLDI();
TLDesc InitLDI_Desc();
void ResetLDI(TLDI s);
void DistrLDI(TLDI* as);
void ResetLDI_Desc(TLDesc s);
void DistrLDI_Desc(TLDesc* as);

/*Functii de navigare web: deschidere/inchidere de pagini sau
tab-uri, undo/redo */
void openTab(TLDI tabList, TLDI* currentTab);
void openPage(TLDI currentTab, void* history, char* url);
void printTabs(TLDI tabList, FILE* output);
void changeTab(TLDI tabList, TLDI* currentTab, int index);
void undoPage(TLDI currentTab);
void redoPage(TLDI currentTab);
void deleteTab(TLDI tabList, TLDI* currentTab);

//Functii de prelucare a istoricului
void deleteHistory(void* history, int nrEntries);
void printHistory(void* history, FILE* output);

//Functii pentru descarcarea resurselor
void setBandwidth(int* bandwidth, int val);
int remainingSize(void* el1, void* el2);
void wait(void* priorities, int seconds, int bandwidth, TLDesc completeDesc);
void startDownload(void* priorities, TLDI currentTab, int index);
void printDownloads(TLDI currentTab, FILE* output);
void showDownloads(void* priorities, TLDesc completeDesc, FILE* output);

//Functii de initializare, distrugere
void DestroyAll(void** history, void** priorities, TLDesc* completeDesc,
TLDI* tabList, FILE* input, FILE* output, char* cmd);
void InitAll(int* bandwidth, void** priorities, void** history,
TLDesc* completeDesc, TLDI* tabList, TLDI* currentTab, char** cmd,
FILE** input, FILE** output, char* input_name, char* output_name);

//Consola interactiva
void Console(char* input_name, char* output_name);








