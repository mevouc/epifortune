# include <stdio.h>
# include <stdlib.h>

FILE* find_first_blockquote(FILE* file);

char* get_blockquote(FILE *file);

char* get_random_quote(void);

char* get_quote(unsigned long nb);

char* reformat_soft(char *old);

char* reformat(char *html);

void print_file(FILE *file);
