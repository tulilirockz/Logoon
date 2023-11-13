#include "alloc_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define HOSTNAME_SIZE 256
#define TIME_SIZE 64

char *create_hostname(void) {
  char hostname[HOSTNAME_SIZE];
  if ((gethostname(&hostname[0], HOSTNAME_SIZE)) == -1) {
    perror("Failed getting system hostname");
    return NULL;
  }
  char *ret = malloc(HOSTNAME_SIZE);
  if (ret == NULL) {
    perror(FAILURE_ALLOC_MEM);
    exit(EXIT_FAILURE);
  }
  strcpy(ret, hostname);
  return ret;
}

char *create_time(void) {
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char current_time[TIME_SIZE];
  strftime(current_time, sizeof(current_time), "%c", tm);
  char *ret = malloc(TIME_SIZE);
  if (ret == NULL) {
    perror(FAILURE_ALLOC_MEM);
    exit(EXIT_FAILURE);
  }
  strcpy(ret, current_time);
  return ret;
}

char *create_message(struct CLIMessage *message) {
  char *full_message = malloc(1); // null terminator
  char *temp_message_ptr;
  if (full_message == NULL) {
    return NULL;
  }
  full_message[0] = '\0';

  if (message->is_quiet != CLI_IS_QUIET) {
    temp_message_ptr = realloc(
        full_message, strlen(full_message) + strlen(message->date) +
                          strlen(message->hostname) + 2); // 2 for spaces
    if (temp_message_ptr == NULL) {
      free(full_message);
      return NULL;
    }
    full_message = temp_message_ptr;

    sprintf(full_message, "%s %s ", message->date, message->hostname);
  }

  if (message->identifier != NULL) {
    temp_message_ptr = realloc(
        full_message, strlen(full_message) + strlen(message->identifier) + 4);
    if (temp_message_ptr == NULL) {
      free(full_message);
      return NULL;
    }
    full_message = temp_message_ptr;

    strcat(full_message, "[");
    strcat(full_message, message->identifier);
    strcat(full_message, "]");
    strcat(full_message, " ");
  }

  if (strcmp(message->text, "-") != STRCMP_FOUND) {
    temp_message_ptr =
        realloc(full_message, strlen(full_message) + strlen(message->text));
    if (temp_message_ptr == NULL) {
      free(full_message);
      return NULL;
    }
    full_message = temp_message_ptr;
    strcat(full_message, message->text);
    return full_message;
  }

  char ch;
  size_t len = strlen(full_message);

  while ((ch = getchar()) != '\n' && ch != EOF) {
    temp_message_ptr = realloc(full_message,
                               strlen(full_message) + len +
                                   2); // new character + null terminator
    if (temp_message_ptr == NULL) {
      free(full_message);
      return NULL;
    }
    full_message = temp_message_ptr;
    full_message[len] = ch;
    len++;
  }

  if (full_message != NULL) {
    full_message[len] = '\0';
  }

  return full_message;
}
