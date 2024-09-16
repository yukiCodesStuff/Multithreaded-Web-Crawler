#pragma once

#include <mutex>
#include "SharedQueue.h"

class WebCrawler {
private:
    std::mutex _mutex;
    SharedQueue* sq;
public:
    WebCrawler();
    ~WebCrawler();
    void* ThreadStartRoutine();
};