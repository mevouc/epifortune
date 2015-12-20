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

void print_file(FILE *file)
{
  char c = EOF;
  while((c = fgetc(file)) != EOF)
    putchar(c);
}
