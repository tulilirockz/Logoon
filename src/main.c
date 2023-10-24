#include <ctype.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct CLIMessage {
  char *identifier;
  char *output_path;
  char *hostname;
  char *date;
  char *text;
} CLIMessage;

char *get_current_hostname(void) {
  const size_t HOSTNAME_SIZE = 256;
  char hostname[HOSTNAME_SIZE];
  if ((gethostname(&hostname[0], HOSTNAME_SIZE)) == -1) {
    perror("Failed getting system hostname");
    return NULL;
  }
  char *ret = malloc(HOSTNAME_SIZE);
  strcpy(ret, hostname);
  return ret;
}

char *get_current_time(void) {
  const size_t TIME_SIZE = 64;
  time_t t = time(NULL);
  struct tm *tm = localtime(&t);
  char current_time[TIME_SIZE];
  strftime(current_time, sizeof(current_time), "%c", tm);
  char *ret = malloc(TIME_SIZE);
  strcpy(ret, current_time);
  return ret;
}

void print_help(void) { printf("HELP HERE"); }

int main(int argc, char **argv) {
  struct CLIMessage usermessage;

  int option;
  while ((option = getopt(argc, argv, "f:i:m:h::")) != -1) {
    switch (option) {
    case 'h':
      print_help();
      return EXIT_SUCCESS;
    case 'f':
      usermessage.output_path = optarg;
      break;
    case 'i':
      usermessage.identifier = optarg;
      break;
    case 'm':
      usermessage.text = optarg;
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

  if (usermessage.text == NULL) {
    perror("A message is required.\n");
    return EXIT_FAILURE;
  }

  if (usermessage.output_path == NULL) {
    perror("An output file is required.\n");
    return EXIT_FAILURE;
  }

  usermessage.date = get_current_time();
  usermessage.hostname = get_current_hostname();

  FILE *output;
  if (strcmp(usermessage.output_path, "-") == 0) {
    output = stdout;
  } else {
    output = fopen(usermessage.output_path, "a");
  }
  if (output == NULL) {
    printf("Failed to open the file.\n");
    return EXIT_FAILURE;
  }

  fprintf(output, "%s %s ", usermessage.date, usermessage.hostname);
  if (usermessage.identifier != NULL) {
    fprintf(output, "[%s] ", usermessage.identifier);
  }

  if (strcmp(usermessage.text, "-") == 0) {
    char a;
    while ((a = fgetc(stdin)) != EOF) {
      fputc(a, output);
    }
  } else {
    fprintf(output, "%s ", usermessage.text);
  }

  free(usermessage.date);
  free(usermessage.hostname);
  fclose(output);
  return EXIT_SUCCESS;
}
