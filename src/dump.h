#ifndef DUMP_H
#define DUMP_H

#include <cstdint>
#include <vector>

extern bool verbose;

void dump_frame(const char *dir, const std::vector<uint8_t> &f);

#endif
