#include <stdio.h>
#include <config.h>

char* genPostRequest(const char host[] , const char url[], const char* data, int dataLen) {
    char* req = new char[1024];
    sprintf(req, "POST %s HTTP/1.1\nAuthorization: Bearer %s\nHost: %s\nAccept: application/json\nContent-Type: application/json\nContent-Length: %d\n\n%s", 
        url, BACKEND_TOKEN, host, dataLen, data);
    return req;
}