#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/dirent.h>

#define PATH_LIMIT = 1024

void list_subdirs(const char *parent_path) {
  DIR *subdir = opendir(parent_path);
  if (!subdir) {
    fprintf(stderr, "Couldn't traverse: %s\n", parent_path);
    return;
  }

  struct dirent *entry;
  while ((entry = readdir(subdir)) != NULL) {
    if (entry->d_type == DT_DIR && entry->d_name[0] != '.')
      printf("  |--- %s/%s\n", parent_path, entry->d_name);
  }
  closedir(subdir);
}

int main() {
  const char *home = getenv("HOME");
  printf("%s", home);

  DIR *dir = opendir(home);

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR && entry->d_name[0] !=
'.') {
      printf("%s\n", entry->d_name);

      // Construct full path
      char sub_path[1024];
      snprintf(sub_path, sizeof(sub_path), "%s/%s", home, entry->d_name);
      list_subdirs(sub_path);
    }
    printf("%s\n", entry->d_name);
  }

  closedir(dir);
  return 0;
}
