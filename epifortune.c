# include "html_operations.h"

int main(int argc, char **argv)
{
  if(argc > 1)
  {
    FILE* file = fopen(argv[1], "r");
    file = find_first_blockquote(file);
    printf("%s\n", reformat_soft(reformat(get_blockquote(file))));
  }
  return 0;
}

