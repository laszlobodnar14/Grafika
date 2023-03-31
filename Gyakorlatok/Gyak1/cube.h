#ifndef CUBE_H
#define CUBE_H

typedef struct Cube
{
     double x;
	 double y;
	 double z;

} Cube;

void set_cube_data(Cube* cube, double x, double y, double z);

double calc_cube_area(const Cube* cube);

#endif // CUBE_H