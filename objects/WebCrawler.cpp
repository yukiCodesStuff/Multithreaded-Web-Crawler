#include "../header/WebCrawler.h"

WebCrawler::WebCrawler() {}

WebCrawler::~WebCrawler() {
    delete[] this->_threads;
    delete[] this->_fileBuf;
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
    this->_fileBuf = (char *)malloc(sz + 1);
    fread(this->_fileBuf, sz, 1, file);
    _fileBuf[sz] = '\0';

    // populate queue
    char* start = this->_fileBuf;
	char* lineEnd = strchr(this->_fileBuf, '\n');

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
    this->_q.push(start); // get last URL at EOF

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
            perror("Failed to join thread\n");
            return;
        }
    }
}

void WebCrawler::Run() {
    while (true) {
        this->_mutex.lock();
        if (this->_q.empty()) {
            this->_mutex.unlock();
            break;
        }

        char* curr = this->_q.front();
        this->_q.pop();
        this->_mutex.unlock();

        WebClientUrl webClientUrl(curr);
        ProcessUrl(webClientUrl);
    }
}

void WebCrawler::ProcessUrl(WebClientUrl &webClientUrl) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int s = getaddrinfo(webClientUrl.getHost(), NULL, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return;
    }

    printf("IP Address for %s: %s\n", webClientUrl.getHost(), result->ai_canonname);

    freeaddrinfo(result);
}

int WebCrawler::DoDNSLookup(WebClientUrl &webClientUrl) {
    // TODO
}

void* WebCrawler::ThreadStartRoutine(void *ptr) {
    WebCrawler* crawler = static_cast<WebCrawler*>(ptr);

    // testing threads
    // pthread_t threadId = pthread_self(); // Get the thread ID
    // printf("Hello World from thread %lu!\n", threadId);

    crawler->Run();
    return NULL;
}