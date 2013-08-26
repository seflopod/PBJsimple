///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/win_unicode.cpp
///
/// \brief  Provides functions for converting between UCS-2 (does windows have
///         full UTF-16 support yet? I don't think so) and UTF-8 and
///         vice-versa.
///
/// \author Benjamin Crist

#include "pbj/win_unicode.h"

#ifdef PBJ_WIN_UNICODE_H_

#include <Windows.h>

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a UCS-2 wchar_t* string to a UTF-8 std::string.
///
/// \param  s The string to convert from UCS-2 to UTF-8.
/// \return The UTF-8 encoded version of s.
std::string narrow(const wchar_t *s)
{
    int wide_size = std::char_traits<wchar_t>::length(s);
    int narrow_size = WideCharToMultiByte(CP_UTF8, 0, s, wide_size, NULL, 0,
                                          NULL, NULL);
    std::string narrow_str(narrow_size, 0);
    WideCharToMultiByte(CP_UTF8, 0, s, wide_size, &narrow_str[0], narrow_size,
                        NULL, NULL);
    return narrow_str;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a UCS-2 std::wstring to a UTF-8 std::string.
///
/// \param  s The string to convert from UCS-2 to UTF-8.
/// \return The UTF-8 encoded version of s.
std::string narrow(const std::wstring &s)
{
    int narrow_size = WideCharToMultiByte(CP_UTF8, 0, &s[0], (int)s.size(),
                                          NULL, 0, NULL, NULL);
    std::string narrowStr(narrow_size, 0);
    WideCharToMultiByte(CP_UTF8, 0, &s[0], (int)s.size(), &narrowStr[0],
                        narrow_size, NULL, NULL);
    return narrowStr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a UTF-8 char* string to a UCS-2 std::wstring.
///
/// \param  s The string to convert from UTF-8 to UCS-2
/// \return The UCS-2 version of s.
std::wstring widen(const char *s)
{
    int narrowSize = std::char_traits<char>::length(s);
    int wideSize = MultiByteToWideChar(CP_UTF8, 0, s, narrowSize, NULL, 0);
    std::wstring wideStr(wideSize, 0);
    MultiByteToWideChar(CP_UTF8, 0, s, narrowSize, &wideStr[0], wideSize);
    return wideStr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Converts a UTF-8 std::string to a UCS-2 std::wstring.
///
/// \param  s The string to convert from UTF-8 to UCS-2
/// \return The UCS-2 version of s.
std::wstring widen(const std::string &s)
{
    int wideSize = MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), NULL,
                                       0);
    std::wstring wideStr(wideSize, 0);
    MultiByteToWideChar(CP_UTF8, 0, &s[0], (int)s.size(), &wideStr[0],
                        wideSize);
   return wideStr;
}

} // namespace pbj

#endif
