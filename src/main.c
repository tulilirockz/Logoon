#include <ctype.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void print_help(void) { printf("HELP HERE"); }

int main(int argc, char **argv) {
  char *identifier = NULL;
  char *output_file = NULL;
  char *message = NULL;

  int option;
  while ((option = getopt(argc, argv, "f:i:m:h::")) != -1) {
    switch (option) {
    case 'h':
      print_help();
      return EXIT_SUCCESS;
    case 'f':
      output_file = optarg;
      break;
    case 'i':
      identifier = optarg;
      break;
    case 'm':
      message = optarg;
      break;
    case '?':
      if (optopt == 'f' || optopt == 'i' || optopt == 'm')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      return EXIT_FAILURE;
    default:
      abort();
    }
  }

  if (message == NULL) {
    perror("A message is required.\n");
    return EXIT_FAILURE;
  }

  if (output_file == NULL) {
    perror("An output file is required.\n");
    return EXIT_FAILURE;
  }

  const size_t HOSTNAME_SIZE = 256;
  char hostname[HOSTNAME_SIZE];
  if ((gethostname(&hostname[0], HOSTNAME_SIZE)) == -1) {
    perror("Failed getting system hostname");
    return EXIT_FAILURE;
  }

  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char current_time[64];
  strftime(current_time, sizeof(current_time), "%c", tm);

  if (strcmp(message, "-") == 0) {
    if (strcmp(output_file, "-") == 0) {
      printf("%s ", current_time);
      printf("%s ", hostname);
      printf("[%s] ", identifier);
      char a;
      while ((a = fgetc(stdin)) != EOF) {
        fputc(a, stdout);
      }
      return EXIT_SUCCESS;
    }

    FILE *output = fopen(output_file, "a");
    if (output == NULL) {
      printf("Failed to open the file.\n");
      return EXIT_FAILURE;
    }
    fprintf(output, "%s ", current_time);
    if (identifier != NULL) {
      fprintf(output, "[%s] ", identifier);
    }

    char a;
    while ((a = fgetc(stdin)) != EOF) {
      fputc(a, output);
    }
    fclose(output);
    return EXIT_SUCCESS;
  }

  int proper_size = 0;

  if (identifier != NULL)
    proper_size += strlen(identifier);

  proper_size += strlen(message) + sizeof(current_time) + HOSTNAME_SIZE + 1;

  char *final_message = malloc(proper_size);

  strcpy(final_message, current_time);
  strcat(final_message, " ");
  strcat(final_message, hostname);
  strcat(final_message, " ");

  if (identifier != NULL) {
    strcat(final_message, "[");
    strcat(final_message, identifier);
    strcat(final_message, "] ");
  }
  strcat(final_message, message);

  if (strcmp(output_file, "-") == 0) {
    printf("%s", final_message);
    free(final_message);
    return EXIT_SUCCESS;
  }

  FILE *output = fopen(output_file, "a");
  if (output == NULL) {
    printf("Failed to open the file.\n");
    return EXIT_FAILURE;
  }
  fprintf(output, "%s\n", final_message);
  fclose(output);
  free(final_message);

  return EXIT_SUCCESS;
}
