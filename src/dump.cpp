#include <cstdio>
#include "dump.h"
#include "frame.h"

using namespace std;

bool verbose = false;

static void hexdump(const vector<uint8_t> &d) {
    for (size_t i = 0; i < d.size(); i += 16) {
        fprintf(stderr, "  %04zx  ", i);
        for (size_t j = i; j < i + 16; j++) {
            if (j < d.size()) fprintf(stderr, "%02x ", d[j]);
            else fprintf(stderr, "   ");
        }
        fprintf(stderr, " |");
        for (size_t j = i; j < i + 16 && j < d.size(); j++)
            fprintf(stderr, "%c", (d[j] >= 32 && d[j] < 127) ? d[j] : '.');
        fprintf(stderr, "|\n");
    }
}

void dump_frame(const char *dir, const vector<uint8_t> &f) {
    const char *name = "UNKNOWN";
    if (f[3] == T_DATA) name = "DATA";
    if (f[3] == T_HEADERS) name = "HEADERS";
    uint32_t len = (f[0] << 16) | (f[1] << 8) | f[2];
    uint32_t stream = ((f[5] & 0x7f) << 24) | (f[6] << 16) | (f[7] << 8) | f[8];
    fprintf(stderr, "%s %s frame: len=%u flags=0x%02x stream=%u\n", dir, name, len, f[4], stream);
    hexdump(f);
}
