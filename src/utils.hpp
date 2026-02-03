/**
 * @file utils.hpp
 * @brief "refn" - High-performance line reader utility.
 * @author Mauricio Ferrari
 * @date 2026-02-02
 */

#ifndef REF_UTILS_HPP
#define REF_UTILS_HPP

#include <unistd.h>

#define DEF_NUM 10

#define RED "\x1B[31m"
#define BLUE "\x1B[34m"
#define YELLOW "\x1B[33m"
#define GREEN "\x1B[32m"
#define RESET "\x1B[0m"

#define parm_f 'f'
#define parm_l 'l'
#define parm_d 'd'
#define parm_i 'i'
#define parm_c 'c'
#define parm_s 's'
#define parm_h 'h'
#define null_arg (-1)

#define IS_READABLE(f) (access(f, F_OK | R_OK) == 0)

unsigned long long count_lines(const char *f);

int view_lines(bool s, bool c, char p, const char *f, unsigned long long x, unsigned long long y);

#endif /** REF_UTILS_HPP **/
