#include "WebClientUrl.h"

WebClientUrl::WebClientUrl() {
    this->baseUrl = "";
}

WebClientUrl::WebClientUrl(std::string baseUrl) : baseUrl(baseUrl) {} 