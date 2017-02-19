#pragma once
#ifndef PIXEL_H
#define PIXEL_H

typedef struct pixel {
	int num;
	double x;
	double y;
	double z;
	double energy;
	double theta;
	double phi;
	double like;
} pixel;
#endif