#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <onigmo.h>

void die(const char *msg)
{
  fputs(msg, stderr);
  abort();
}

#define ustrlen(x) strlen((char*)x)
#define countof(x) (sizeof(x) / sizeof(x[0]))

int main(int argc, const char **argv)
{
  assert(onig_init() == 0);

  int err;
  OnigUChar negLookahead[] = "(?!\\A)hell";
  UChar text[] = "hello hellbeast";
  regex_t *oni_re = NULL;
  OnigErrorInfo errinfo;

  err = onig_new(&oni_re, negLookahead, (negLookahead + ustrlen(negLookahead)), ONIG_OPTION_NONE, ONIG_ENCODING_UTF_8, ONIG_SYNTAX_DEFAULT, &errinfo);
  assert(err == ONIG_NORMAL);
  puts("Regex built!");

  OnigPosition pos;
  UChar *start = text;
  UChar *end = start + ustrlen(start);
  printf("Start is %s, total length %d\n", (char*)text, (int)(end - start));
  printf("Now searching with regular Onigmo options\n");
  while ((pos = onig_search(oni_re, text, end, start, end, NULL, 0)) != ONIG_MISMATCH) {
    start += ((ptrdiff_t)pos + 1);
    printf("Found it at %td\n", pos);
  }

  start = text;
  printf("Now searching with NOTBOS Onigmo options\n");
  while ((pos = onig_search(oni_re, text, end, start, end, NULL, ONIG_OPTION_NOTBOS)) != ONIG_MISMATCH) {
    start += ((ptrdiff_t)pos + 1);
    printf("Found it at %td\n", pos);
  }

  onig_free(oni_re);
  return 0;
}
