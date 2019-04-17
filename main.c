
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static unsigned int		ft_line_count(int fd)
{
	unsigned int	line_count;
	char			temp;

	line_count = 0;
	temp = 'S';
	while (read(fd, &temp, 1))
		if (temp == '\n')
			line_count++;
	lseek(fd, 0, SEEK_SET);
	return (line_count + 1);
}

static char				*ft_rand_str(int fd)
{
	unsigned int	i;
	unsigned int	line;
	char			*str;
	size_t			offset;
	size_t			str_size;

	i = 0;
	srand(time(NULL));
	line = rand() % (ft_line_count(fd) - 1);
	str = (char *)malloc(1);
	*str = '#';
	str_size = 0;
	offset = 0;
	while (line > 0)
	{
		read(fd, str, 1);
		if (*str == '\n')
			line--;
		offset++;
	}
	*str = '#';
	while (*str != '\n')
	{
		read(fd, str, 1);
		str_size++;
	}
	free(str);
	if (!(str = (char *)malloc(sizeof(*str) * (str_size + 1))))
		return (NULL);
	lseek(fd, offset, SEEK_SET);
	while (str_size > 1)
	{
		read(fd, (str + i), 1);
		i++;
		str_size--;
	}
	*(str + i) = '\0';
	lseek(fd, 0, SEEK_SET);
	return (str);
}

static char				*ft_generate(int setup_fd, int punch_fd)
{
	char	*setup;
	char	*punch;
	char	*output;
	char	*punch_start;
	char	*setup_start;
	int		i;
	int		size;

	i = 0;
	setup = ft_rand_str(setup_fd);
	setup_start = setup;
	size = strlen(setup) + 1;
	output = (char *)malloc(sizeof(*output) * size);
	while (*setup)
	{
		if (*setup == '#')
		{
			sleep(1);
			punch = ft_rand_str(punch_fd);
			punch_start = punch;
			size = size + strlen(punch);
			if(!(output = (char *)realloc(output, sizeof(*output) * size)))
				return (NULL);
			while (*punch)
			{
				output[i] = *punch;
				i++;
				punch = punch + 1;
			}
			free(punch_start);
		}
		else
		{
			output[i] = *setup;
			i++;
		}
		setup = setup + 1;
	}
	output[i] = '\0';
	free(setup_start);
	return (output);
}

int						main(void)
{
	int		setup_fd;
	int		punch_fd;
	char	*output;

	if ((setup_fd = open("setup", O_RDONLY)) == -1)
		{
			write(2, "Cannot find file \"setup\"\n", 25);
			return (1);
		}
	if ((punch_fd = open("punch", O_RDONLY)) == -1)
		{
			write(2, "Cannot find file \"punch\"\n", 25);
			return (1);
		}
	output = ft_generate(setup_fd, punch_fd);
	if (!output)
		write(2, "Oops!\n", 6);
	else
		write(1, output, strlen(output));
	write(1, "\n", 1);
	close(setup_fd);
	close(punch_fd);
	return (0);
}
