# include "epifortune.h"
# include "html_operations.h"
# include "quotation.h"
# include <string.h>

int main(int argc, char **argv)
{
  int help = 0;
  unsigned long number = 0;
  int a = 0;
  int c = 0;
  int q = 0;
  int n = 0;
  int i = 1;
  int shit = 0;
  while(i < argc)
  {
    if(!strncmp("-h", argv[i], 2) || !strncmp("--help", argv[i], 6))
      help = 1;
    else if(!strncmp("-n", argv[i], 2) || !strncmp("--number", argv[i], 8))
    {
      if(i + 1 < argc)
        number = strtoul(argv[++i], NULL, 10);
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
  else if(help)
    print_help();
  else
  {
    if(number)
    {
      run_on_quote(get_quote(number), !a, !c, !q, !n);
    }
    else
    {
      run_on_quote(get_random_quote(), !a, !c, !q, !n);
    }
  }
  return 0;
}

void run_on_quote(char *blockquote, int a, int c, int q, int n)
{
  struct quotation *quotation = get_unformatted(blockquote);
  print_quotation(reformat(quotation), a, c, q, n);
  free(blockquote);
  free_quotation(quotation);
  system("rm -f [0-9]*");
}

void print_help(void)
{
  printf("epifortune: Syntax: epifortune [-h] [-n number] [-c acqn]\n");
  printf("\n");
  printf("  -h | --help           Prompt this help.\n");
  printf("  -n | --number number  Print the number-th quote. If this option ");
  printf("is not set,\n");
  printf("                        a random quote will be printed.\n");
  printf("  -c | --conceal acqn   Hide one or more infos about the ");
  printf("quotation:\n");
  printf("                          * a - Author\n");
  printf("                          * c - Context\n");
  printf("                          * q - Quote\n");
  printf("                          * n - Number\n");
  printf("\n");
  printf("Examples:\n");
  printf("  epifortune\n");
  printf("  epifortune --help\n");
  printf("  epifortune -n 42 -c n\n");
  printf("  epifortune -c aqn\n");
  printf("\n");
  printf("Provided by Meven 'mevouc' Courouble (http://vouc.me).\n");
  printf("Code (https://github.com/mevouc/epifortune) under Beerware ");
  printf("licence.\n");
}
