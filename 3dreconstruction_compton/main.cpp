#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include "style.h"
#define MAX_LENGTH 300
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

#define txt_file "Geant4(10cm).txt"
//int p = 2*max_x + 1;
//int q = 2*max_y + 1;
//int r = max_z;


int window_size_x = 1000;
int window_size_y = window_size_x;
bool up_flag = false;
bool down_flag = false;
bool MouseFlagLeft = false;
bool MouseFlagRight = false;
bool MouseFlagMiddle = false;
bool View_point_flag = false;
//double View_from[3] = {-8.0 ,8.0, 0.0};
double View_from[3] = {0.0, 30.0, -80.0};
double View_to[3] = {0.0, -40.0, 80.0};
double View_from2[3] = {0.0, 10.0, 0.01 };
double View_to2[3] = {0.0, -10.0, 0.0 };
using namespace std;

#if 1

tmp_point *point_array[MAX_DATA];
unsigned int num_tmp_points;

tmp_point d1[MAX_DATA];
tmp_point d2[MAX_DATA];
unsigned int num_points;

void readtxt()
{
	int i;
	double x,y,z,e;
	char line[150];
	//static char dummy[100];

	FILE *in;

	tmp_point *tmp_pnt;

	in = fopen(txt_file, "rt");
	if(in == NULL) printf("no data\n");

	//ofstream fout;
	//fout.open("insert_coordinates.txt");

	cout << "reading file\n" << endl;
	num_tmp_points = 0;
	while (fgets(line, 150, in) !=NULL){
		sscanf(line, "%d, %lf, %lf, %lf, %lf", &i, &x, &y, &z, &e);
		tmp_pnt = (tmp_point *)malloc(sizeof(tmp_point));
		point_array[num_tmp_points] = tmp_pnt;
		tmp_pnt->num=i;
		tmp_pnt->x = x;
		tmp_pnt->y = y;
		tmp_pnt->z = z;
		tmp_pnt->energy = e;
		num_tmp_points++;
		//fout << i << ", " << x << ", " << y << ", " << z << ", " << e << "\n" << endl;
		//printf("%d, %lf, %lf, %lf, %lf", i, x, y, z, e);
	}
	fclose(in);
	if(num_tmp_points > 0){
		printf("complete %d \n", num_tmp_points);
	}
	else
		printf("failure..\n");

	/*if (fout.is_open() == true)
	{
	fout.close();
	}*/

}

void loadtxtfile()
{
	unsigned int i;

	readtxt();

	num_points = 0;
	for(i = 0; i < num_tmp_points; i++){
		if(i != num_tmp_points-1){
			if(point_array[i]->energy < 0.66 && point_array[i]->num < 64 && point_array[i + 1]->num > 63){   //&& point_array[i + 1]->num > 63
				if(point_array[i]->energy + point_array[i + 1]->energy <= 0.66444 && point_array[i]->energy + point_array[i + 1]->energy >= 0.65555){
						d1[num_points].num=point_array[i]->num;
					    d1[num_points].x=point_array[i]->x;
						d1[num_points].y=point_array[i]->y;
						d1[num_points].z=point_array[i]->z;
						d1[num_points].energy=point_array[i]->energy;
						d2[num_points].num=point_array[i+1]->num;
						d2[num_points].x=point_array[i+1]->x;
						d2[num_points].y=point_array[i+1]->y;
						d2[num_points].z=point_array[i+1]->z;
						d2[num_points].energy=point_array[i+1]->energy;
						num_points++;
				}
			}
		}
	}
	cout <<"matched number point values is \n" <<num_points << endl;
	if(num_points == 0)
		printf("no data matched\n");
	for (i=0; i<num_tmp_points; i++)
	{
		free(point_array[i]);
	}

}
#endif


// vector
typedef struct Vector3D{
	double x;
	double y;
	double z;
	int vol;
} point;

// nums of inside array expressed as [x * y * z]
static point voxel_position[31][31][31];   
int max_x = 31;
int max_y = 31;
int max_z = 31;

struct Energy_deposit{
	double energy;
};
// length of vector
double get_vector_length (point v)
{
	return pow( (v.x * v.x) + (v.y * v.y) + (v.z * v.z), 0.5 );
}

double get_new_vector_length(double x1,double y1, double z1, double x2, double y2, double z2)
{
	double length = sqrt( pow( (x1 - x2) , 2) + pow( (y1 - y2) , 2) + pow( (z1 - z2) , 2));
	return length;
}

// vector inner product
double inner_product(point vl, point vr)
{
	return vl.x * vr.x + vl.y * vr.y + vl.z * vr.z;
}

//double Angle_vector(point A, point B)
//{
//	double length_A = get_vector_length(A);
//	double lenght_B = get_vector_length(B);
//
//	double cos_theta = inner_product(A, B) / (length_A * length_A);
//
//	double theta = acos(cos_theta);
//	// to make degree
//
//	// theta_deg = theta * 180 / PI;
//
//	return theta;
//}

double Angle_vector(double x1, double y1, double z1, double x2, double y2, double z2) {
	double length_A = pow((pow(x1, 2) + pow(y1, 2) + pow(z1, 2)),1/2);
	double length_B = pow((pow(x2, 2) + pow(y2, 2) + pow(z2, 2)),1/2);

	double cos_theta = ((x1 * x2) + (y1 * y2) + (z1 * z2)) / (length_A * length_B);
	double theta = acos(cos_theta);

	return theta;
}

double Angle_vector_com(double d1x, double d1y, double d1z, double d2x, double d2y, double d2z, double x1, double y1, double z1)
{
	double length_d2d1 = sqrt( pow((d1x - d2x),2) + pow((d1y-d2y),2) + pow( (d1z - d2z) ,2));
	double length_d1x = sqrt( pow((x1 - d1x) ,2) + pow ((y1 - d1y) ,2) + pow ((z1 - d1z) ,2));

	double cos_theta = ((d1x - d2x)*(x1 - d1x) + (d1y-d2y)*(y1 - d1y) + (d1z - d2z)*(z1 - d1z)) / (length_d2d1 * length_d1x);
	double theta = acos(cos_theta);
	
	return cos_theta;
}
point compose_vector(point A, point B)
{
	point C;
	C.x = B.x - A.x;
	C.y = B.y - A.y;
	C.z = B.z - A.z;

	return C;
}
double scattering_angle(double A, double B)
{

	double Angle = (1 + 0.511*(1/(A + B) - 1 / (B)));
	double Angle_deg = acos(Angle);
	return Angle;

}
void View_control(bool vector_flag){
	double View_distance;
	double temp[5];
	temp[2] = View_from[2] - View_to[2];
	temp[1] = View_from[0] - View_to[0];
	temp[0] = pow(temp[1], 2.0) + pow(temp[2], 2.0);
	View_distance = pow(temp[0], 0.5);
		printf("%f\n", View_distance);
	temp[0] = View_from[2] - View_to[2];
	temp[3] = temp[0] / View_distance;
	temp[1] = View_from[0] - View_to[0];
	temp[4] = temp[1] / View_distance;
	temp[2] = atan2(temp[4], temp[3]);
	//temp[2] = acos(temp[1]);
	if (vector_flag) temp[2] = temp[2] + 0.01;
	else temp[2] = temp[2] - 0.01;
	temp[0] = View_distance * cos(temp[2]);
	temp[1] = View_distance * sin(temp[2]);
	View_from[2] = View_to[2] + temp[0];
	View_from[0] = View_to[0] + temp[1];

}
void View_control_up_down(bool vector_flag){
	double View_distance;
	double temp[5];
	temp[2] = View_from[1] - View_to[1];
	temp[1] = View_from[0] - View_to[0];
	temp[0] = pow(temp[1], 2.0) + pow(temp[2], 2.0);
	View_distance = pow(temp[0], 0.5);
	//	printf("%f\n", View_distance);
	temp[0] = View_from[1] - View_to[1];
	temp[3] = temp[0] / View_distance;
	temp[1] = View_from[0] - View_to[0];
	temp[4] = temp[1] / View_distance;
	temp[2] = atan2(temp[4], temp[3]);
	//temp[2] = acos(temp[1]);
	if (vector_flag) temp[2] = temp[2] + 0.01;
	else temp[2] = temp[2] - 0.01;
	temp[0] = View_distance * cos(temp[2]);
	temp[1] = View_distance * sin(temp[2]);
	View_from[1] = View_to[1] + temp[0];
	View_from[0] = View_to[0] + temp[1];
}
void View_control2(bool vector_flag){
	double View_distance;
	double temp[5];
	temp[2] = View_from2[2] - View_to2[2];
	temp[1] = View_from2[0] - View_to2[0];
	temp[0] = pow(temp[1], 2.0) + pow(temp[2], 2.0);
	View_distance = pow(temp[0], 0.5);
	temp[0] = View_from2[2] - View_to2[2];
	temp[3] = temp[0] / View_distance;
	temp[1] = View_from2[0] - View_to2[0];
	temp[4] = temp[1] / View_distance;
	temp[2] = atan2(temp[4], temp[3]);
	if (vector_flag) temp[2] = temp[2] + 0.01;
	else temp[2] = temp[2] - 0.01;
	temp[0] = View_distance * cos(temp[2]);
	temp[1] = View_distance * sin(temp[2]);
	View_from2[2] = View_to2[2] + temp[0];
	View_from2[0] = View_to2[0] + temp[1];
}

void Reconstruction()
{
	/*point d1[1];
	point d2[1];
	struct Energy_deposit d1_dep[1];
	struct Energy_deposit d2_dep[1];

	int i, j, k, m ; 

	for(i = 0; i < 1; i++)
	{
	d1[i].x = 0.0;
	d1[i].y = 0.0;
	d1[i].z = 0.0;

	d2[i].x = 0.0;
	d2[i].y = 0.0;
	d2[i].z = -80.0;

	d1_dep[i].energy = 0.22;
	d2_dep[i].energy = 0.44;

	}
	cout << "insert_info" << endl;*/

	loadtxtfile();

	int i, j, k, m;
    m = num_points;

	for(i = 0; i < max_x ; i++)
	{
		for (j = 0; j < max_y; j++)
		{
			for(k=0; k < max_z; k++)
			{
				//printf("%d, %d, %d \n", (i-(max_x - 1)/2), (j-(max_y - 1)/2), k);
				voxel_position[i][j][k].x=(double) (i-(max_x - 1)/2);
				voxel_position[i][j][k].y=(double) (j-(max_y - 1)/2);
				voxel_position[i][j][k].z=(double) k;
				voxel_position[i][j][k].vol=0; 
				//printf("%f, %f, %f \n", voxel_position[i][j][k].x, voxel_position[i][j][k].y, voxel_position[i][j][k].z);
			}
		}
	}
	cout << "insert_coordinates" << endl;

    int cnt=0;

	/*ofstream fout;
	fout.open("Coordinates.txt");*/

	for(i = 0; i < max_x; i++){
		for (j = 0; j < max_y; j++){
			for(k = 0; k < max_z; k++){
				for(m = 0; m < num_points; m++){
					//if(Angle_vector( compose_vector(d2[k],d1[k]), compose_vector(d1[k], voxel_position[i][j][z])) <= 
					//	acos(0.5/get_vector_length(compose_vector(d1[k], voxel_position[i][j][z]))) + scattering_angle(d1_dep[k], d2_dep[k]) &&
					//	Angle_vector( compose_vector(d2[k],d1[k]), compose_vector(d1[k], voxel_position[i][j][z])) >= 
					//	scattering_angle(d1_dep[k], d2_dep[k]) - acos(0.5/get_vector_length(compose_vector(d1[k], voxel_position[i][j][z]))) ){
					//	voxel_position[i][j][z].vol = voxel_position[i][j][z].vol + 1;
					//	cnt++;
					////cout << Angle_vector( compose_vector(d2[k],d1[k]), compose_vector(d1[k], voxel_position[i][j][z])) << endl;
					//}
					if(Angle_vector_com(d1[m].x, d1[m].y, d1[m].z, d2[m].x, d2[m].y, d2[m].z, voxel_position[i][j][k].x, voxel_position[i][j][k].y, voxel_position[i][j][k].z)
						<= 0.25 / get_new_vector_length(d1[m].x, d1[m].y, d1[m].z, voxel_position[i][j][k].x, 
						voxel_position[i][j][k].y, voxel_position[i][j][k].z) + scattering_angle(d1[m].energy, d2[m].energy) && Angle_vector_com(d1[m].x, d1[m].y, d1[m].z, d2[m].x, d2[m].y, d2[m].z, voxel_position[i][j][k].x, voxel_position[i][j][k].y, voxel_position[i][j][k].z)
						>= scattering_angle(d1[m].energy, d2[m].energy) - 0.25 / get_new_vector_length(d1[m].x, d1[m].y, d1[m].z, voxel_position[i][j][k].x,
						voxel_position[i][j][k].y, voxel_position[i][j][k].z)){
							voxel_position[i][j][k].vol = voxel_position[i][j][k].vol + 1;
							cnt++;
					}
				}
			}
		}
	}
	//for(m = 0 ; m < num_points; m++){
	//	fout << m <<", "<< d1[m].num <<", "<< d2[m].num << endl; 
	//}
	//if (fout.is_open() == true)
	//{
	//	fout.close();
	//}

	cout<<"done1"<<endl;

}

void voxel_simulation()
{

	int i, j, k, z;
	int cnt=0;

	double max = -10.0;
	double min = 10000.0;

	Reconstruction();

	cout << "complete1" << endl;

	GLfloat changing[] = {0.5, 0.5, 1.0, 1.0}; //Blue

	for(i = 0; i < max_x; i++){
		for(j = 0; j< max_y; j++){
			for(k = 0; k < max_z; k++){
				if(voxel_position[i][j][k].vol > max){
					max = voxel_position[i][j][k].vol;
				}
				if(voxel_position[i][j][k].vol < min){
					min = voxel_position[i][j][k].vol;
				}
			}
		}
	}
	//printf("%lf, %lf \n", max, min);

	for(i = 0; i < max_x; i++){
		for(j = 0; j< max_y; j++){
			for(k = 0; k < max_z; k++){
				if(voxel_position[i][j][k].vol > 0){
					glPushMatrix();
					//glEnable(GL_BLEND);
					if(voxel_position[i][j][k].vol < (max + min) / 2){
						changing[0] = (voxel_position[i][j][k].vol - min) / ((max - min) / 2)  ;
						changing[1] = 0.1;
						changing[2] = ((max + min) / 2 - voxel_position[i][j][k].vol)/((max - min) / 2);
						glColor3f((voxel_position[i][j][k].vol - min) / ((max - min) / 2), 0.1, ((max + min) / 2 - voxel_position[i][j][k].vol)/((max - min) / 2));
					}
					else{
						changing[0] = (max - voxel_position[i][j][k].vol) / ((max - min) / 2)  ;
						changing[1] = (voxel_position[i][j][k].vol - (max + min) / 2) / ((max - min) / 2);
						changing[2] = 0.1;
						glColor3f((voxel_position[i][j][k].vol - (max + min) / 2) / ((max - min) / 2),(max - voxel_position[i][j][k].vol) / ((max - min) / 2), 0.1);
						//printf("%f, %f, %f \n", changing[0], changing[1], changing[2]);
					}
					//glColor3f(0.0, 0.5, 0.8);
					//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, changing);
					glTranslated((GLdouble)voxel_position[i][j][k].x, (GLdouble)voxel_position[i][j][k].z, (GLdouble)voxel_position[i][j][k].y);
					glutSolidCube(0.5);
					glPopMatrix();
					cnt++;
				}
			}
		}
	}
	cout << cnt << endl;
	//glPushMatrix();
}
void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (MouseFlagLeft){
		if (View_point_flag) View_control(false);
		else View_control2(false);
	}
	else if (MouseFlagRight){
		if (View_point_flag) View_control(true);
		else View_control2(true);
	}
	if (up_flag) View_control_up_down(true);
	if (down_flag) View_control_up_down(false);


	gluLookAt(View_from[0], View_from[1], View_from[2], View_to[0], View_to[1], View_to[2], 0.0, 1.0, 0.0);

	//glPushMatrix();
	voxel_simulation();
	//cout << "complete2" << endl;
	//glPopMatrix;
	//
	glutSwapBuffers();

	//glColor3f(1.0, 0.0, 0.0);
	//glLoadIdentity();
	//gluLookAt(View_from[0], View_from[1], View_from[2], View_to[0], View_to[1], View_to[2], 0.0, 1.0, 0.0);
	//glutSolidCube(5.0);
	//glTranslated(0.0, 0.0, 0.0);
	//glFlush();



}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		switch (state) {
		case GLUT_UP:
			if (MouseFlagLeft){
				MouseFlagLeft = false;
			}
			break;
		case GLUT_DOWN:
			MouseFlagLeft = true;
			if (x < window_size_x * 2 / 3) View_point_flag = true;
			else View_point_flag = false;
			break;
		default:
			break;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		switch (state) {
		case GLUT_UP:
			if (MouseFlagRight) MouseFlagMiddle = false;
			break;
		case GLUT_DOWN:
			MouseFlagMiddle = true;
			break;
		default:
			break;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		switch (state) {
		case GLUT_UP:
			if (MouseFlagRight) MouseFlagRight = false;
			break;
		case GLUT_DOWN:
			MouseFlagRight = true;
			if (x < window_size_x * 2 / 3) View_point_flag = true;
			else View_point_flag = false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//GLfloat direction[] = { 0.0, 1.0, 0.0 };
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//アルファの設定
	//glEnable(GL_BLEND);//アルファのブレンド有効
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
	//glLightfv(GL_LIGHTING, GL_SPOT_DIRECTION, direction);
	//glDisable(GL_LIGHT2);
	//glDisable(GL_LIGHT3);
	//glDisable(GL_LIGHT4);

	/*glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);*/

}

void reshape(int w, int h)
{
	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLfloat) w/ (GLfloat) h, 1.0, 1000.0 );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_size_x, window_size_y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutInitWindowPosition(0, 0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	//cout << MouseFlagRight << endl;
	init();
	glutMainLoop();

	return 0;

}