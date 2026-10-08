#!/bin/bash
# usage: ./test.sh [host:port]
# needs a server running that serves the files in ./www (your own or somebody else's)
cd "$(dirname "$0")/.."
HP=${1:-localhost:9000}

pass=0
fail=0
check() {
    if [ "$2" = "0" ]; then echo "PASS  $1"; pass=$((pass+1)); else echo "FAIL  $1"; fail=$((fail+1)); fi
}

./bcurl $HP/index.html > out.tmp
cmp -s out.tmp www/index.html; check "GET /index.html matches file" $?

./bcurl $HP/ > out.tmp
cmp -s out.tmp www/index.html; check "GET / gives index.html" $?

./bcurl $HP/sub/page.txt > out.tmp
cmp -s out.tmp www/sub/page.txt; check "file in sub folder" $?

./bcurl $HP/big.bin > out.tmp
cmp -s out.tmp www/big.bin; check "100000 byte binary file identical" $?

./bcurl $HP/empty.txt > out.tmp
[ $? -eq 0 ] && [ ! -s out.tmp ]; check "empty file, exit 0" $?

./bcurl $HP/nope.html > out.tmp 2>/dev/null
code=$?
[ $code -eq 1 ]; check "missing file: exit 1" $?

./bcurl $HP/../secret > /dev/null 2>&1
[ $? -eq 1 ]; check "403 also gives exit 1" $?

./bcurl -v $HP/index.html 2> err.tmp > out.tmp
cmp -s out.tmp www/index.html && grep -q '^> HEADERS' err.tmp && grep -q '^< DATA' err.tmp; check "-v prints frames on stderr only" $?

./bcurl -v $HP/index.html 2>&1 >/dev/null | grep -q '  0000  '; check "-v output has a hexdump" $?

./bcurl $HP/index.html /about.html /sub/page.txt > out.tmp 2>/dev/null
(cat www/index.html www/about.html www/sub/page.txt) > want.tmp
[ $? -eq 0 ] && cmp -s out.tmp want.tmp; check "three files in one run come back in order" $?

./bcurl $HP/index.html /nope.html /about.html > /dev/null 2>&1
[ $? -eq 1 ]; check "several paths in one run, exit 1 because one is 404" $?

[ $(cat src/*.cpp | grep -c "connect(") -eq 1 ]; check "only one connect() in the source" $?

./bcurl localhost:1/index.html > /dev/null 2>&1
[ $? -eq 2 ]; check "no server -> exit 2" $?

./bcurl > /dev/null 2>&1
[ $? -eq 2 ]; check "no arguments -> exit 2" $?

rm -f out.tmp err.tmp want.tmp
echo "$pass passed, $fail failed"
[ $fail -eq 0 ]
