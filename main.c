// BFS Search over eg. Home Directory
// for all nested git repos (with max nested limi)
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/dirent.h>

typedef struct {
  uint16_t len;
  char data[];
} __attribute__((aligned(64))) QueuePathEntry;

void process_dir_into_queue(const char *dirpath) {
  struct dirent *entry;

  DIR *dir = opendir(dirpath);

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
      printf("%s\n", entry->d_name);

      // Construct full path
      char sub_path[1024];
      snprintf(sub_path, sizeof(sub_path), "%s/%s", dirpath, entry->d_name);
    }
  }
  closedir(dir);
}

int main() {
  const char *home = getenv("HOME");
  printf("%s", home);
  process_dir_into_queue(home);
  return 0;
}
