#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define FLOAT float

FLOAT	ft_strtof(char *str, char **rest)
{
	double	ret;
	double	sign;
	double	dec;
	bool	dot;

	sign = 1;
	dot = false;
	if (*str == '-')
		sign = -1;
	if (*str == '+' || *str == '-')
		str++;
	if (*str == '.' && !isdigit(*(str + 1)))
		return (0);
	ret = 0;
	dec = 1.0 / 10;
	while (isdigit(*str) || *str == '.')
	{
		if (dot)
		{
			if (*str == '.')
				break ;
			ret += dec * (*str - '0');
			dec /= 10;
		}
		else
		{
			if (*str == '.')
				dot = true;
			else if (isdigit(*str))
				ret = ret * 10 + (*str - '0');
		}
		str++;
	}
	*rest = str;
	return (ret * sign);
}

/*
int main(int argc, char **argv)
{
	extern int  errno;
	char *copy = strdup(argv[1]);
	printf("ft  :%.100f\n", ft_strtof(argv[1], &argv[1]));
	strerror(errno);
	printf("std :%.100f\n", strtof(copy, &copy));
	strerror(errno);
	printf("my  :%s\n", argv[1]);
	printf("std :%s\n", copy);
	return (0);
}
*/
