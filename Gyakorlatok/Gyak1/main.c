#include "cube.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
     Cube cube;
	 double area;
	 
	 set_cube_data(&cube, 5, 10, 8);
	 area = calc_cube_area(&cube);
	 
	 printf("Circle area: %lf\n", area);
	 
	 return 0;

}