#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include "request.h"
#include "frame.h"
#include "headers.h"
#include "dump.h"

using namespace std;

bool send_request(int fd, string host, string path, uint32_t stream) {
    vector<uint8_t> h;
    add_header(h, ":method", "GET");
    add_header(h, ":path", path);
    add_header(h, "host", host);
    add_header(h, "user-agent", "bcurl/1.0");
    add_header(h, "accept", "*/*");

    size_t len = h.size();
    vector<uint8_t> f(9 + len);
    f[0] = len >> 16;
    f[1] = len >> 8;
    f[2] = len;
    f[3] = T_HEADERS;
    f[4] = F_END;
    f[5] = stream >> 24;
    f[6] = stream >> 16;
    f[7] = stream >> 8;
    f[8] = stream;
    memcpy(&f[9], h.data(), len);

    if (verbose) dump_frame(">", f);
    return write_full(fd, f.data(), f.size());
}

// returns the status code, or 0 if something went wrong
int read_response(int fd, uint32_t stream) {
    int status = 0;
    while (true) {
        vector<uint8_t> f(9);
        if (!read_full(fd, f.data(), 9)) return 0;
        uint32_t len = (f[0] << 16) | (f[1] << 8) | f[2];
        f.resize(9 + len);
        if (len > 0 && !read_full(fd, &f[9], len)) return 0;
        if (verbose) dump_frame("<", f);

        uint8_t type = f[3];
        uint8_t flags = f[4];
        uint32_t s = ((f[5] & 0x7f) << 24) | (f[6] << 16) | (f[7] << 8) | f[8];
        if (s != stream) continue;

        vector<uint8_t> payload(f.begin() + 9, f.end());
        if (type == T_HEADERS) {
            map<string, string> h;
            if (!parse_headers(payload, h)) return 0;
            status = atoi(h[":status"].c_str());
        } else if (type == T_DATA) {
            fwrite(payload.data(), 1, payload.size(), stdout);
        } else {
            // unknown frame type, skip it
            continue;
        }
        if (flags & F_END) return status;
    }
}
