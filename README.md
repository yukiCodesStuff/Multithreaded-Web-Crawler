# Multithreaded-Web-Crawler
Web Crawler that does the job faster by using multithreading

## Usage Guide
Load a text file full of URLs that you hope to crawl into inputFiles.

Run commands in this order:
1. ```make clean```
2. ```make all```
3. ```make test``` <br />
   This simply runs the test file in the ```inputFiles``` directory which has 1,000,000 links in it

## Resources
<a href="https://man7.org/linux/man-pages/man3/getaddrinfo.3.html">getaddrinfo() man page</a>
<a href="https://www.geeksforgeeks.org/socket-programming-cc/">Socket Programming C</a>
<a href="https://www.geeksforgeeks.org/snprintf-c-library/">snprintf()</a>