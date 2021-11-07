//  / <copyright>© George Illarionov 2021. All rights ignored</copyright>
#ifndef C_CPP_HW_2_PARKMAILRU_LIB_PARALLEL_PARSER_INCLUDE_PARSER_H_
#define C_CPP_HW_2_PARKMAILRU_LIB_PARALLEL_PARSER_INCLUDE_PARSER_H_

#include <stdlib.h>

#include "graphs.h"

int parse_text(const char *text, const size_t text_len, Graph **graphs,
               const size_t graphs_count);
#endif  // C_CPP_HW_2_PARKMAILRU_LIB_PARALLEL_PARSER_INCLUDE_PARSER_H_
