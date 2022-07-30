#pragma once

#include <string>
#include <fstream>

#include "../includes.h"

/**
 * @brief Narrow wide string to string
 *
 * @param str Wide string to narrow
 *
 * @returns Narrowed string
 */
std::string narrow(const wchar_t* str);
inline auto narrow(const std::wstring& str) { return narrow(str.c_str()); }

/**
 * @brief Widen string to wide string
 *
 * @param str String to widen
 *
 * @returns Widened wstring
 */
std::wstring widen(const char* str);
inline auto widen(const std::string& str) { return widen(str.c_str()); }

// Above code was taken from https://github.com/matcool/ReplayBot/blob/rewrite/src/utils.hpp

/**
 * @brief Read binary data from file
 *
 * @param file File to read from
 *
 * @returns Binary data
 */
template<typename T>
inline T readBinary(std::ifstream& file) {
    T value;
    file.read(reinterpret_cast<char*>(&value), sizeof(T));
    return value;
}

/**
 * @brief Write binary data to file
 *
 * @param file File to write to
 * @param value Value to write
 */
template<typename T>
inline void writeBinary(std::ofstream& file, const T value) {
    file.write(reinterpret_cast<const char*>(&value), sizeof(T));
}