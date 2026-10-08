#ifndef FRAME_H
#define FRAME_H

#include <cstdint>
#include <cstddef>

const uint8_t T_DATA = 0;
const uint8_t T_HEADERS = 1;
const uint8_t F_END = 1;

bool read_full(int fd, uint8_t *buf, size_t n);
bool write_full(int fd, const uint8_t *buf, size_t n);

#endif
