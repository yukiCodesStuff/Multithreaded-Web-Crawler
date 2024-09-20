#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class WebClientUrl {
private:
    char* baseUrl = NULL;

    char* scheme = NULL;
    char* host = NULL;
    char* path = NULL;
    char* query = NULL;
    char* fragment = NULL;

    char* port = NULL;
public:
    WebClientUrl();
    WebClientUrl(char* baseUrl);

    void PopulateAttr(char* &baseUrl);
    void PrintUrlContents();
};