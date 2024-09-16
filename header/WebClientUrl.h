#pragma once

#include <string>

class WebClientUrl {
private:
    std::string baseUrl;
public:
    WebClientUrl();
    WebClientUrl(std::string baseUrl);
};