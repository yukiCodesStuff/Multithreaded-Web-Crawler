#pragma once

#include <mutex>
#include <queue>
#include <stdio.h>

#include "WebClientUrl.h"

class WebCrawler {
private:
    std::mutex _mutex;
    std::queue<char*> _q; // stores char* URLs; converts after
    char* fileBuf;
public:
    WebCrawler();
    ~WebCrawler();

    void LoadQueue(FILE* file);

    void* ThreadStartRoutine();
};