//#include <fstream>
//#include <iostream>
//#include <stdio.h>
//#include <string.h>
//#include <tchar.h>
//#include <stdlib.h>
//#include "style.h"
//#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable:4996)
//#define MAX_LENGTH 30
////#define MAX_DATA 3000
//
//
//tmp_point *point_array[MAX_DATA];
//unsigned int num_tmp_points;
//
//tmp_point d1[MAX_DATA];
//tmp_point d2[MAX_DATA];
//unsigned int num_points;
//
//
//bool readtxt(const char* txt_file)
//{
//	int i;
//	double x,y,z, e;
//	char line[100];
//	//static char dummy[100];
//
//	FILE *in;
//
//	tmp_point *tmp_pnt;
//
//	in = fopen(txt_file, "r");
//	if(in == NULL) return false;
//
//	printf("reading file\n");
//	num_tmp_points = 0;
//	while (fgets(line, 100, in) !=NULL){
//		sscanf(line, "%d, %lf, %lf, %lf, %lf", &i, &x, &y, &z, &e);
//		tmp_pnt = (tmp_point *)malloc(sizeof(tmp_pnt));
//		tmp_pnt->num=i;
//		tmp_pnt->x = x;
//		tmp_pnt->y = y;
//		tmp_pnt->z = z;
//		tmp_pnt->energy = e;
//		num_tmp_points++;
//	}
//	fclose(in);
//	if(num_tmp_points > 0){
//		printf("complete &d \n", num_tmp_points);
//	}
//	else
//		printf("failure..\n");
//	return(num_tmp_points >0);
//
//}
//
//bool loadtxtfile(const char* txt_file)
//{
//	unsigned int i;
//
//	if(readtxt(txt_file))
//		printf("Numbers of data: %d\n", num_tmp_points);
//	else{
//		printf("cannot open");
//		return false;
//	}
//
//	num_points = 0;
//
//	for(i = 0; i < num_tmp_points; i++){
//		if(i < num_tmp_points){
//			if(point_array[i]->energy != 0.66 && point_array[i]->num < 64 && point_array[i + 1]->num > 63){
//				if(point_array[i]->energy + point_array[i + 1]->energy <= 0.66444 &&
//					point_array[i]->energy + point_array[i + 1]->energy >= 0.6555){
//						d1[num_points].x=point_array[i]->x;
//						d1[num_points].y=point_array[i]->y;
//						d1[num_points].z=point_array[i]->z;
//						d1[num_points].energy=point_array[i]->energy;
//						d2[num_points].x=point_array[i+1]->x;
//						d2[num_points].y=point_array[i+1]->y;
//						d2[num_points].z=point_array[i+1]->z;
//						d2[num_points].energy=point_array[i+1]->energy;
//						num_points++;
//				}
//			}
//		}
//	}
//	if(num_points == 0)
//		printf("no data matched\n");
//	for (i=0; i<num_tmp_points; i++)
//	{
//		free(point_array[i]);
//	}
//
//	return(true);
//
//}
//
//
