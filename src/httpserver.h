#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <string>
#include <cstdint>

void start_httpserver(const std::string &host, uint16_t port);
void stop_httpserver();

#endif