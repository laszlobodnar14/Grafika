#ifndef TEGLA_H
#define TEGLA_H

typedef struct Tegla
{
	double x;
	double y;
	double z;
	
} Tegla;



void set_tegla_data(Tegla* tegla, double x, double y, double z);


double calc_tegla_area(const Tegla* tegla);
double calc_tegla_terf(const Tegla* tegla);

#endif // CIRCLE_H