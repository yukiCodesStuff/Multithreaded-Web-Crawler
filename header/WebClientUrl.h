#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class WebClientUrl {
private:
    char* baseUrl = NULL;

    // not nullable
    char* scheme = NULL;
    char* host = NULL;

    // nullable
    char* path = NULL;
    char* query = NULL;
    char* fragment = NULL;

    char* port = NULL;
public:
    WebClientUrl();
    WebClientUrl(char* baseUrl);

    char* getScheme();
    char* getHost();
    char* getPath();
    char* getQuery();
    char* getFragment();
    char* getPort();

    void PopulateAttr(char* &baseUrl);
    void PrintUrlContents();
};