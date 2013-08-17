#ifndef PBJ_WIN_UNICODE_H_
#ifdef _WIN32
#define PBJ_WIN_UNICODE_H_

#include <string>

namespace pbj {

std::string narrow(const wchar_t *s);
std::string narrow(const std::wstring &s);
std::wstring widen(const char *s);
std::wstring widen(const std::string &s);

} // namespace pbj

#endif
#endif
