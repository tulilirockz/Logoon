#include "alloc_data.h"
#include <ctype.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help(void) {
  printf("Usage: logoon [OPTION]... -m [MESSAGE] -f [FILE]\n");
  printf("When FILE is -, write to standard output.\n"
         "When MESSAGE is -, read standard input.\n");
  printf("\n-f\tfile that will be written to");
  printf("\n-i\tidentifier for program that will log");
  printf("\n-q\tquieter logging, no date or hostname");
  printf("\n-m\tmessage that will be written to FILE");
  printf("\n-h\tshow this help\n");
}

int main(int argc, char **argv) {
  struct CLIMessage *usermessage =
      (struct CLIMessage *)malloc(sizeof(struct CLIMessage));

  char *output_path = getenv("LOGOON_OUTPUT");
  if (output_path != NULL) {
    usermessage->output_path = output_path;
  }
  char *identifier = getenv("LOGOON_IDENTIFIER");
  if (output_path != NULL) {
    usermessage->identifier = identifier;
  }

  int option;
  while ((option = getopt(argc, argv, "f:i:m:q:h::")) != -1) {
    switch (option) {
    case 'h':
      print_help();
      return EXIT_SUCCESS;
    case 'f':
      usermessage->output_path = optarg;
      break;
    case 'i':
      usermessage->identifier = optarg;
      break;
    case 'm':
      usermessage->text = optarg;
      break;
    case 'q':
      usermessage->is_quiet = CLI_IS_QUIET;
      break;
    case '?':
      if (optopt == 'f' || optopt == 'i' || optopt == 'm')
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      else if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      free(usermessage);
      return EXIT_FAILURE;
    default:
      free(usermessage);
      abort();
    }
  }

  if (usermessage->text == NULL) {
    perror("A message is required.\n");

    free(usermessage);
    return EXIT_FAILURE;
  }

  if (usermessage->output_path == NULL) {
    perror("An output file is required.\n");

    free(usermessage);
    return EXIT_FAILURE;
  }

  if (usermessage->is_quiet != CLI_IS_QUIET) {
    usermessage->date = create_time();
    usermessage->hostname = create_hostname();
  }

  char *cool_message = create_message(usermessage);

  if (cool_message == NULL) {
    perror(FAILURE_ALLOC_MEM);
    if (usermessage->is_quiet != CLI_IS_QUIET) {
      free(usermessage->date);
      free(usermessage->hostname);
    }

    free(usermessage);
    return 1;
  }

  FILE *output;
  if (strcmp(usermessage->output_path, "-") == 0) {
    output = stdout;
  } else {
    output = fopen(usermessage->output_path, "a");
  }
  if (output == NULL) {
    printf("Failed to open the file.\n");
    return EXIT_FAILURE;
  }

  fprintf(output, "%s\n", cool_message);

  fclose(output);
  free(cool_message);
  if (usermessage->is_quiet != CLI_IS_QUIET) {
    free(usermessage->date);
    free(usermessage->hostname);
  }
  free(usermessage);
  return EXIT_SUCCESS;
}
