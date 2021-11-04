#ifndef PRALLEL_PARSER_H
#define PRALLEL_PARSER_H

#include <stdlib.h>

#include "digraphs.h"

int parse_text(const char *text, const size_t text_len, Digraph **digraphs,
               const size_t digraphs_count);
#endif