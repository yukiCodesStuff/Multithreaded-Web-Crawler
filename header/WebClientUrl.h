#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>

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

    const char* getScheme() const;
    const char* getHost() const;
    const char* getPath() const;
    const char* getQuery() const;
    const char* getFragment() const;
    const char* getPort() const;

    void PopulateAttr(char* &baseUrl);
    void PrintUrlContents();
};