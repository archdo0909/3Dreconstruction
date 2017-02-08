#include <iostream>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <math.h>

int max_x= 40;
int max_y= 40;
int max_z= 100;

int p = 2*max_x + 1;
int q = 2*max_y + 1;
int r = max_z;
int window_size_x = 500;
int window_size_y = window_size_x;
//double View_from[3] = {-8.0 ,8.0, 0.0};
double View_from[3] = {-10.0, 10.0, 5.0};
//double View_to[3]= {0.0, -10.0, 0.0};
double View_to[3] = {10.0, -10.0, 0.0};

////change

using namespace std;

// vector
typedef struct Vector3D{
	double x;
	double y;
	double z;
	int vol;
} point;
static point voxel_position[70][70][100];

struct Energy_deposit{
	double energy;
};
// length of vector
double get_vector_length (point v)
{
	return pow( (v.x * v.x) + (v.y * v.y) + (v.z * v.z), 0.5 );
}

// vector inner product
double inner_product(point vl, point vr)
{
	return vl.x * vr.x + vl.y * vr.y + vl.z * vr.z;
}

double Angle_vector(point A, point B)
{
	double length_A = get_vector_length(A);
	double lenght_B = get_vector_length(B);

	double cos_theta = inner_product(A, B) / (length_A * length_A);

	double theta = acos(cos_theta);
	// to make degree

	// theta_deg = theta * 180 / PI;

	return theta;
}
point compose_vector(point A, point B)
{
	point C;
	C.x = B.x - A.x;
	C.y = B.y - A.y;
	C.z = B.z - A.z;

	return C;
}
double scattering_angle(Energy_deposit A, Energy_deposit B)
{
	double Angle;

	Angle = acos(1 + 0.511*(1/(A.energy + B.energy) - 1 / (B.energy)));
	return Angle;

}
void Reconstruction()
{
	point d1[1];
	point d2[1];
	struct Energy_deposit d1_dep[1];
	struct Energy_deposit d2_dep[1];

	int i, j, k, z; 

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
	cout << "insert_info" << endl;
	for(i = 0; i < p ; i++)
	{
		for (j = 0; j < q; j++)
		{
			for(k=0; k < r; k++)
			{
				voxel_position[i][j][k].x=(double) (i-max_x);
				voxel_position[i][j][k].y=(double) (j-max_y);
				voxel_position[i][j][k].z=(double) k;
				voxel_position[i][j][k].vol=0;
			}
		}
	}
	cout << "insert_coordinates" << endl;

    int cnt=0;

	for(i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			for(z = 0; z < 1; z++){
				for(k = 0; k < 1; k++){
					if(Angle_vector( compose_vector(d2[k],d1[k]), compose_vector(d1[k], voxel_position[i][j][z])) <= 
						acos(0.5/get_vector_length(compose_vector(d1[k], voxel_position[i][j][z]))) + scattering_angle(d1_dep[k], d2_dep[k]) &&
						Angle_vector( compose_vector(d2[k],d1[k]), compose_vector(d1[k], voxel_position[i][j][z])) >= 
						scattering_angle(d1_dep[k], d2_dep[k]) - acos(0.5/get_vector_length(compose_vector(d1[k], voxel_position[i][j][z]))) ){
						voxel_position[i][j][z].vol = voxel_position[i][j][z].vol + 1;
						cnt++;
					//cout << Angle_vector( compose_vector(d2[k],d1[k]), compose_vector(d1[k], voxel_position[i][j][z])) << endl;
					}
				}
			}
		}
	}
	cout<<"done1"<<endl;

}

void voxel_simulation()
{

	//point d1[1];
	//point d2[1];
	//struct Energy_deposit d1_dep[1];
	//struct Energy_deposit d2_dep[1];

	int i, j, k, z;
	int cnt=0;

	Reconstruction();

	//for(i = 0; i < 1; i++)
	//{
	//	d1[i].x = 0.0;
	//	d1[i].y = 0.0;
	//	d1[i].z = 0.0;

	//	d2[i].x = 0.0;
	//	d2[i].y = 0.0;
	//	d2[i].z = -80.0;

	//	d1_dep[i].energy = 0.22;
	//	d2_dep[i].energy = 0.44;

	//}
	//for(i = 0; i < p; i++)
	//{
	//	for (j = 0; j < q; j++)
	//	{
	//		for(k=0; k < r; k++)
	//		{
	//			voxel_position[i][j][k].x=i-max_x;
	//			voxel_position[i][j][k].y=j-max_y;
	//			voxel_position[i][j][k].z=k;
	//			voxel_position[i][j][k].vol=0;
	//		}
	//	}
	//}

	//for(i = 0; i < 10; i++){
	//	for (j = 0; j < 10; j++){
	//		for(z = 0; z < 1; z++){
	//			for(k = 0; k < 1; k++){
	//				if(Angle_vector( compose_vector(d2[k],d1[k]), compose_vector(d1[k], voxel_position[i][j][z])) <= 
	//					acos(0.5/get_vector_length(compose_vector(d1[k], voxel_position[i][j][z]))) + scattering_angle(d1_dep[k], d2_dep[k]) &&
	//					Angle_vector( compose_vector(d2[k],d1[k]), compose_vector(d1[k], voxel_position[i][j][z])) >= 
	//					scattering_angle(d1_dep[k], d2_dep[k]) - acos(0.5/get_vector_length(compose_vector(d1[k], voxel_position[i][j][z]))) ){
	//						voxel_position[i][j][z].vol = voxel_position[i][j][z].vol + 1;
	//						cnt++;
	//						//cout << Angle_vector( compose_vector(d2[k],d1[k]), compose_vector(d1[k], voxel_position[i][j][z])) << endl;
	//				}
	//			}
	//		}
	//	}
	//}

	cout << "complete1" << endl;

	for(i = 0; i < 10; i++){
		for(j = 0; j< 10; j++){
			for(k = 0; k < 1; k++){
				if(voxel_position[i][j][k].vol > 0){
					cnt++;
					glColor3f(0.0, 0.5, 0.8);
					glPushMatrix();
					glTranslated((GLdouble)voxel_position[i][j][k].x, (GLdouble)voxel_position[i][j][k].z, (GLdouble)voxel_position[i][j][k].y);
					glutSolidCube(1);
					glPopMatrix();
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
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glEnable(GL_DEPTH_TEST);
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
	gluPerspective(50.0, (GLfloat) w/ (GLfloat) h, 1.0, 20.0 );
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

	init();
	glutMainLoop();

	return 0;

}