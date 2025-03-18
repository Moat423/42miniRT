
#include "../../include/miniRT.h"

int	input_check(int argc, char **argv)
{
	if (argc == 2)
		return (1);
	ft_fprintf(2, "usage: ./miniRT scene/scene-description.rt\n");
	ft_fprintf(2, "scene-description.rt must be a valid file inside the 'scene' folder");
	return (0);
}
