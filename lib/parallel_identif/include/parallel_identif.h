#ifndef PARALLEL_IDENT_H
#define PARALLEL_IDENT_H

#include "digraphs.h"

int indentification_parallel(const char *array, const size_t array_len, Digraph *digraph, const unsigned separates_count);
int indentification_parallel_all(const char *array,const size_t array_len, Digraph *digraphs_arr, const size_t digraphs_arr_len, const unsigned separates_count);

#endif // !PARALLEL_IDENT_H