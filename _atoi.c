#include "shell.h"

/**
 * interactive - returns true if shell is interactive mode
 * @info: struct address
 *
 * Return: 1 if interactive mode, 0 otherwise
 */
int interactive(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * is_delim - checks if character is a delimeter
 * @b: the char to check
 * @delim: the delimeter string
 * Return: 1 if true, 0 if false
 */
int is_delim(char b, char *delim)
{
	while (*delim)
		if (*delim++ == b)
			return (1);
	return (0);
}

/**
 *_isalpha - checks for alphabetic character
 *@b: The character to input
 *Return: 1 if b is alphabetic, 0 otherwise
 */

int _isalpha(int b)
{
	if ((b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 *_atoi - converts a string to an integer
 *@w: the string to be converted
 *Return: 0 if no numbers in string, converted number otherwise
 */

int _atoi(char *w)
{
	int t, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (t = 0;  w[t] != '\0' && flag != 2; t++)
	{
		if (w[t] == '-')
			sign *= -1;

		if (w[t] >= '0' && w[t] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (w[t] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}

