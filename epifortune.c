# include "html_operations.h"

int main(int argc, char **argv)
{
  if(argc > 1)
  {
    printf("%s", reformat_soft(reformat(get_quote(strtoul(argv[1], NULL,10)))));
  }
  else
    printf("%s", reformat_soft(reformat(get_random_quote())));
  system("rm -f [0-9]*");
  return 0;
}

