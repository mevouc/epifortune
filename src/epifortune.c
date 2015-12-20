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
