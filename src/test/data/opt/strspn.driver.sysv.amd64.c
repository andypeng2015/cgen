#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

extern size_t cgen_strspn(const char *s, const char *c);

static void
check_one(const char *s, const char *accept) {
  size_t got = cgen_strspn(s, accept);
  assert((long)got >= 0);
  size_t exp = strspn(s, accept);
  if (got != exp) {
    fprintf(
      stderr,
      "Mismatch!\n  s      = \"%s\"\n  accept = \"%s\"\n  got=%zu exp=%zu\n", s,
      accept, got, exp);
    assert(got == exp);
  }
}

static unsigned int
rng(unsigned int *state) {
  *state = (*state) * 1664525u + 1013904223u;
  return *state;
}

static void
random_string(char *out, size_t out_cap, unsigned int *st,
              const char *alphabet) {
  assert(out_cap > 0);
  size_t alpha_n = strlen(alphabet);
  size_t n = rng(st) % (out_cap - 1); // length in [0, out_cap-2]
  for (size_t i = 0; i < n; i++) {
    out[i] = alphabet[rng(st) % alpha_n];
  }
  out[n] = '\0';
}

int
main() {
  check_one("", "");
  check_one("", "abc");
  check_one("abc", "");
  check_one("abc", "a");
  check_one("aaab", "a");
  check_one("bbb", "a");
  check_one("abcde", "abc");
  check_one("abcde", "cba");
  check_one("aaaaa", "a");
  check_one("ababab", "ab");
  check_one("ababab", "ba");
  check_one(" \t\nxyz", " \t\n");
  check_one("xyz", " \t\n");
  check_one("012345", "0123456789");

  check_one("aaab", "aaaaa");
  check_one("abc", "aabbcc");

  {
    unsigned int st = 0xC0FFEEu;
    const char *alph_s = "abccddeeffgghhiijjkkllmmnnooppqqrrssttuvwxyz0123";
    const char *alph_a = "abcxyz0123";
    char s[128];
    char a[64];
    for (int i = 0; i < 50000; i++) {
      random_string(s, sizeof(s), &st, alph_s);
      random_string(a, sizeof(a), &st, alph_a);
      if ((rng(&st) % 10) == 0) {
        char c = alph_a[rng(&st) % strlen(alph_a)];
        a[0] = c;
        a[1] = '\0';
      }
      check_one(s, a);
    }
  }
}
