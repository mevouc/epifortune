# ifndef _HTML_OPERATIONS_H_
# define _HTML_OPERATIONS_H_

# include <stdio.h>

FILE* find_first_blockquote(FILE* file);

char* get_blockquote(FILE *file);

char* get_random_quote(void);

char* get_quote(unsigned long nb);

void print_file(FILE *file);

# endif
