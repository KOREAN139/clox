#include "scanner.h"
#include "debug.h"

Scanner scanner;

/* Initialize scanner */
void initScanner(char *src)
{
  scanner.start = src;
  scanner.curr = src;
  scanner.line = 1;
}

/*
 * Returns 1 if scanner reached end of script
 *         0 otherwise
 */
int isAtEnd()
{
  return *scanner.curr == '\0';
}

/* Read one character from script */
char advance()
{
  return *scanner.curr++;
}

/*
 * Returns 1 if current character is what we expected
 *         0 otherwise
 */
int match(char expected)
{
  if (isAtEnd()) return 0;
  if (*scanner.curr != expected) return 0;

  scanner.curr++;
  return 1;
}

/* Make token from [scanner.start, scanner.curr) */
Token makeToken(tokenType type)
{
  Token newToken;
  newToken.type = type;
  newToken.start = scanner.start;
  newToken.len = (int)(scanner.curr - scanner.start);
  newToken.line = scanner.line;
  return newToken;
}

Token scanToken()
{
  char c = advance();
  switch (c) {
  case '(': 
    return makeToken(TOKEN_LEFT_PAREN); 
  case ')': 
    return makeToken(TOKEN_RIGHT_PAREN); 
  case '{': 
    return makeToken(TOKEN_LEFT_BRACE); 
  case '}': 
    return makeToken(TOKEN_RIGHT_BRACE); 
  case ',': 
    return makeToken(TOKEN_COMMA); 
  case '.': 
    return makeToken(TOKEN_DOT); 
  case '-': 
    return makeToken(TOKEN_MINUS); 
  case '+': 
    return makeToken(TOKEN_PLUS); 
  case ';': 
    return makeToken(TOKEN_SEMICOLON); 
  case '*': 
    return makeToken(TOKEN_STAR); 
  case '!': 
    return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  case '=': 
    return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL); 
  case '<': 
    return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
  case '>': 
    return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER); 
  default:
    error(scanner.line, "Unexpected character");
    return makeToken(TOKEN_ERROR);
  }
}

/* Scan tokens over script */
void scanTokens()
{
  Token curToken;
  while (!isAtEnd()) {
    scanner.start = scanner.curr;
    curToken = scanToken();
  }
}
