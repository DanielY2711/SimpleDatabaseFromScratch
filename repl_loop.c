#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#ifdef _WIN32
typedef ptrdiff_t ssize_t;
#endif

#define INITIAL_BUFFER_SIZE 1024

typedef struct {
  char* buffer;
  size_t buffer_length;
  ssize_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer() {
  InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
  if (!input_buffer) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  input_buffer->buffer = malloc(INITIAL_BUFFER_SIZE);
  if (!input_buffer->buffer) {
    fprintf(stderr, "Memory allocation failed\n");
    free(input_buffer);
    exit(EXIT_FAILURE);
  }

  input_buffer->buffer_length = INITIAL_BUFFER_SIZE;
  input_buffer->input_length = 0;

  return input_buffer;
}

void print_prompt() { printf("db > "); }


void read_input(InputBuffer* input_buffer) {
  if (fgets(input_buffer->buffer, (int)input_buffer->buffer_length, stdin) == NULL) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }

  ssize_t bytes_read = (ssize_t)strlen(input_buffer->buffer);
  if (bytes_read > 0 && input_buffer->buffer[bytes_read - 1] == '\n') {
    input_buffer->buffer[bytes_read - 1] = '\0';
    input_buffer->input_length = bytes_read - 1;
  } else {
    input_buffer->input_length = bytes_read;
  }
}

void close_input_buffer(InputBuffer* input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

int main() {
  InputBuffer* input_buffer = new_input_buffer();
  while (true) {
    print_prompt();
    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    } else {
      printf("Unrecognized command '%s'.\n", input_buffer->buffer);
    }
  }
}
