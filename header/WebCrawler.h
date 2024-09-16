#pragma once

#include <mutex>

class WebCrawler {
private:
    std::mutex _mutex;
public:
    WebCrawler();
    void* ThreadStartRoutine();
};