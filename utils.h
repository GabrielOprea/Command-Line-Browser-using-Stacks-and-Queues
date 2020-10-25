/* OPREA-GROZA Gabriel - 313CB */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifndef _DOWNLOAD_RESOURCE_
#define _DOWNLOAD_RESOURCE_
#define MAX_RESOURCE_LEN 100 //dimensiunea maxima a numelui unei resurse

typedef struct{
    char name[MAX_RESOURCE_LEN];
    unsigned long dimension;
    unsigned long currently_downloaded;
} Resource;

Resource* get_page_resources(const char *URL_Name, int *n);

#endif /* _DOWNLOAD_RESOURCE_ */
