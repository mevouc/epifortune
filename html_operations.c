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
	char *quote = calloc(count + 1, sizeof(char));
	fsetpos(file, begin);
	for(size_t i = 0; i < count - 1; i++)
		quote[i] = fgetc(file);
	free(blockquote);
	return quote;
}

char* get_random_quote(void)
{
	system("wget -q http://epiquote.fr/random -O ./0");
	FILE *file = fopen("0", "r");
	char *blockquote = NULL;
	if(!file)
		err(1, "Fail to get a list of random quotes.\n");
	else
	{
		blockquote = get_blockquote(find_first_blockquote(file));
		fclose(file);
	}
	system("rm -f ./0");
	return blockquote;
}

char* get_quote(long nb)
{
	char *wget = malloc(32 * sizeof(char));
	wget = strcpy(wget, "wget -q http://epiquote.fr/");
	char *number = malloc(5 * sizeof(char));
	sprintf(number, "%ld", nb);
	wget = strcat(wget, number);
	system(wget);
	free(wget);
	FILE* file = fopen(number, "r");
	char *blockquote = NULL;
	if(!file)
		err(1, "Fail to read the quote #%ld.\n", nb);
	else
	{
		blockquote = get_blockquote(find_first_blockquote(file));
		fclose(file);
	}
	char *rmf = malloc(12 * sizeof(char));
	rmf = strcpy(rmf, "rm -f ");
	rmf = strcat(rmf, number);
	free(number);
	system(rmf);
	free(rmf);
	return blockquote;
}

void print_file(FILE *file)
{
	char c = EOF;
	while((c = fgetc(file)) != EOF)
		putchar(c);
}
