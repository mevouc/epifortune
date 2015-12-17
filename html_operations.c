# include "html_operations.h"
# include <err.h>

long get_max()
{
	long max = 0;
	system("wget -q http://epiquote.fr/last -O 0");
	FILE* last = fopen("0", "r");
	if(!last)
		err(1, "Fail to get the max value of quotes.");
	else
	{
		max = find_last(last);
		fclose(last);
	}
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
