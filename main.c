#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "gfx.h"
#include "main.h"

int ysize = 1000;
int xsize = 1000;

typedef struct t_Point {
	double x;
	double y;
	double z;
} Point;

typedef struct t_Connection {
	int p1;
	int p2;
} Connection;

typedef struct t_Plane {
	Point origin;
	Point i_hat;
	Point j_hat;
} Plane;

void accept_inputs(int*);
Point* project_points(Point*, int /* length */, Plane /* ypos for plane */);
void render_points(Point*, int /* length */, Connection*, int);


Point P_multiply(Point p, double scalar) {
	Point q;
	q.x = p.x * scalar;
	q.y = p.y * scalar;
	q.z = p.z * scalar;

	return q;
}

Point P_cross(Point p, Point q) {
	Point r;
	r.x = p.y*q.z - p.z*q.y;
	r.y = p.z*q.x - p.x*q.z;
	r.z = p.x*q.y - p.y*q.x;

	return r;
}

double P_dot(Point p, Point q) {
	return p.x * q.x + p.y * q.y + p.z * q.z;
}

double P_length(Point p) {
	return sqrt((p.x * p.x) + (p.y * p.y) + (p.z * p.z));
}

Point P_normalise(Point p) {
	Point r;

	double length = P_length(p);
	r.x = p.x / length;
	r.y = p.y / length;
	r.z = p.z / length;

	return r;
}

void accept_inputs(int* flags) {
	return;
}


Point* project_points(Point* points, int length, Plane plane) {
	//TODO: Rework this
	Point* res = malloc(sizeof(Point) * length);

	Point unit;
	for (int i = 0; i < length; i++) {
		unit = P_normalise(points[i]);
		Point p;
		p.z = (unit.z * (plane.origin.y - points[i].y) / unit.y) + points[i].z;
		p.x = (unit.x * (plane.origin.y - points[i].y) / unit.y) + points[i].x;
		res[i] = p;
	}
	return res;
}

void render_points(Point* points, int point_length, Connection* connections, int connection_length) {
	for (int i = 0; i < connection_length; i++) {
		Point p1 = points[connections[i].p1];
		Point p2 = points[connections[i].p2];

		// printf("(%lf, %lf)\n", (p1.x * xsize) + (xsize / 2), p1.z * ysize);
		gfx_color(255, 255, 255);
		gfx_line((p1.x * xsize) + (xsize / 2), (p1.z * ysize) + (ysize / 2), (p2.x * xsize) + (xsize / 2), (p2.z * ysize) + (ysize / 2));
	}
}

int main() {

	// TODO: have connections between points, so can render selectively

	gfx_open(xsize, ysize, "Testing");

	size_t point_length = 8;
	Point* points = malloc(sizeof(Point) * point_length);

	points[0] = (Point) {  2, 7,  2 };
	points[1] = (Point) { -2, 7,  2 };
	points[2] = (Point) { -2, 7, -2 };
	points[3] = (Point) {  2, 7, -2 };
	points[4] = (Point) {  2, 11, 2 };
	points[5] = (Point) { -2, 11, 2 };
	points[6] = (Point) { -2, 11,-2 };
	points[7] = (Point) {  2, 11,-2 };

	size_t connection_length = 12;
	Connection* connections = malloc(sizeof(Connection) * connection_length);
	connections[0] = (Connection)  { 0, 1 };
	connections[1] = (Connection)  { 0, 3 };
	connections[2] = (Connection)  { 0, 4 };
	connections[3] = (Connection)  { 1, 2 };
	connections[4] = (Connection)  { 1, 5 };
	connections[5] = (Connection)  { 2, 3 };
	connections[6] = (Connection)  { 2, 6 };
	connections[7] = (Connection)  { 3, 7 };
	connections[8] = (Connection)  { 4, 5 };
	connections[9] = (Connection)  { 4, 7 };
	connections[10] = (Connection) { 5, 6 };
	connections[11] = (Connection) { 6, 7 };


	gfx_clear_color(0, 0, 0);
	gfx_clear();

	Plane render_plane = {
		(Point) { 0, 1, 0 },
		(Point) { 1, 0, 0 },
		(Point) { 0, 0, 1 }
	};


	double counter;
	while (1) {
		accept_inputs(0);

		Point* projected = project_points(points, point_length, render_plane);
		render_points(projected, point_length, connections, connection_length);

		gfx_flush();
		usleep((1.0 / 60.0) * 1000000);

		printf("%lf\n", render_plane.origin.y);

		gfx_clear();
		// printf("%lf\n", sin(counter / 10));
		counter++;
	}

	return 0;
}
