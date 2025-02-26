#include <stdio.h>
#include <stdlib.h>

int main() {
  const char *home = getenv("HOME");
  printf("%s", home);
}
