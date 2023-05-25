#include "shell.h"

/**
 * **strtow - splits a string into words. Repeat delimiters are ignored
 * @str: the input string
 * @d: the delimeter string
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **strtow(char *str, char *d)
{
	int b, h, r, n, numwords = 0;
	char **q;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!d)
		d = " ";
	for (b = 0; str[b] != '\0'; b++)
		if (!is_delim(str[b], d) && (is_delim(str[b + 1], d) || !str[b + 1]))
			numwords++;

	if (numwords == 0)
		return (NULL);
	q = malloc((1 + numwords) * sizeof(char *));
	if (!q)
		return (NULL);
	for (b = 0, h = 0; h < numwords; h++)
	{
		while (is_delim(str[b], d))
			b++;
		r = 0;
		while (!is_delim(str[b + r], d) && str[b + r])
			r++;
		q[h] = malloc((r + 1) * sizeof(char));
		if (!q[h])
		{
			for (r = 0; r < h; r++)
				free(q[r]);
			free(q);
			return (NULL);
		}
		for (n = 0; n < r; n++)
			q[h][n] = str[b++];
		q[h][n] = 0;
	}
	q[h] = NULL;
	return (q);
}

/**
 * **strtow2 - splits a string into words
 * @str: the input string
 * @d: the delimeter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **strtow2(char *str, char d)
{
	int b, h, r, n, numwords = 0;
	char **q;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (b = 0; str[b] != '\0'; b++)
		if ((str[b] != d && str[b + 1] == d) ||
		    (str[b] != d && !str[b + 1]) || str[b + 1] == d)
			numwords++;
	if (numwords == 0)
		return (NULL);
	q = malloc((1 + numwords) * sizeof(char *));
	if (!q)
		return (NULL);
	for (b = 0, h = 0; h < numwords; h++)
	{
		while (str[b] == d && str[b] != d)
			b++;
		r = 0;
		while (str[b + r] != d && str[b + r] && str[b + r] != d)
			r++;
		q[h] = malloc((r + 1) * sizeof(char));
		if (!q[h])
		{
			for (r = 0; r < h; r++)
				free(q[r]);
			free(q);
			return (NULL);
		}
		for (n = 0; n < r; n++)
			q[h][n] = str[b++];
		q[h][n] = 0;
	}
	q[h] = NULL;
	return (q);
}

