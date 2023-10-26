#pragma once

#define FAILURE_ALLOC_MEM "Failure to allocate memory"
#define CLI_IS_QUIET 1
#define STRCMP_FOUND 0

typedef struct CLIMessage {
  char *identifier;
  char *output_path;
  char *hostname;
  char *date;
  char *text;
  int is_quiet;
} CLIMessage;

char *create_hostname(void);
char *create_time(void);
char *create_message(struct CLIMessage *message);
