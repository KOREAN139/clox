#ifndef CLOX_COMPILER_H
#define CLOX_COMPILER_H

#include "vm.h"
#include "scanner.h"

typedef struct {
  token_t current;
  token_t previous;
  bool had_error;
  bool panic;
} parser_t;

bool compile(const char *, chunk_t *);

#endif /* CLOX_COMPILER_H */
