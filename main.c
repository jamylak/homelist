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
    // current bytes used in the queue so far
    size_t currentBytes;
} Queue;

void process_dir_into_queue(Queue *queue, const char *dirpath) {
  struct dirent *entry;

  DIR *dir = opendir(dirpath);

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR && entry->d_name[0] != '.') {
      printf("%s\n", entry->d_name);

      // Construct full path from parent dir
      // +1 for the slash and +1 for null terminator
      uint16_t pathlen = strlen(dirpath) + 1 + entry->d_namlen + 1;

      // Calculate the size of this QueuePathEntry (unaligned)
      size_t entry_size = sizeof(uint16_t) + pathlen;

      // Round up to the next 64-byte boundary
      size_t aligned_size = (entry_size + 63) & ~63;

      // Get the current entry as a pointer
      QueuePathEntry *current_entry = (QueuePathEntry *)((char *)queue->entries + queue->currentBytes);

      // Set the entry’s fields
      current_entry->len = pathlen;
      snprintf(current_entry->data, pathlen, "%s/%s", dirpath, entry->d_name);

      // Increment currentBytes by the aligned size
      queue->currentBytes += aligned_size;
    }
  }
  closedir(dir);
}

int main() {
  QueuePathEntry *all_entries = aligned_alloc(64, ARENA_SIZE);
  Queue queue = {.entries = all_entries, .currentBytes=0};
  
  const char *home = getenv("HOME");
  printf("%s\n", home);
  process_dir_into_queue(&queue, home);

  // Test get something from queue
  printf("Test queue item: %s\n", queue.entries[0].data);

  QueuePathEntry *second_entry = (QueuePathEntry *)((char *)queue.entries + 
    ((sizeof(uint16_t) + queue.entries->len + 63) & ~63));

  printf("Test queue item 2: %s\n", second_entry->data);
  return 0;
}
