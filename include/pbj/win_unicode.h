///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/win_unicode.h
///
/// \brief  Provides functions for converting between UCS-2 (does windows have
///         full UTF-16 support yet? I don't think so) and UTF-8 and
///         vice-versa.
///
/// \author Benjamin Crist

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
