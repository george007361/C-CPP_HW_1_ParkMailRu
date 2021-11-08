//  / <copyright>© George Illarionov 2021. All rights ignored</copyright>
#ifndef LIB_PARALLEL_PARSER_INCLUDE_PARSER_H_
#define LIB_PARALLEL_PARSER_INCLUDE_PARSER_H_

#define PART_MIN_LEN_MB 20

#include <stdlib.h>

#include "graphs.h"

int parse_text(const char *text, const size_t text_len, Graph **graphs,
               const size_t graphs_count);
#endif  // LIB_PARALLEL_PARSER_INCLUDE_PARSER_H_
