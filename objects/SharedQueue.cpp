#include "SharedQueue.h"

SharedQueue::SharedQueue() {}

SharedQueue::~SharedQueue() {}

void SharedQueue::push(WebClientUrl url) {
    this->q.push(url);
}

WebClientUrl SharedQueue::pop() {
    if (this->q.empty()) {
        return WebClientUrl();
    }

    WebClientUrl temp = this->q.front();
    this->q.pop();
    return temp;
}