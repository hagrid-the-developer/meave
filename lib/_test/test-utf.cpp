#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <cstring>

#include "lib/utf8.hpp"

namespace $ = std;
namespace $$ = meave;

int
main(void) {
    $::ifstream t("UTF-8-demo.txt");
    $::string str(($::istreambuf_iterator<char>(t)), $::istreambuf_iterator<char>());

    char buf[str.size() + 1];

    const char *p = str.c_str();
    char *q = buf;
    while (*p) {
        wchar_t wch;
        p = $$::utf8_to_wchar(p, wch);
        q = $$::wchar_to_utf8(q, wch);
    }
    *q = 0;

    $::cerr << "cmp=" << !strcmp(str.c_str(), buf) << $::endl;

    return 0;
}
