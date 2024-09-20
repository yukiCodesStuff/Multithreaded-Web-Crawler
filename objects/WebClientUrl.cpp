#include "../header/WebClientUrl.h"

WebClientUrl::WebClientUrl() {
    this->baseUrl = "";
}

WebClientUrl::WebClientUrl(char* baseUrl) : baseUrl(baseUrl) {
    PopulateAttr(baseUrl);
    PrintUrlContents();
}

void WebClientUrl::PopulateAttr(char* &baseUrl) {

    // get fragment
    char* fragmentPtr = strchr(baseUrl, '#');
    if (fragmentPtr != NULL) {
        this->fragment = fragmentPtr + 1;
        *fragmentPtr = '\0'; // truncate
    }

    // get query
    char* queryPtr = strchr(baseUrl, '?');
    if (queryPtr != NULL) {
        this->query = queryPtr + 1;
        *queryPtr = '\0'; // truncate
    }

    // get scheme
    char* schemePtr = strchr(baseUrl, ':');
    if (schemePtr == NULL) {
        perror("Improper URL Format\n");
        return;
    }
    this->scheme = baseUrl;
    baseUrl = schemePtr + 3;
    *schemePtr = '\0';
    if (strcmp(this->scheme, "http") != 0 && strcmp(this->scheme, "https") != 0) {
        perror("Unsupported scheme\n");
        return;
    }

    // get path
    char* pathPtr = strchr(baseUrl, '/');
    if (pathPtr != NULL) {
        this->path = pathPtr + 1;
        *pathPtr = '\0';
    }

    // get port
    char* portPtr = strchr(baseUrl, ':');
    if (portPtr != NULL) {
        this->port = portPtr + 1;
        *portPtr = '\0';
    }

    // get host
    this->host = baseUrl;
}

void WebClientUrl::PrintUrlContents() {
    printf("Host: %s\nScheme: %s\nPort: %s\nPath: %s\nQuery: %s\n\n", 
    this->host, this->scheme, this->port, this->path, this->query);
}