#include "tegla.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
	Tegla tegla;
	double area;
	double terf;

	set_tegla_data(&tegla, 5, 10, 5);
	area = calc_tegla_area(&tegla);
	terf= calc_tegla_terf(&tegla);
	
	printf("Tegla Felszin: %lf\n", area);
	printf("Tegla Terfogat: %lf\n", terf);
	
	return 0;
}