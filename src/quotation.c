# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "quotation.h"


struct quotation* init_quotation(char *author, char *context, char *quote,
                                unsigned long number)
{
  struct quotation *quotation = calloc(1, sizeof(struct quotation));
  quotation->author = author;
  quotation->context = context;
  quotation->quote = quote;
  quotation->number = number;
  return quotation;
}

void free_quotation(struct quotation *quotation)
{
  if(quotation)
  {
    free(quotation->author);
    free(quotation->context);
    free(quotation->quote);
    free(quotation);
  }
}

struct quotation* get_unformatted(char *blockquote)
{
  size_t i = 0;
  size_t len = strlen(blockquote);
  /* Ignore first line */
  for( ; i < len && blockquote[i] != '\n'; i++);
  i++;
  /* Goto the begin of author */
  for( ; i < len && blockquote[i] == ' '; i++);
  if(!strncmp("<strong>", blockquote + i, 8))
    i += 8;
  size_t index_author = i;
  size_t size_author = 0;
  while(!size_author)
  {
    for( ; i < len && blockquote[i] != '<'; i++);
    if(!strncmp("</strong>", blockquote + i, 9))
    {
      size_author = i - index_author;
      i += 8; /* will be 9 with the following i++ */
    }
    i++;
  }
  char *author = calloc(size_author + 1, sizeof(char));
  for(size_t j = 0; j < size_author; j++)
    author[j] = blockquote[index_author++];
  /* Until begin of next line */
  for( ; i < len && blockquote[i] != '\n'; i++);
  i++;
  /* Goto the begin of context */
  for( ; i < len && blockquote[i] == ' '; i++);
  size_t index_context = 0;
  size_t size_context = 0;
  if(blockquote[i] == '(') /* There is a context */
  {
    if(!strncmp("(<em>", blockquote + i, 5))
      i += 5;
    index_context = i;
    while(!size_context)
    {
      for( ; i < len && blockquote[i] != '<'; i++);
      if(!strncmp("</em>)", blockquote + i, 6))
      {
        size_context = i - index_context;
        i += 5;
      }
      i++;
    }
  }
  char *context = NULL;
  if(size_context)
    context = calloc(size_context + 1, sizeof(char));
  for(size_t j = 0; j < size_context; j++)
    context[j] = blockquote[index_context++];
  /* Until begin of next line */
  for( ; i < len && blockquote[i] != '<'; i++);
  if(!strncmp("</small></p>", blockquote + i, 12))
    i += 12;
  for( ; i < len && blockquote[i] != '\n'; i++);
  i++;
  /* Goto begin of quote */
  for( ; i < len && blockquote[i] == ' '; i++);
  if(!strncmp("<p>", blockquote + i, 3))
    i += 3;
  size_t index_quote = i;
  size_t size_quote = 0;
  while(!size_quote)
  {
    for( ; i < len && blockquote[i] != '<'; i++);
    if(!strncmp("</p>", blockquote + i, 4))
    {
      size_quote = i - index_quote;
      i += 3;
    }
    i++;
  }
  char *quote = calloc(size_quote + 1, sizeof(char));
  for(size_t j = 0; j < size_quote; j++)
    quote[j] = blockquote[index_quote++];
  /* Until begin of number line, 4 lines below */
  for(size_t j = 0; j < 4; j++)
  {
    for( ; i < len && blockquote[i] != '\n'; i++);
    i++;
  }
  /* Goto begin of number */
  for( ; i < len && blockquote[i] != '#'; i++);
  i++;
  size_t index_num = i;
  size_t size_num = 0;
  for( ; i < len && blockquote[i] != '<'; i++);
  size_num = i - index_num;
  char *num = calloc(size_num + 1, sizeof(char));
  for(size_t j = 0; j < size_num; j++)
    num[j] = blockquote[index_num++];
  unsigned long number = strtoul(num, NULL, 10);
  free(num);
  return init_quotation(author, context, quote, number);
}

char* reformat_str(char *html)
{
  size_t len = strlen(html);
  char *str = calloc(len + 1, sizeof(char));
  size_t i = 0;
  size_t j = 0;
  while(i < len)
  {
    if(html[i] == '<')
    {
      if(!strncmp("<br />", html + i, 6))
      {
        str[j] = '\n';
        i += 5;
      }
    }
    else if(html[i] == '&')
    {
      i++;
      size_t index_spec = i;
      for( ; i < len && html[i] != ';'; i++);
      size_t size_spec = i - index_spec;
      char *spec = calloc(size_spec + 1, sizeof(char));
      for(size_t k = 0; k < size_spec; k++)
        spec[k] = html[index_spec++];
      if(spec[0] == '#')
      {
        long num = strtol(spec + 1, NULL, 10);
        str[j] = num;
      }
      else
      {
        if(!strncmp("lt", spec, 2))
          str[j] = '<';
        else if(!strncmp("gt", spec, 2))
          str[j] = '>';
        else if(!strncmp("quot", spec, 4))
          str[j] = '"';
      }
      free(spec);
    }
    else if(html[i] == ' ')
    {
      for( ; i < len && html[i] == ' '; i++);
      i--;
      str[j] = ' ';
    }
    else
      str[j] = html[i];
    i++;
    j++;
  }
  free(html);
  return str;
}

char* wrap80col(char *str)
{
  size_t len = strlen(str);
  size_t i = 0;
  size_t last_space = 0;
  size_t count = 0;
  while(i < len)
  {
    count++;
    if(str[i] == '\n')
      count = 0;
    if(str[i] == ' ')
      last_space = i;
    if(count == 80)
    {
      str[last_space] = '\n';
      i = last_space;
      count = 0;
    }
    i++;
  }
  return str;
}

struct quotation* reformat(struct quotation *quotation)
{
  quotation->author = wrap80col(reformat_str(quotation->author));
  if(quotation->context)
    quotation->context = wrap80col(reformat_str(quotation->context));
  quotation->quote = wrap80col(reformat_str(quotation->quote));
  return quotation;
}

size_t maxlinelen(char *str)
{
  size_t max = 0;
  size_t len = strlen(str);
  size_t i = 0;
  size_t count = 0;
  while(i < len)
  {
    if(str[i] == '\n')
    {
      max = count > max ? count : max;
      count = 0;
    }
    count++;
    i++;
  }
  return max ? max : len;
}

void print_quotation(struct quotation *quotation, int a, int c, int q, int n)
{
  if(a)
  {
    puts(quotation->author);
    for(size_t i = 0; i < maxlinelen(quotation->author); i++)
      putchar('=');
    putchar('\n');
  }
  if(c)
  {
    if(quotation->context)
    {
      puts(quotation->context);
      for(size_t i = 0; i < maxlinelen(quotation->context); i++)
        putchar('-');
      putchar('\n');
    }
  }
  if((a || c) && (q || n))
    putchar('\n');
  if(q)
    puts(quotation->quote);
  if(q && n)
    putchar('\n');
  if(n)
    printf("#%lu\n", quotation->number);
}
