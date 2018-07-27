#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "debug.h"

void run(char *source)
{
  
};

void runPrompt()
{
  char line[1<<10];
  for (;;) {
    printf("> ");
    fgets(line, 1<<10, stdin);
    run(line);
    hadError = 0;
  }
};

void runScript(char *path)
{
  int fsz; 
  char *src;
  FILE *fp = fopen(path, "rb");

  if (!fp) {
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    fclose(fp);
    return;
  }

  /* Get size of given script */
  fseek(fp, 0L, SEEK_END);
  fsz = ftell(fp);
  
  /* Put file pointer to very first of script */
  rewind(fp);

  if(!(src = malloc(fsz + 1))) {
    fprintf(stderr, "Could not allocate buffer\n");
    fclose(fp);
    return;
  }

  /* Read script */
  if (fread(src, 1, fsz, fp) < fsz) {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    fclose(fp);
    return;
  }

  /* 
   * Mark EOF as '\0' 
   * Note that EOF != '\0', i marked like this
   * just for my own convenience
   */
  src[fsz] = '\0';

  /* Do interpret */
  run(src);

  fclose(fp);
  free(src);

  return;
};

int main(int argc, char **argv)
{
  if (argc > 2) 
    fprintf(stderr, "Usage: clox [script]\n");
  else 
    argc == 1 ? runPrompt() : runScript(argv[1]);
  return 0;
}
