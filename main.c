// BFS Search over eg. Home Directory
// for all nested git repos (with max nested limi)
// #include <cstdint>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/dirent.h>

#define ARENA_SIZE 1000000 * 128
#define MAX_NESTING_LEVEL 3

#define QUEUE_PATH_ENTRY_BASE_SIZE sizeof(uint16_t) * 2
#define QUEUE_PATH_ENTRY_ALIGNMENT 64

typedef struct {
    uint16_t lenBytes;
    // how nested the dir is eg. a->b->c
    uint16_t nesting_level;
    bool is_git_dir;
    char data[];
} __attribute__((aligned(QUEUE_PATH_ENTRY_ALIGNMENT))) QueuePathEntry;


// Update QUEUE_PATH_ENTRY_BASE_SIZE if editing members 
typedef struct {
    QueuePathEntry *entries;
    // current bytes used in the queue so far
    size_t currentBytes;
    // amount of bytes process in the queue so far
    // eg. we've already completed those queue items
    size_t processedBytes;
} Queue;

void process_dir_into_queue(Queue *queue, const char *dirpath, uint16_t nesting_level) {
  struct dirent *entry;
  bool is_git_dir = false;

  DIR *dir = opendir(dirpath);

  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".git") == 0) {
      is_git_dir = true;
    }
    if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0) {

      // Construct full path from parent dir
      // +1 for the slash and +1 for null terminator
      uint16_t pathlen = strlen(dirpath) + 1 + entry->d_namlen + 1;

      // Calculate the size of this QueuePathEntry (unaligned)
      size_t entry_size = QUEUE_PATH_ENTRY_BASE_SIZE + pathlen;

      // Round up to the next QUEUE_PATH_ENTRY_ALIGNMENT-byte boundary
      // eg. for 64 byte alignment
      // size_t aligned_size = (entry_size + 63) & ~63;
      size_t aligned_size = (entry_size + (QUEUE_PATH_ENTRY_ALIGNMENT - 1)) & ~(QUEUE_PATH_ENTRY_ALIGNMENT - 1);

      // Get the current entry as a pointer
      QueuePathEntry *current_entry = (QueuePathEntry *)((char *)queue->entries + queue->currentBytes);

      // Set the entry’s fields
      // I guess we are not saving the string size itself
      // could do so if needed... but since null terminated maybe
      // doesn't matter
      current_entry->lenBytes = aligned_size;
      current_entry->nesting_level = nesting_level + 1;
      current_entry->is_git_dir = is_git_dir;
      snprintf(current_entry->data, pathlen, "%s/%s", dirpath, entry->d_name);
      // printf("%s\n", current_entry->data);

      // Increment currentBytes by the aligned size
      queue->currentBytes += aligned_size;

    }
    if (is_git_dir) {
      // Now that we found a git dir, we should not add more folders
      // to the queue within this dir
      break;
    }
  }
  closedir(dir);
}

void process_queue_item(Queue *queue) {
  assert(queue->processedBytes < queue->currentBytes);
  QueuePathEntry *current_entry = (QueuePathEntry *)((char *)queue->entries + queue->processedBytes);
  if (current_entry->is_git_dir) {
    // Print without .git at the end
    printf("%.*s\n", (int)(strlen(current_entry->data) - 4), current_entry->data);
  }

  queue->processedBytes += current_entry->lenBytes;
  if (current_entry->nesting_level < MAX_NESTING_LEVEL)
    process_dir_into_queue(queue, current_entry->data, current_entry->nesting_level);
}

void process_queue(Queue *queue) {
  while (queue->processedBytes < queue->currentBytes) {
    process_queue_item(queue);
  }
}

int main() {
  QueuePathEntry *all_entries = aligned_alloc(QUEUE_PATH_ENTRY_ALIGNMENT, ARENA_SIZE);
  Queue queue = {.entries = all_entries, .processedBytes=0, .currentBytes=0};
  
  const char *home = getenv("HOME");
  printf("%s\n", home);
  process_dir_into_queue(&queue, home, 0);
  process_queue(&queue);

  return 0;
}
