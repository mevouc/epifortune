# include "html_operations.h"

int main()
{
	FILE* file = fopen("806", "r");
	file = find_first_blockquote(file);
	printf("%s\n", reformat(get_blockquote(file)));
	return 0;
}

