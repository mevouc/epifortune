# include "html_operations.h"
# include <err.h>
# include <string.h>

# define _DEFAULT_SOURCE

FILE* find_first_blockquote(FILE *file)
{
  char c = EOF;
  char* blockquote = calloc(16, sizeof(char));
  while((c = fgetc(file)) != EOF)
  {
    if(c == '<')
    {
      fgets(blockquote, 11, file);
      if(!strncmp("blockquote", blockquote, 11))
        break;
    }
  }
  for(; c != EOF && c != '\n'; c = fgetc(file));
  if(c == EOF)
    return NULL;
  free(blockquote);
  return file;
}

char* get_blockquote(FILE *file)
{
  fpos_t *begin = malloc(sizeof(fpos_t));
  fgetpos(file, begin);
  int end = 0;
  char c = EOF;
  char *blockquote = calloc(16, sizeof(char));
  size_t count = 0;
  while(!end && (c = fgetc(file)) != EOF)
  {
    count++;
    if(c == '<')
    {
      fgets(blockquote, 12, file);
      if(!strncmp("/blockquote", blockquote, 12))
      {
        count -= 12;
        end = 1;
      }
      else
        count += 10;
    }
  }
  char *quote = calloc(count * 2, sizeof(char));
  fsetpos(file, begin);
  int stop = 0;
  size_t endblockquote = 0;
  for(size_t i = 0; i < count + 2 && !stop; i++)
  {
    char c = fgetc(file);
    if(c == '<')
      endblockquote++;
    if(endblockquote == 1 && c == '/')
      endblockquote++;
    if(endblockquote == 2 && c == 'b')
      stop = 1;
    if(c == EOF)
      stop = 1;
    quote[i] = c;
  }
  if(endblockquote)
  {
    size_t i = 0;
    for(i = strlen(quote); i > 0 && quote[i] != '\n'; i--);
    if(quote[i] == '\n')
      quote[i] = '\0';
  }
  free(blockquote);
  return quote;
}

char* get_random_quote(void)
{
  int ret = system("wget -q http://epiquote.fr/random -O ./0");
  if(ret != 0)
  {
    errx(1, "Fail to get a random quote.");
    return NULL;
  }
  FILE *file = fopen("0", "r");
  char *blockquote = NULL;
  if(!file)
  {
    errx(1, "Fail to get a random quote.");
    return NULL;
  }
  else
  {
    printf("NOT NULL\n");
    blockquote = get_blockquote(find_first_blockquote(file));
    fclose(file);
  }
  return blockquote;
}

char* get_quote(unsigned long nb)
{
  if(nb > 999999)
  {
    errx(1, "There are clearly not so much quotes.");
    return NULL;
  }
  char *wget = calloc(32, sizeof(char));
  wget = strcpy(wget, "wget -q http://epiquote.fr/");
  char *number = calloc(24, sizeof(char));
  sprintf(number, "%lu", nb);
  wget = strcat(wget, number);
  system(wget);
  free(wget);
  FILE* file = fopen(number, "r");
  char *blockquote = NULL;
  if(!file)
  {
    errx(1, "Fail to read the quote #%lu.", nb);
    free(number);
    return NULL;
  }
  else
  {
    blockquote = get_blockquote(find_first_blockquote(file));
    fclose(file);
  }
  char *rmf = malloc(12 * sizeof(char));
  rmf = strcpy(rmf, "rm -f ");
  rmf = strcat(rmf, number);
  free(number);
  free(rmf);
  return blockquote;
}

char* reformat_soft(char *old)
{
  size_t len = strlen(old);
  char *new = calloc(2 * len, sizeof(char));
  size_t i = 0;
  size_t j = 0;
  while(i < len)
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
  while(i < len)
  {
    if(html[i] == '\n')
    {
      for( ; html[i + 1] == ' '; i++);
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
    else
      new[j++] = html[i];

    i++;
  }

  free(html);
  return new;
}

void print_file(FILE *file)
{
  char c = EOF;
  while((c = fgetc(file)) != EOF)
    putchar(c);
}
