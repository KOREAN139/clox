#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include "chunk.h"

void disassemble_chunk(chunk_t *, const char *);
int disassemble_instruction(chunk_t *, int);

#endif /* CLOX_DEBUG_H */
