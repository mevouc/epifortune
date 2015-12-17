# include "html_operations.h"
# include <err.h>
# include <string.h>

# define _DEFAULT_SOURCE

long get_max()
{
	long max = 0;
	system("wget -q http://epiquote.fr/last -O ./0");
	FILE* last = fopen("0", "r");
	if(!last)
		err(1, "Fail to get the max value of quotes.");
	else
	{
		max = find_last(last);
		fclose(last);
	}
	system("rm -f ./0");
	return max;
}

long find_last(FILE* file)
{
	long max = 0;
	char c = EOF;
	while(!max && ((c = fgetc(file)) != EOF))
	{
		if(c == '"')
		{
			char chev = fgetc(file);
			char sharp = fgetc(file);
			if(chev == '>' && sharp == '#')
			{
				if(!fscanf(file, "%ld</a>", &max))
					err(1, "Fail to find max.");
			}
			else
				fseek(file, -2, SEEK_CUR);
		}
	}
	return max;
}

long random_quote(long max)
{
	return 1 + (random() % max);
}

void print_quote(long nb)
{
	char *wget = malloc(32 * sizeof(char));
	wget = strcpy(wget, "wget -q http://epiquote.fr/");
	char *number = malloc(5 * sizeof(char));
	sprintf(number, "%ld", nb);
	wget = strcat(wget, number);
	system(wget);

	/* TODO
	 * Read quote in file named nb and print it.
	 */

	char *rmf = malloc(12 * sizeof(char));
	rmf = strcpy(rmf, "rm -f ");
	rmf = strcat(rmf, number);
	system(rmf);
}
