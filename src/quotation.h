# ifndef _QUOTATION_H_
# define _QUOTATION_H_

struct quotation
{
  char *author;
  char *context;
  char *quote;
  unsigned long number;
};

struct quotation* init_quotation(char *author, char *context, char *quote,
                                unsigned long number);

void free_quotation(struct quotation *quotation);

struct quotation* get_unformatted(char *blockquote);

char* reformat_str(char *html);

char* wrap80columns(char *str);

struct quotation* reformat(struct quotation *quotation);

void print_quotation(struct quotation *quotation, int a, int c, int q, int n);

# endif
