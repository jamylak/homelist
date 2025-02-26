#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/dirent.h>

int main() {
  const char *home = getenv("HOME");
  printf("%s", home);

  DIR *dir = opendir(home);
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR) {
      printf("%s\n", entry->d_name);
    }
    printf("%s\n", entry->d_name);
  }

  closedir(dir);
  return 0;
}
