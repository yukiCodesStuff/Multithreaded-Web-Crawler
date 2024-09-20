#include <iostream>

#include "header/WebClientUrl.h"
#include "header/WebCrawler.h"

int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        printf("[ERROR] Usage: main <number of threads> <input file>\n");
        return -1;
    }

    // store inputs
    char* numEnd;
    long int numThreads = strtol(argv[1], &numEnd, 10); // base 10
    char* inputFile = argv[2];

    printf("Opening file %s\n", inputFile);

    FILE *f = fopen(inputFile, "r");

    WebCrawler *crawler = new WebCrawler();
    crawler->LoadQueue(f);
    crawler->Crawl(numThreads);

    delete crawler;

    fclose(f);
    return 0;
}