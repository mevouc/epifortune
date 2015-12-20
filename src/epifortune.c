# include "epifortune.h"
# include "html_operations.h"
# include "quotation.h"

int main(int argc, char **argv)
{
  if(argc > 1)
  {
    char *str = get_quote(strtoul(argv[1], NULL,10));
    struct quotation *q = get_unformatted(str);
    print_quotation(reformat(q));
    free(str);
    free_quotation(q);
  }
  else
  {
    char *str = get_random_quote();
    struct quotation *q = get_unformatted(str);
    print_quotation(reformat(q));
    free(str);
    free_quotation(q);
  }
  system("rm -f [0-9]*");
  return 0;
}

void print_help(void)
{
  printf("epifortune: Syntax: epifortune [-h] [-n number] [-c acqn]\n");
  printf("\n");
  printf("  -h | --help           Prompt this help.\n");
  printf("  -n | --number number  Print the number-th quote. If this option ");
  printf("is not set,\n");
  printf("                        a random quote will be printed.\n");
  printf("  -c | --conceal acqn   Hide one or more infos about the quotation:");
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
