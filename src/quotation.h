# ifndef _QUOTATION_H_
# define _QUOTATION_H_

# include <stdlib.h>

struct quotation
{
  char *author;
  char *context;
  char *quote;
  long number;
};

struct quotation* init_quotation(char *author, char *context, char *quote,
                                long number);

void free_quotation(struct quotation *quotation);

struct quotation* get_unformatted(char *blockquote);

char* reformat_soft(char *old);

char* reformat(char *html);

# endif
