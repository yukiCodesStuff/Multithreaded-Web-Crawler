#pragma once

#include <queue>
#include "WebClientUrl.h"

class SharedQueue {
private:
    std::queue<WebClientUrl> q;
public:
    SharedQueue();
    void push(WebClientUrl url);
    WebClientUrl pop();
};