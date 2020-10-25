/* OPREA-GROZA Gabriel - 313CB */
#include "functii.h"

/*Functie ce elibereaza toata memoria alocata pentru cozile de istoric,
descarcari, listele de taburi si descarcari, comanda, si inchide
fisierele deschise */
void DestroyAll(void** history, void** priorities, TLDesc* completeDesc,
TLDI* tabList, FILE* input, FILE* output, char* cmd)
{
    fclose(input);
    fclose(output);
    free(cmd);

    DistrQ(history, freeString);
    DistrQ(priorities, freeResources);
    DistrLDI_Desc(completeDesc);
    DistrLDI(tabList);
}

/*Functie ce initializeaza si aloca memorie pentru bandwidth, cozi,
liste de taburi/descarcari, comanda si deschide fisierele */
void InitAll(int* bandwidth, void** priorities, void** history,
TLDesc* completeDesc, TLDI* tabList, TLDI* currentTab, char** cmd,
FILE** input, FILE** output, char* input_name, char* output_name)
{
    *bandwidth = 1024;

    *priorities = InitQ(sizeof(Resource));
    if(!*priorities) exit(ERROR);


    *tabList = InitLDI();

    *history = InitQ(sizeof(char*));
    if(!(*history))
        exit(ERROR);

    *completeDesc = InitLDI_Desc();
    if(!*completeDesc) exit(ERROR);

    *input = fopen(input_name, "r");
    if(!*input) return;
    *output = fopen(output_name, "w");
    if(!*output) return;

    *cmd = calloc(MAX_CMD_LEN, sizeof(char));
    if(!*cmd) exit(ERROR);

    openTab(*tabList, currentTab);
}

/*Consola interactiva ce va primi numele celor 2 fisiere de input/output, le va
deschide si va citi din input comenzi pana la intalnirea EOF. */
void Console(char* input_name, char* output_name)
{
    int bandwidth; //nr de bytes descarcati pe secunda
    void *priorities, *history; //cozi de prioritati/istoric
    TLDI tabList, currentTab; //lista de taburi si tabul curent
    TLDesc completeDesc; //lista de descarcari finalizate
    char* cmd; //comanda curenta
    FILE *input, *output;
    //Initializez toata variabilele necesare
    InitAll(&bandwidth, &priorities, &history, &completeDesc, &tabList,
    &currentTab, &cmd, &input, &output, input_name, output_name);

    while(1) //citesc comenzi pana cand fscanf returneaza cod de eroare
    {
        char* verif = fgets(cmd, MAX_CMD_LEN, input);
        if(verif == NULL) break;
        cmd[strcspn(cmd, "\n")] = '\0'; //elimin newline-ul din comanda
        char* partCmd = strtok(cmd, " "); //impart comanda in token-uri

        /*Pentru fiecare comanda gasita, preiau argumentele urmatoare si apelez
        functia corespunzatoare */
        if(!strcmp(partCmd, "set_band"))
        {
            partCmd = strtok(NULL, " ");
            int newBandwidth = atoi(partCmd);
            setBandwidth(&bandwidth, newBandwidth);
        }
        else if(!strcmp(partCmd, "newtab"))
            openTab(tabList, &currentTab);
        else if(!strcmp(partCmd, "deltab"))
            deleteTab(tabList, &currentTab);
        else if(!strcmp(partCmd, "change_tab"))
        {
            partCmd = strtok(NULL, " ");
            int index = atoi(partCmd);
            changeTab(tabList, &currentTab, index);
        }
        else if(!strcmp(partCmd, "print_open_tabs"))
            printTabs(tabList, output);
        else if(!strcmp(partCmd, "goto"))
        {
            partCmd = strtok(NULL, " ");
            openPage(currentTab, history, partCmd);
            wait(priorities, 1, bandwidth, completeDesc);
        }
        else if(!strcmp(partCmd, "back"))
            undoPage(currentTab);
        else if(!strcmp(partCmd, "forward"))
            redoPage(currentTab);
        else if(!strcmp(partCmd, "history"))
            printHistory(history, output);
        else if(!strcmp(partCmd, "del_history"))
        {
            partCmd = strtok(NULL, " ");
            int entries = atoi(partCmd);
            deleteHistory(history, entries);
        }
        else if(!strcmp(partCmd, "list_dl"))
            printDownloads(currentTab, output);
        else if(!strcmp(partCmd, "downloads"))
            showDownloads(priorities, completeDesc, output);
        else if(!strcmp(partCmd, "download"))
        {
            partCmd = strtok(NULL, " ");
            int index = atoi(partCmd);
            startDownload(priorities, currentTab, index);
        }
        else if(!strcmp(partCmd, "wait"))
        {
            partCmd = strtok(NULL," ");
            int seconds = atoi(partCmd);
            wait(priorities, seconds, bandwidth, completeDesc);
        }
        else printf("Comanda gresita\n");
    }
    DestroyAll(&history, &priorities, &completeDesc, &tabList, input,
    output, cmd);
}

int main(int argc, char* argv[])
{
    if(argc > 3)
    {
        printf("%s\n", "Too many arguments");
        return -1;
    }
    else if(argc < 3)
    {
        printf("%s\n", "Not enough arguments");
        return -1;
    }
    /*Primul argument(pe langa numele executabilului) va fi numele
    fisierului de intrare, urmat de numele fisierului de iesire */
    Console(argv[1], argv[2]);
}
