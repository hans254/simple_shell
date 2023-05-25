#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t w = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		w = getline(buf, &len_p, stdin);
#else
		w = _getline(info, buf, &len_p);
#endif
		if (w > 0)
		{
			if ((*buf)[w - 1] == '\n')
			{
				(*buf)[w - 1] = '\0'; /* remove trailing newline */
				w--;
			}
			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = w;
				info->cmd_buf = buf;
			}
		}
	}
	return (w);
}

/**
 * get_input - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t b, f, len;
	ssize_t w = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	w = input_buf(info, &buf, &len);
	if (w == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		f = b; /* init new iterator to current buf position */
		p = buf + b; /* get pointer for return */

		check_chain(info, buf, &f, b, len);
		while (f < len) /* iterate to semicolon or end */
		{
			if (is_chain(info, buf, &f))
				break;
			f++;
		}

		b = f + 1; /* increment past nulled ';'' */
		if (b >= len) /* reached end of buffer? */
		{
			b = len = 0; /* reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (_strlen(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _getline() */
	return (w); /* return length of buffer from _getline() */
}

/**
 * read_buf - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: w
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t w = 0;

	if (*i)
		return (0);
	w = read(info->readfd, buf, READ_BUF_SIZE);
	if (w >= 0)
		*i = w;
	return (w);
}

/**
 * _getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: z
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t b, len;
	size_t o;
	ssize_t w = 0, z = 0;
	char *p = NULL, *new_p = NULL, *a;

	p = *ptr;
	if (p && length)
		z = *length;
	if (b == len)
		b = len = 0;

	w = read_buf(info, buf, &len);
	if (w == -1 || (w == 0 && len == 0))
		return (-1);

	a = _strchr(buf + b, '\n');
	o = a ? 1 + (unsigned int)(a - buf) : len;
	new_p = _realloc(p, z, z ? z + o : o + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (z)
		_strncat(new_p, buf + b, o - b);
	else
		_strncpy(new_p, buf + b, o - b + 1);

	z += o - b;
	b = o;
	p = new_p;

	if (length)
		*length = z;
	*ptr = p;
	return (z);
}

/**
 * sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

