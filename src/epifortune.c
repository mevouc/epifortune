# include "epifortune.h"
# include "html_operations.h"
# include "quotation.h"
# include <string.h>
# include <stdlib.h>

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
      if(i + 1 < argc)
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
    printf("epifortune: Invalid option '%s'\n", argv[shit]);
    print_help();
  }
  else if(help || (last && number))
    print_help();
  else
  {
    col = col ? col : 80;
    if(number)
      run_on_quote(get_quote(number), !a, !c, !q, !n, col);
    else if(last)
      run_on_quote(get_last_quote(), !a, !c, !q, !n, col);
    else
      run_on_quote(get_random_quote(), !a, !c, !q, !n, col);
  }
  return 0;
}

void run_on_quote(char *blockquote, int a, int c, int q, int n,
    unsigned long col)
{
  struct quotation *quotation = get_unformatted(blockquote);
  print_quotation(reformat(quotation, col), a, c, q, n, col);
  free(blockquote);
  free_quotation(quotation);
  system("rm -f [0-9]*");
  system("rm -f ./-[0-9]*");
}

void print_help(void)
{
  printf("Usage: epifortune [global options]\n");
  printf("\n");
  printf("Global options:\n");
  printf("  -h, --help           Prompt this help.\n");
  printf("  -n, --number number  Print the number-th quote. If this option is");
  printf(" not set, a\n");
  printf("                       random quote will be printed.\n");
  printf("  -l, --last           Print the last quote. You can't set this ");
  printf(" option and\n");
  printf("                       the --number one at the same time.");
  printf("  -c, --conceal acqn   Hide one or more infos about the ");
  printf("quotation:\n");
  printf("                         * a - Author\n");
  printf("                         * c - Context\n");
  printf("                         * q - Quote\n");
  printf("                         * n - Number\n");
  printf("  -w, --wrap column    Word-wraps the message at about column ");
  printf("columns, default\n");
  printf("                       is 80 columns. Low values may cause odd ");
  printf("display, use\n");
  printf("                       them moderatly.\n");
  printf("\n");
  printf("Examples:\n");
  printf("  epifortune\n");
  printf("  epifortune --help\n");
  printf("  epifortune -n 42 -c n\n");
  printf("  epifortune -c aqn\n");
  printf("  epifortune --last");
  printf("\n");
  printf("Provided by Meven 'mevouc' Courouble (http://vouc.me).\n");
  printf("Code (https://github.com/mevouc/epifortune) under Beerware ");
  printf("licence.\n");
}
