# include "html_operations.h"
# include <stdlib.h>
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
  {
    system("rm -f [0-9]*");
    system("rm -f ./-[0-9]*");
    errx(1, "Unable to parse the HTML file. Check your Internet connection.");
  }
  free(blockquote);
  return file;
}

char* get_blockquote(FILE *file)
{
  fpos_t *begin = malloc(sizeof(fpos_t));
  fgetpos(file, begin);
  char c = EOF;
  int end = 0;
  char *tag_end_block = calloc(6, sizeof(char));
  size_t count = 0;
  /* Count number of char of the string to return */
  while(!end && (c = fgetc(file)) != EOF)
  {
    if(c == '<')
    {
      fgets(tag_end_block, 6, file);
      if(!strncmp("/a> −", tag_end_block, 5))
      {
        end = 1;
      }
      count += 5;
    }
    count++;
  }
  char *blockquote = calloc(count + 1, sizeof(char));
  fsetpos(file, begin);
  free(begin);
  for(size_t i = 0; i < count; i++)
    blockquote[i] = fgetc(file);
  free(tag_end_block);
  return blockquote;
}

char* dl_quote(long nb)
{
  if(nb == -1)
  {
    int ret = system("wget -q http://epiquote.fr/last -O ./-1");
    if(ret != 0)
    {
      errx(1, "Failed to get a random quote.");
    }
    return NULL;

  }
  if(nb == 0)
  {
    int ret = system("wget -q http://epiquote.fr/random -O ./0");
    if(ret != 0)
    {
      errx(1, "Failed to get a random quote.");
    }
    return NULL;
  }
  else
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
    return number;
  }
}

char* get_random_quote(void)
{
  dl_quote(0);
  FILE *file = fopen("./0", "r");
  char *blockquote = NULL;
  if(!file)
  {
    errx(1, "Failed to get a random quote.");
    return NULL;
  }
  else
  {
    blockquote = get_blockquote(find_first_blockquote(file));
    fclose(file);
  }
  return blockquote;
}

char* get_last_quote(void)
{
  dl_quote(-1);
  FILE *file = fopen("./-1", "r");
  char *blockquote = NULL;
  if(!file)
  {
    errx(1, "Failed to get the last quote.");
    return NULL;
  }
  else
  {
    blockquote = get_blockquote(find_first_blockquote(file));
    fclose(file);
  }
  return blockquote;
}

char* get_quote(long nb)
{
  char *number = dl_quote(nb);
  FILE *file = fopen(number, "r");
  free(number);
  char *blockquote = NULL;
  if(!file)
  {
    errx(1, "Failed to read the quote #%lu, this quote may not exist.", nb);
    return NULL;
  }
  else
  {
    blockquote = get_blockquote(find_first_blockquote(file));
    fclose(file);
  }
  return blockquote;
}

void print_file(FILE *file)
{
  char c = EOF;
  while((c = fgetc(file)) != EOF)
    putchar(c);
}
