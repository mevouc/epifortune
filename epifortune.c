# include "html_operations.h"

int main(int argc, char **argv)
{
  if(argc > 1)
  {
    char *str = reformat_soft(reformat(get_quote(strtoul(argv[1], NULL,10))));
    printf("%s", str);
    free(str);
  }
  else
  {
    char *str = reformat_soft(reformat(get_random_quote()));
    printf("%s", str);
    free(str);
  }
  system("rm -f [0-9]*");
  return 0;
}

