#include <ctype.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    printf("A message is required.\n");
    return EXIT_FAILURE;
  }

  if (output_file == NULL) {
    printf("An output file is required.\n");
    return EXIT_FAILURE;
  }

  if (strcmp(message, "-") == 0) {
    FILE *output = fopen(output_file, "a");
    if (output == NULL) {
      printf("Failed to open the file.\n");
      return EXIT_FAILURE;
    }
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

  proper_size += strlen(message) + 1;

  char *final_message = malloc(proper_size);

  if (identifier != NULL) {
    strcpy(final_message, "[");
    strcat(final_message, identifier);
    strcat(final_message, "] ");
    strcat(final_message, message);
  } else {
    strcpy(final_message, message);
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
