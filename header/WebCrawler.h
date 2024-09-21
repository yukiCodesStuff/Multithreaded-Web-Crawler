#pragma once

#define IPV4_BUF_SIZE 16

#include <mutex>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "WebClientUrl.h"

class WebCrawler {
private:
    std::mutex _mutex;
    std::queue<char*> _q; // stores char* URLs; converts after
    pthread_t* _threads;

    char* _fileBuf;

    int DoDNSLookup(WebClientUrl &webClientUrl);
    int DoForwardLookup(WebClientUrl &webClientUrl, struct addrinfo &hints, struct addrinfo* &result, char *ipstr);
    int DoReverseLookup(WebClientUrl &webClientUrl);

    int DoConnect(WebClientUrl &webClientUrl, struct addrinfo* &result, char *ipstr);
public:
    WebCrawler();
    ~WebCrawler();

    void LoadQueue(FILE* file);
    void Crawl(int numThreads);
    void Run();
    void ProcessUrl(WebClientUrl &webClientUrl);

    static void* ThreadStartRoutine(void *ptr);
};