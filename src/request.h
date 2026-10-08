#ifndef REQUEST_H
#define REQUEST_H

#include <cstdint>
#include <string>

bool send_request(int fd, std::string host, std::string path, uint32_t stream);
int read_response(int fd, uint32_t stream);

#endif
