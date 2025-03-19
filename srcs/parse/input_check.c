
#include "../../include/miniRT.h"

int	input_check(int argc)
{
	if (argc == 2)
		return (1);
	ft_fprintf(2, "usage: ./miniRT scene/scene-description.rt\n");
	ft_fprintf(2, "scene-description.rt must be a valid file inside the 'scene' folder");
	return (0);
}

int	ft_parseerror(char *error, char *line)
{
	ft_fprintf(2, "ERROR\n%s\n   line:\n%s", error, line);
	return (0);
}
