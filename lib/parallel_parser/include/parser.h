#ifndef PRALLEL_PARSER_H
#define PRALLEL_PARSER_H

#include <stdlib.h>

#include "graphs.h"

int parse_text(const char *text, const size_t text_len, Graph **graphs,
               const size_t graphs_count);
#endif