/**
 * @file utils.cpp
 * @brief "refn" - High-performance line reader utility.
 * @author Mauricio Ferrari
 * @date 2026-02-02
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

#include "utils.hpp"

using namespace std;

/// Counts the number of lines in a file using a 64KB buffer.
///
/// This function reads the file in binary mode to maximize throughput.
/// It adheres to POSIX-style counting by identifying '\n' characters.
///
/// # Arguments
/// * `f` - A constant pointer to the file path string.
///
/// # Returns
/// The total number of lines detected (unsigned long long).
unsigned long long count_lines(const char *f) {
    ifstream file(f, ios::binary);
    if (!file) return 0;

    static const size_t BUFFER_SIZE = 64 * 1024; // 64KB Buffer
    vector<char> buffer(BUFFER_SIZE);
    unsigned long long lines = 0;
    char last_char = '\n';
    bool empty = true;

    while (file.read(buffer.data(), BUFFER_SIZE) || file.gcount() > 0) {
        empty = false;
        auto bytes = static_cast<size_t>(file.gcount());
        for (size_t i = 0; i < bytes; ++i) {
            if (buffer[i] == '\n') lines++;
            last_char = buffer[i];
        }
    }

    return (!empty && last_char != '\n') ? lines + 1 : lines;
}

/// Displays a specific range of lines from a document based on selected parameters.
///
/// Depending on the mode (first, last, delimit, invert), this function calculates
/// the boundaries and streams the relevant lines to the standard output.
///
/// # Arguments
/// * `s` - Simple mode toggle (disables headers and line numbers).
/// * `c` - Color mode toggle.
/// * `p` - Operation parameter ('f', 'l', 'd', 'i').
/// * `f` - Constant pointer to the file path string.
/// * `x`, `y` - Range boundaries for line selection.
///
/// # Returns
/// Integer status code (0 for success, 1 for file error).
int view_lines(bool s, bool c, char p, const char *f, unsigned long long x, unsigned long long y) {
    ifstream file(f);
    if (!file.is_open()) return 1;

    unsigned long long cnt = 0;
    string data;
    string blue = c ? BLUE : "", green = c ? GREEN : "", red = c ? RED : "", reset = c ? RESET : "";

    if (!s) cout << red << f << ":" << reset << "\n";

    unsigned long long total_lines = 0;
    if (p == parm_l || p == parm_i) {
        total_lines = count_lines(f);
    }

    unsigned long long start_at = 1;
    unsigned long long end_at = numeric_limits<unsigned long long>::max();

    if (p == parm_f) {
        end_at = x;
    } else if (p == parm_l) {
        start_at = (total_lines > x) ? (total_lines - x + 1) : 1;
        end_at = total_lines;
    } else if (p == parm_d) {
        if (y == null_arg) y = x;
        start_at = x;
        end_at = y;
    } else if (p == parm_i) {
        if (y == null_arg) y = x;
        start_at = (total_lines >= y) ? (total_lines - y + 1) : 1;
        end_at = (total_lines >= x) ? (total_lines - x + 1) : total_lines;
    }

    while (getline(file, data)) {
        cnt++;

        if (cnt >= start_at && cnt <= end_at) {
            if (!s) {
                cout << blue << setw(10) << cnt << green << ": " << reset << data << "\n";
            } else {
                cout << data << "\n";
            }
        }

        if (cnt >= end_at && (p == parm_f || p == parm_d)) break;
    }

    return 0;
}