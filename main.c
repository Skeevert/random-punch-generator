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

void	ft_rand_str(int fd)
{
	unsigned int	i;
	unsigned int	line;
	char			ch;
	size_t			offset;

	lseek(fd, 0, SEEK_SET);
	i = 0;
	srand(time(NULL));
	line = rand() % (ft_line_count(fd) - 1);
	ch = '#';
	offset = 0;
	while (line > 0)
	{
		read(fd, &ch, 1);
		if (ch == '\n')
			line--;
	}
}

void	ft_print_punch(int punch_fd)
{
	char	ch;
	
	ch = ' ';
	ft_rand_str(punch_fd);
	while(ch != '\n' && ch)
	{
		read(punch_fd, &ch, 1);
		write(1, &ch, 1);
	}
}

void	ft_print_setup(int setup_fd, int punch_fd)
{
	char	ch;
	
	ch = ' ';
	ft_rand_str(setup_fd);
	while(ch != '\n' && ch)
	{
		read(setup_fd, &ch, 1);
		if (ch == '#')
		{
			ft_print_punch(punch_fd);
			read(setup_fd, &ch, 1);
		}
		write(1, &ch, 1);
	}
}

int		main(void)
{
	int		setup_fd;
	int		punch_fd;

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
	ft_print_setup(setup_fd, punch_fd);
	return (0);
}
