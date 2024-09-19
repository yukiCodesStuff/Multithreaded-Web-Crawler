#include <unistd.h>

#include "../header/WebCrawler.h"

WebCrawler::WebCrawler() {}

WebCrawler::~WebCrawler() {
    delete[] this->_threads;
    delete[] this->fileBuf;
}

void WebCrawler::LoadQueue(FILE* file) {

    printf("Opening file...\n");
    if (file == NULL) {
        perror("Failed to open file\n");
        return;
    }

    // get file size
    fseek(file, 0L, SEEK_END);
    long sz = ftell(file);
    fseek(file, 0L, SEEK_SET);

    // initialize buffer; make room for null terminating character
    this->fileBuf = (char *)malloc(sz + 1);
    fread(this->fileBuf, sz, 1, file);
    fileBuf[sz] = '\0';

    // populate queue
    char* start = this->fileBuf;
	char* lineEnd = strchr(fileBuf, '\n');

	while (lineEnd != NULL) {

        // push URL
		this->_q.push(start);

        // find next URL
		start = lineEnd + 1;
		if (*(lineEnd - 1) == '\r') {
			lineEnd--;
		}
		*(lineEnd) = '\0';
		lineEnd = strchr(start, '\n');
	}

    printf("Loaded %zu URLs into queue\n", this->_q.size());
}

void WebCrawler::Crawl(int numThreads) {

    // initialize threads
    this->_threads = new pthread_t[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        if (pthread_create(this->_threads + i, NULL, &WebCrawler::ThreadStartRoutine, this) != 0) {
            perror("Failed to create thread\n");
            return;
        }
    }

    // join threads
    for (int i = 0; i < numThreads; ++i) {
        if (pthread_join(this->_threads[i], NULL) != 0) {
            return;
        }
    }
}

void* WebCrawler::ThreadStartRoutine(void *ptr) {
    WebCrawler* crawler = static_cast<WebCrawler*>(ptr);
    pthread_t threadId = pthread_self(); // Get the thread ID
    sleep(1);
    printf("Hello World from thread %lu!\n", threadId);
    return NULL;
}