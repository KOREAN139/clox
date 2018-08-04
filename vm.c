#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

vm_t vm;

static void reset_stack()
{
  vm.top = vm.stack;
}

void init_VM()
{
  reset_stack();
}

void free_VM()
{
}

void push(val_t value)
{
  *vm.top = value;
  vm.top++;
}

val_t pop()
{
  vm.top--;
  return *vm.top;
}

static interpret_result_t run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
  /*
   * XXX It'll be fun implementing one of these
   * direct threaded code
   * jump table
   * computed goto
   */
  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (val_t *ptr = vm.stack; ptr < vm.top; ptr++) {
      printf("[");
      print_value(*ptr);
      printf("]");
    }
    puts("");
    disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT: {
      val_t constant = READ_CONSTANT();
      push(constant);
      break;
    }
    case OP_RETURN:
      print_value(pop());
      puts("");
      return INTERPRET_OK;
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
}

interpret_result_t interpret(chunk_t *chunk)
{
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}
