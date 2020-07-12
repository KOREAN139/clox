#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

static parser_t parser;

chunk_t *compiling_chunk;

static chunk_t *current_chunk()
{
  return compiling_chunk;
}

static void error_at(token_t *token, const char *message)
{
  if (parser.panic)
    return;

  parser.panic = true;
  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {
    // nothin
  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  parser.had_error = true;
}

static void error(const char *message)
{
  error_at(&parser.previous, message);
}

static void error_at_current(const char *message)
{
  error_at(&parser.current, message);
}

static void advance()
{
  parser.previous = parser.current;

  for (;;) {
    parser.current = scan_token();
    if (parser.current.type != TOKEN_ERROR)
      break;

    error_at_current(parser.current.start);
  }
}

static void consume(token_type_t type, const char *message)
{
  if (parser.current.type == type) {
    advance();
    return;
  }

  error_at_current(message);
}

static void emit_byte(uint8_t byte)
{
  write_chunk(current_chunk(), byte, parser.previous.line);
}

static void emit_bytes(int count, ...)
{
  va_list byte_list;
  va_start(byte_list, count);
  for (int i = 0; i < count; i++) {
    uint8_t byte = (uint8_t) va_arg(byte_list, int);
    emit_byte(byte);
  }
  va_end(byte_list);
}

static void emit_return()
{
  emit_byte(OP_RETURN);
}

static uint8_t make_constant(val_t value)
{
  int constant = add_constant(current_chunk(), value);
  if (constant > UINT8_MAX) {
    error("Too many constants in one chunk.");
    return 0;
  }

  return (uint8_t) constant;
}

static void emit_constant(val_t value)
{ 
  emit_bytes(2, OP_CONSTANT, make_constant(value));
}

static void end_compiler()
{
  emit_return();
}

static void number()
{
  double value = strtod(parser.previous.start, NULL);
  emit_constant(value);
}

static void expression()
{
}

bool compile(const char *source, chunk_t *chunk)
{
  init_scanner(source);

  compiling_chunk = chunk;
  parser.had_error = false;
  parser.panic = false;
  advance();
  /** expression(); */
  consume(TOKEN_EOF, "Expect end of expression.");
  end_compiler();
  return !parser.had_error;
}
