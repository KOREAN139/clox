#include <stdio.h>
#include <stdbool.h>

#include "debug.h"

void report(int line, char *where, char *message)
{
  fprintf(stderr, "[line %d] Error %s: %s\n", line, where, message);
  hadError = 1;
}

void error(int line, char *message)
{
  report(line, "", message);
};
