/**
 * @file main.cpp
 * @brief "refn" - High-performance line reader utility.
 * @author Mauricio Ferrari
 * @date 2026-02-02
 */

#include <cstring>
#include <iostream>
#include <getopt.h>

#include "utils.hpp"

#define VERSION "4.0"

using namespace std;

/// Displays application usage and available options.
void help(const char *x) {
    cout << x << " v" << VERSION << " - A high-performance utility to read specific lines." << endl;
    cout << "\nUsage: " << x << " [OPTIONS] [FILE]..." << endl;

    cout << "\nOperations:" << endl;
    cout << "  -f, --first=[N]    Display the first N lines (default: 10)." << endl;
    cout << "  -l, --last=[N]     Display the last N lines (default: 10)." << endl;
    cout << "  -d, --delimit=R    Display a specific range R (e.g., 10-20 or 50)." << endl;
    cout << "  -i, --invert=R     Display a specific range R in inverted order." << endl;

    cout << "\nOptions:" << endl;
    cout << "  -c, --color        Enable syntax-like coloring for line numbers." << endl;
    cout << "  -s, --simple       Plain text output (no line numbers or headers)." << endl;
    cout << "  -h, --help         Display this help information and exit." << endl;

    cout << "\nExample:" << endl;
    cout << "  " << x << " -d 10-20 log.txt" << endl;
    cout << "  " << x << " -l5 access.log" << endl;

    exit(0);
}

/// Stores the global state and configuration for the current execution.
struct Context {
    char mode = parm_f;
    unsigned long long arg1 = DEF_NUM;
    unsigned long long arg2 = null_arg;
    bool use_color = false;
    bool simple_view = false;
};

/// Safely converts string to unsigned long long without throwing exceptions.
unsigned long long safe_stoull(const char* str, unsigned long long default_value) {
    if (!str || *str == '\0') return default_value;
    char* end_ptr;
    unsigned long long val = strtoull(str, &end_ptr, 10);
    return (end_ptr == str) ? default_value : val;
}

int main(int argc, char **argv) {
    const char* ref_file = strrchr(argv[0], '/');
    ref_file = (ref_file) ? ref_file + 1 : argv[0];
    Context ctx;

    const struct option long_opts[] = {
            {"first",   optional_argument, nullptr, parm_f},
            {"last",    optional_argument, nullptr, parm_l},
            {"delimit", required_argument, nullptr, parm_d},
            {"invert",  required_argument, nullptr, parm_i},
            {"color",   no_argument,       nullptr, parm_c},
            {"simple",  no_argument,       nullptr, parm_s},
            {"help",    no_argument,       nullptr, parm_h},
            {nullptr,   0,                 nullptr, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "f::l::d:i:csh", long_opts, nullptr)) != -1) {
        switch (opt) {
            case parm_f:
            case parm_l:
                ctx.mode = (char)opt;
                if (optarg) {
                    ctx.arg1 = safe_stoull(optarg, DEF_NUM);
                }
                else if (optind < argc && isdigit(argv[optind][0])) {
                    ctx.arg1 = safe_stoull(argv[optind++], DEF_NUM);
                }
                else {
                    ctx.arg1 = DEF_NUM;
                }
                break;
            case parm_d:
            case parm_i:
                ctx.mode = (char)opt;
                if (optarg) {
                    string s(optarg);
                    size_t pos = s.find('-');
                    if (pos != string::npos) {
                        ctx.arg1 = safe_stoull(s.substr(0, pos).c_str(), 1);
                        ctx.arg2 = safe_stoull(s.substr(pos + 1).c_str(), 1);
                    } else {
                        ctx.arg1 = safe_stoull(optarg, 1);
                        ctx.arg2 = ctx.arg1;
                    }
                }
                break;
            case parm_c: ctx.use_color = true; break;
            case parm_s: ctx.simple_view = true; break;
            case parm_h: help((char*)ref_file); break;
            default: return 1;
        }
    }

    if (optind >= argc) {
        help((char*)ref_file);
    }

    for (int i = optind; i < argc; i++) {
        char* current_file = argv[i];

        if (!IS_READABLE(current_file)) {
            cerr << YELLOW << ref_file << ": File '" << current_file << "' not found." << RESET << endl;
            continue;
        }

        char* path = realpath(current_file, nullptr);
        view_lines(ctx.simple_view, ctx.use_color, ctx.mode, path, ctx.arg1, ctx.arg2);
        free(path);
    }

    return 0;
}
