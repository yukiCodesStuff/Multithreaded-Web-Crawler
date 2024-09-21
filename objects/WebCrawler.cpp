#include "../header/WebCrawler.h"

#define INITIAL_BUF_SIZE 64*1024
#define THRESHOLD 32*1024

#define ROBOTS_MAX 16384

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
    struct addrinfo *result;
    char *ipstr = new char[IPV4_BUF_SIZE];
    WebCrawler::DoForwardLookup(webClientUrl, hints, result, ipstr);
    WebCrawler::DoConnect(webClientUrl, result, ipstr);

    delete[] ipstr;
    freeaddrinfo(result);
}

int WebCrawler::DoDNSLookup(WebClientUrl &webClientUrl) {
    // TODO
}

int WebCrawler::DoForwardLookup(WebClientUrl &webClientUrl, struct addrinfo &hints, struct addrinfo* &result, char *ipstr) {
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;    /* Allow only IPv4 */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    hints.ai_flags = AI_CANONNAME;
    hints.ai_protocol = 0;          /* Any protocol */

    int s = getaddrinfo(webClientUrl.getHost(), NULL, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        return -1;
    }

    if (result) {
        void *addr = &((struct sockaddr_in *)result->ai_addr)->sin_addr;
        inet_ntop(result->ai_family, addr, ipstr, IPV4_BUF_SIZE);
        printf("First available IP Address for %s: %s\n", webClientUrl.getHost(), ipstr);
    } else {
        printf("No addresses found for %s\n", webClientUrl.getHost());
    }
    return 0;
}


int WebCrawler::DoReverseLookup(WebClientUrl &webClientUrl) {

}

int WebCrawler::DoConnect(WebClientUrl &webClientUrl, struct addrinfo* &result, char *ipstr) {
    struct sockaddr_in serv_addr;
    int clientfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Est TCP Connection
    if (clientfd < 0) {
        perror("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_port = htons(static_cast<unsigned short>(std::stoi(webClientUrl.getPort())));

    printf("In connect with IP addr: %s\n", ipstr);

    if (inet_pton(AF_INET, ipstr, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / address not supported\n");
        return -1;
    }

    printf("Checkpoint 1\n");

    int status;
    if ((status = connect(clientfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        perror("Failed connection");
        fprintf(stderr, "Error code: %d\n", errno);
        close(clientfd);
        return -1;
    }

    printf("Checkpoint 2\n");

    char sendBuf[BUFSIZ];
    snprintf(sendBuf, BUFSIZ,
             "HEAD /robots.txt HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-agent: yukiTAMUCrawler/1.0\r\n"
             "Connection: close\r\n\r\n",
             webClientUrl.getHost());

	send(clientfd, sendBuf, strlen(sendBuf), 0);

    printf("Sent %s\n", sendBuf);

	// receive response
	int ret, currPos = 0, bufSize = INITIAL_BUF_SIZE;
	char* recvBuf = (char*)malloc(INITIAL_BUF_SIZE);

	while (true) {
        int bytes = recv(clientfd, recvBuf + currPos, bufSize - currPos, 0);
        if (bytes == 0) { // nothing left to read
            break;
        } else if (bytes < 0) { // error occurred
            perror("Error occurred while receiving");
            free(recvBuf);
            close(clientfd);
            return -1;
        }

        currPos += bytes;

        // Check if we need to resize the buffer
        if (currPos >= bufSize - THRESHOLD) {
            bufSize += THRESHOLD;
            char* newBuf = (char*)realloc(recvBuf, bufSize);
            if (newBuf == NULL) {
                perror("Buffer reallocation failed");
                free(recvBuf);
                close(clientfd);
                return -1;
            }
            recvBuf = newBuf;
        }
    }

    recvBuf[currPos] = '\0';
    printf("%s\n", recvBuf);
    close(clientfd);
	free(recvBuf);
}

void* WebCrawler::ThreadStartRoutine(void *ptr) {
    WebCrawler* crawler = static_cast<WebCrawler*>(ptr);

    // testing threads
    // pthread_t threadId = pthread_self(); // Get the thread ID
    // printf("Hello World from thread %lu!\n", threadId);

    crawler->Run();
    return NULL;
}