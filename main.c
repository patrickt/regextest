#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <onigmo.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

void die(const char *msg)
{
  fputs(msg, stderr);
  abort();
}

#define ustrlen(x) strlen((char*)x)
#define countof(x) (sizeof(x) / sizeof(x[0]))

struct result {

};

int main(int argc, const char **argv)
{
  assert(onig_init() == 0);

  int err;
  OnigUChar negLookahead[] = "(?!\\A)hell";
  UChar text[] = "hello dang hell \nhell beast \nhellish nation";
  regex_t *oni_re = NULL;
  OnigErrorInfo errinfo;

  err = onig_new(&oni_re, negLookahead, (negLookahead + ustrlen(negLookahead)), ONIG_OPTION_NONE, ONIG_ENCODING_UTF_8, ONIG_SYNTAX_DEFAULT, &errinfo);
  assert(err == ONIG_NORMAL);
  puts("**Onigmo**");

  OnigPosition pos;
  UChar *start = text;
  UChar *end = start + ustrlen(start);
  printf("Start is %s, total length %d\n", (char*)text, (int)(end - start));
  printf("Now searching with regular Onigmo options\n");
  while ((pos = onig_search(oni_re, text, end, start, end, NULL, 0)) != ONIG_MISMATCH) {
    start = text + ((ptrdiff_t)pos + 4);
    printf("Found it at %td\n", pos);
  }

  start = text;
  printf("Now searching with NOTBOS Onigmo options\n");
  while ((pos = onig_search(oni_re, text, end, start, end, NULL, ONIG_OPTION_NOTBOS)) != ONIG_MISMATCH) {
    start = text + ((ptrdiff_t)pos + 4);
    printf("Found it at %td\n", pos);
  }

  onig_free(oni_re);

  puts("**PCRE2**");
  PCRE2_SPTR pcreNegLookahead = (PCRE2_SPTR)"(?!\\A)hell";
  PCRE2_SIZE erroffset;
  err = 0;

  pcre2_code *pc_re = pcre2_compile(pcreNegLookahead, ustrlen(pcreNegLookahead), 0, &err, &erroffset, NULL);
  assert(pc_re != NULL);

  return 0;
}
