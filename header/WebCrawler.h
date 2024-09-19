#pragma once

#include <mutex>
#include <queue>
#include <stdio.h>
#include <pthread.h>

#include "WebClientUrl.h"

class WebCrawler {
private:

    std::mutex _mutex;
    std::queue<char*> _q; // stores char* URLs; converts after
    pthread_t* _threads;

    char* fileBuf;
public:
    WebCrawler();
    ~WebCrawler();

    void LoadQueue(FILE* file);
    void Crawl(int numThreads);
    void Run();

    static void* ThreadStartRoutine(void *ptr);
};