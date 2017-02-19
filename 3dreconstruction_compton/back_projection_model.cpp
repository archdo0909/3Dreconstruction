#include "pixel.h"
#include <iostream>
#include <cmath>
#include <math.h>

#define image_pixel_num 60;
#define PI 3.14;
#define data_point_num 100;


static pixel image[360];
//static pixel layer1[64];
//static pixel layer2[64];
static pixel data[64][64][70];
static pixel d1[1];
static pixel d2[1];

//double Klevin_Nishina(double beta, double Energy) {
//	double r = Energy / (0.511);
//	double r0 = 2.817 * pow(10.0, -15.0);
//	double K = pow(r0, 2) * (1 + pow(cos(beta), 2.0)) * ((1 + r*(1 - cos(beta))) + pow(r, 2)*pow((1 - cos(beta)), 2)) / pow((1 + r*(1 - cos(beta))), 3);
//	return K;
//
//}
double inner_product(pixel vector1, pixel vector2)
{
	double inner = vector1.x * vector2.x + vector1.y*vector2.y + vector1.z*vector2.z;

	return inner;
}
double vector_angle(pixel vector1, pixel vector2, pixel vector3, pixel vector4)
{
	double cos_Angle = ((vector1.x - vector2.x)*(vector3.x - vector4.x) + (vector1.y - vector2.y)*(vector3.y - vector4.y)
		+ (vector1.z - vector2.z)*(vector3.z - vector4.z)) / (sqrt(pow((vector1.x - vector2.x), 2) + pow((vector1.y - vector2.y), 2)
		+ pow((vector1.z - vector2.z), 2))*sqrt(pow((vector3.x - vector4.x), 2) + pow((vector3.y - vector4.y), 2) + pow((vector3.z - vector4.z), 2)));

	return cos_Angle;
}
double Klein_Nishina(double beta, double E)
{
	double r0 = 2.8179403227 * pow(10, -13);
	double r = 0.66 / 0.511;
	double K = (pow(r0, 2) / 2) * ((1 + pow(cos(beta), 2))*(1 + r*(1 - cos(beta))) + pow(r, 2)*(pow((1 - cos(beta)), 2))) / pow((1 + r*(1 - cos(beta))), 3);

	return K;
} 
double system_matrix(pixel image1, pixel detector1, pixel detector2)
{
	double p = 3.0;
	double pi = PI;
	double t;
	double Angle = acos(vector_angle(image1, detector1, detector1, detector2)); //radian
	double V1Oj_len_2 = pow((image1.x - detector1.x), 2) + pow((image1.y - detector1.y), 2) + pow((image1.z - detector1.z), 2);
	double sigma = 1.0;
	double cos_beta = 1 - 0.511*((1 / detector2.energy) - (1 / (detector1.energy + detector2.energy)));
	double beta = acos(cos_beta);
	double cos_theta_Oj = abs(image1.z / sqrt(pow(image1.x ,2) + pow(image1.y,2) + pow(image1.z,2)));


	if (abs(Angle - beta) < p*sigma) {
		t = Klein_Nishina(beta, 0.66)*(cos_theta_Oj/V1Oj_len_2)*(1/(sqrt(2*pi)*sigma))*exp((-1/2)*pow((Angle-beta)/beta,2));
	}
	else {
		t = 0;
	}

	return t;
}
void projection()
{
	int i = 0;
	int j = 0;
	int k = 0;
	int x = 0;
	int y = 0;
	int l = 0;
	int z = 0;
	int data_num = data_point_num;
	int gamma_total = 1000;
	double Sj = data_num / gamma_total;
	double pi = PI;
	double r = 0;
	int cnt = 0;
	int layer1_num = layer1_pixel_num;
	int layer2_num = layer2_pixel_num;
	int pixel_num = image_pixel_num;


	//insert coordinates
	for (i = 0; i < 360; i++) {
		int row = i / 60;
		int colomn = i % 60;
		image[i].theta = ((-30 + colomn) / 180) *pi;
		image[i].phi = ((-30 + row) / 180) * pi;
		image[i].x = r*cos(((-30 + colomn) / 180) *pi)*sin(((-30 + row) / 180) * pi);
		image[i].y = r*sin(((-30 + row) / 180) * pi)*sin(((-30 + colomn) / 180) *pi);
		image[i].z = r*cos(((-30 + row) / 180) * pi);
		image[i].like = 0;
	}

	///tij*ramda   image pixel related with parameter j, data-bin num i, data_num k, iteration l
	for(l = 0; l < 5; l++){
		for(k = 0 ; k < data_num; k++){
			for()
		}
	}



}