#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/dirent.h>

#define PATH_LIMIT = 1024

int main() {
  const char *home = getenv("HOME");
  printf("%s", home);

  DIR *dir = opendir(home);

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
      printf("%s\n", entry->d_name);

      // Construct full path
      char sub_path[1024];
      snprintf(sub_path, sizeof(sub_path), "%s/%s", home, entry->d_name);
    }
  }

  closedir(dir);
  return 0;
}
