#ifndef PARALLEL_IDENT_H
#define PARALLEL_IDENT_H

#include "digraphs.h"

int parse_text(const char *text, const size_t text_len, Digraph **digraphs,
               const size_t digraphs_count);
#endif  // !PARALLEL_IDENT_H