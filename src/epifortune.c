# include "html_operations.h"
# include "quotation.h"
# include <string.h>
# include <stdlib.h>

static void run_on_quote(char *blockquote, int a, int c, int q, int n,
                         unsigned long col, char *rm);
static void print_help(FILE * s);

int main(int argc, char **argv)
{
  int help = 0;
  int shit = 0;
  unsigned long number = 0;
  int a = 0;
  int c = 0;
  int q = 0;
  int n = 0;
  unsigned long col = 0;
  int i = 1;
  int last = 0;
  while(i < argc)
  {
    if(!strncmp("-h", argv[i], 2) || !strncmp("--help", argv[i], 6))
      help = 1;
    else if(!strncmp("-l", argv[i], 2) || !strncmp("--last", argv[i], 6))
      last = 1;
    else if(!strncmp("-n", argv[i], 2) || !strncmp("--number", argv[i], 8))
    {
      if(i + 1 < argc && strncmp("-", argv[i + 1], 1))
        number = strtoul(argv[++i], NULL, 10);
    }
    else if(!strncmp("-w", argv[i], 2) || !strncmp("--wrap", argv[i], 6))
    {
      if(i + 1 < argc)
        col = strtoul(argv[++i], NULL, 10);
    }
    else if(!strncmp("-c", argv[i], 2) || !strncmp("--conceal", argv[i], 9))
    {
      if(i + 1 < argc)
      {
        i++;
        for(size_t j = 0; j < strlen(argv[i]); j++)
        {
          switch(argv[i][j])
          {
            case 'a':
              a = 1;
              break;
            case 'c':
              c = 1;
              break;
            case 'q':
              q = 1;
              break;
            case 'n':
              n = 1;
              break;
            default:
              break;
          }
        }
      }
    }
    else
      shit = i;
    i++;
  }
  if(shit)
  {
    fprintf(stderr, "epifortune: Invalid option '%s'\n", argv[shit]);
    print_help(stderr);
    return 1;
  }
  else if (last && number)
  {
    print_help(stderr);
    return 1;
  }
  else if(help)
    print_help(stdout);
  else
  {
    col = col ? col : 80;
    char *blockquote = NULL;
    char *rm = calloc(24, sizeof(char));
    if(number)
    {
      blockquote = get_quote(number);
      sprintf(rm, "rm -f %lu", number);
    }
    else if(last)
    {
      blockquote = get_last_quote();
      sprintf(rm, "rm -f ./-1");
    }
    else
    {
      blockquote = get_random_quote();
      sprintf(rm, "rm -f ./0");
    }
    run_on_quote(blockquote, !a, !c, !q, !n, col, rm);
  }
  return 0;
}

static void run_on_quote(char *blockquote, int a, int c, int q, int n,
                         unsigned long col, char *rm)
{
  struct quotation *quotation = get_unformatted(blockquote);
  print_quotation(reformat(quotation, col), a, c, q, n, col);
  free(blockquote);
  free_quotation(quotation);
  system(rm);
  free(rm);
}

static void print_help(FILE * s)
{
  fprintf(s, "Usage: epifortune [global options]\n");
  fprintf(s, "\n");
  fprintf(s, "Global options:\n");
  fprintf(s, "  -h, --help           Prompt this help.\n");
  fprintf(s, "  -n, --number number  Print the number-th quote. If this");
  fprintf(s, " option is not set, a\n");
  fprintf(s, "                       random quote will be printed.\n");
  fprintf(s, "  -l, --last           Print the last quote. You can't set");
  fprintf(s, " this option and\n");
  fprintf(s, "                       the --number one at the same time.\n");
  fprintf(s, "  -c, --conceal acqn   Hide one or more infos about the ");
  fprintf(s, "quotation:\n");
  fprintf(s, "                         * a - Author\n");
  fprintf(s, "                         * c - Context\n");
  fprintf(s, "                         * q - Quote\n");
  fprintf(s, "                         * n - Number\n");
  fprintf(s, "  -w, --wrap column    Word-wraps the message at about column ");
  fprintf(s, "columns, default\n");
  fprintf(s, "                       is 80 columns. Low values may cause odd ");
  fprintf(s, "display, use\n");
  fprintf(s, "                       them moderatly.\n");
  fprintf(s, "\n");
  fprintf(s, "Examples:\n");
  fprintf(s, "  epifortune\n");
  fprintf(s, "  epifortune --help\n");
  fprintf(s, "  epifortune -n 42 -c n\n");
  fprintf(s, "  epifortune -c aqn\n");
  fprintf(s, "  epifortune --last");
  fprintf(s, "\n");
  fprintf(s, "Provided by Meven 'mevouc' Courouble (http://vouc.me).\n");
  fprintf(s, "Code (https://github.com/mevouc/epifortune) under Beerware ");
  fprintf(s, "licence.\n");
}
