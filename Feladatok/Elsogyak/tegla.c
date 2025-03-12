#include "tegla.h"

#include <math.h>

void set_tegla_data(Tegla* tegla, double x, double y, double z)
{
	tegla->x = x;
	tegla->y = y;
	tegla->z = z;
	
}

double calc_tegla_area(const Tegla* tegla)
{
	double area = tegla->x * tegla->y *2 + tegla->x * tegla->y *2 + tegla->y * tegla->z *2;
	return area;
}
double calc_tegla_terf(const Tegla* tegla)
{
	double terf = tegla->x * tegla->y * tegla->z ;
	return terf;
}