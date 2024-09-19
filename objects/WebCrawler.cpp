#include "../header/WebCrawler.h"

WebCrawler::WebCrawler() {}

WebCrawler::~WebCrawler() {
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

        printf("%s\n", start);

        // find next URL
		start = lineEnd + 1;
		if (*(lineEnd - 1) == '\r') {
			lineEnd--;
		}
		*(lineEnd) = '\0';
		lineEnd = strchr(start, '\n');
	}
}

void* WebCrawler::ThreadStartRoutine() {}