# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "quotation.h"


struct quotation* init_quotation(char *author, char *context, char *quote,
                                long number)
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
  long number = strtol(num, NULL, 10);
  return init_quotation(author, context, quote, number);
}

char* reformat_soft(char *old)
{
  size_t len = strlen(old);
  char *new = calloc(2 * len, sizeof(char));
  size_t i = 0;
  size_t j = 0;
  int stop = 0;
  while(i < len && !stop)
  {
    if(old[i] == '&')
    {
      char *s = calloc(256, sizeof(char));
      sscanf(old + i, "&%s;", s);
      if(s[0] == '#')
      {
        int c;
        sscanf(s, "#%d", &c);
        new[j++] = c;
      }
      else if(!strncmp(s, "lt", 2))
        new[j++] = '<';
      else if(!strncmp(s, "gt", 2))
        new[j++] = '>';
      else if(!strncmp(s, "quot", 4))
        new[j++] = '"';
      for( ; old[i] != ';'; i++);
      free(s);
    }
    else if(old[i] == '\0')
    {
      new[j] = '\0';
      i--;
      stop = 1;
    }
    else
      new[j++] = old[i];
    i++;
  }
  free(old);
  return new;
}

char* reformat(char *html)
{
  size_t len = strlen(html);
  char *new = calloc(2 * len, sizeof(char));
  size_t i = 0;
  size_t j = 0;
  for( ; i < len && html[i] != '\n'; i++);
  int stop = 0;
  while((i < len) && !stop)
  {
    if(html[i] == '\n')
    {
      for( ; html[i + 1] == ' '; i++);
    }
    else if(html[i] == ' ')
    {
      for( ; html[i + 1] == ' '; i++);
      new[j++] = ' ';
    }
    else if(html[i] == '<')
    {
      char *s = calloc(256, sizeof(char));
      if(!strncmp(html + i, "<strong>", 8))
      {
        i += 8;
        size_t k = 0;
        while(html[i] != '<')
        {
          k++;
          new[j++] = html[i++];
        }
        for( ; html[i] != '\n'; i++);
        new[j++] = '\n';
        for(size_t m = 0; m < k; m++)
          new[j + m] = '=';
        j += k;
        new[j++] = '\n';
        i--;
      }
      else if(!strncmp(html + i, "</small>", 8))
        i += 7;
      else if(!strncmp(html + i, "</p>", 4))
      {
        i += 3;
        new[j++] = '\n';
      }
      else if(!strncmp(html + i, "<p>", 3))
        i += 2;
      else if(!strncmp(html + i, "<br />", 6))
      {
        i += 5;
        new[j++] = '\n';
      }
      else
        new[j++] = html[i];
      free(s);
    }
    else if(html[i] == '(')
    {
      char *s = calloc(256, sizeof(char));
      if(!strncmp(html + i, "(<em>", 5))
      {
        i += 5;
        size_t k = 0;
        while(html[i] != '<')
        {
          k++;
          new[j++] = html[i++];
        }
        for( ; html[i] != ')'; i++);
        i++;
        new[j++] = '\n';
        for(size_t m = 0; m < k; m++)
          new[j + m] = '-';
        j += k;
        new[j++] = '\n';
        i--;
      }
      else
        new[j++] = html[i];
      free(s);
    }
    else if(html[i] == '\0')
    {
      new[j] = '\0';
      i--;
      stop = 1;
    }
    else
      new[j++] = html[i];
    i++;
  }
  free(html);
  for( ; i > 0 && (new[i] == '\0' || new[i] == '\n'); i--);
  if((i > 0) && (i + 1 < len))
    if((new[i + 1] == '\n') && (i + 2 < len))
      new[i + 2] = '\0';
  return new;
}

void print_quotation(struct quotation *quotation)
{
  printf("%s\n", quotation->author);
  for(size_t i = 0; i < strlen(quotation->author); i++)
    putchar('=');
  putchar('\n');
  if(quotation->context)
  {
    printf("%s\n", quotation->context);
    for(size_t i = 0; i < strlen(quotation->context); i++)
      putchar('-');
    putchar('\n');
  }
  putchar('\n');
  printf("%s\n", quotation->quote);
  printf("\n#%lu\n", quotation->number);
}
