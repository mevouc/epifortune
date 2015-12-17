# include "html_operations.h"

int main()
{
	FILE* file = fopen("43", "r");
	file = find_first_blockquote(file);
	printf("%s\n", get_blockquote(file));
	return 0;
}

