#include "WebCrawler.h"

WebCrawler::WebCrawler() {
    this->sq = new SharedQueue();
}

WebCrawler::~WebCrawler() {
    delete this->sq;
}

void* WebCrawler::ThreadStartRoutine() {

}