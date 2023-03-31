#include "cube.h"
#include <math.h>

void set_cube_data(Cube* cube, double x, double y, double z)
{
      cube->x = x;
	  cube->y = y;
	  cube->z = z;

}

double calc_cube_area(const Cube* cube)
{
    double area = 2*(cube->x*cube->y + cube->y*cube->z + cube->z*cube->a);
	return area;

}