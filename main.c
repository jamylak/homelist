// BFS Search over eg. Home Directory
// for all nested git repos (with max nested limi)
// #include <cstdint>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/dirent.h>

#define ARENA_SIZE 1000000 * 128

typedef struct {
    uint16_t len;
    char data[];
} __attribute__((aligned(64))) QueuePathEntry;

typedef struct {
    QueuePathEntry *entries;
    size_t current;
} Queue;

void process_dir_into_queue(const char *dirpath) {
  struct dirent *entry;

  DIR *dir = opendir(dirpath);

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
      printf("%s\n", entry->d_name);

      // Construct full path
      uint16_t pathlen = strlen(dirpath) + 1 + entry->d_namlen;
      // char sub_path[pathlen];
      // snprintf(sub_path, sizeof(sub_path), "%s/%s", dirpath, entry->d_name);
    }
  }
  closedir(dir);
}

int main() {
  // Just guessing average dir size as 128
  QueuePathEntry *all_entries = malloc(ARENA_SIZE);
  Queue queue = {.entries = all_entries, .current=0};
  
  const char *home = getenv("HOME");
  printf("%s", home);
  process_dir_into_queue(home);
  return 0;
}
