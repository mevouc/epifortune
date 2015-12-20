# ifndef _QUOTATION_H_
# define _QUOTATION_H_

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

char* reformat(char *html);

char* reformat_soft(char *old);

void print_quotation(struct quotation *quotation);

# endif
