#include <assert.h>

union U {
  float f;
  int i;
};

struct S {
  union U u;
  int x;
};

extern int getint(union U x);
extern union U mkfloat(float f);
extern int struct_with_union(struct S x);

int
main() {
  union U u;

  u.i = 42;
  assert(getint(u) == 42);

  u.i = -1;
  assert(getint(u) == -1);

  u = mkfloat(3.14f);
  assert(u.f == 3.14f);

  struct S s;
  s.u.i = 0;
  s.x = 99;
  assert(struct_with_union(s) == 99);
}
