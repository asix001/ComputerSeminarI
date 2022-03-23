/*Joanna Masikowska B9TB1710 - final project for Computer Seminar I, June 2021*/

#include "Final.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <fnmatch.h> //UNIX standard, contains usleep()
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h> 
#include <GL/glfw.h>
#include <FTGL/ftgl.h>

#define CrossingNumber 100 /*Number of intersections*/
#define MaxName 50 /* Maximum 50 characters (single byte characters) */
#define Radius_Marker 0.2 

/*Font file*/
#ifndef FONT_NAME
#define FONT_NAME "/ReggaeOne-Regular.ttf" 
#endif

/*Buffer for the directory*/
#ifndef MAX_BUF
#define MAX_BUF 200
#endif

char currentPath[MAX_BUF];

void currentDirectory(void) 
{
	char path[MAX_BUF];	
	getcwd(path, MAX_BUF);
	strcpy(currentPath, path);
}

#define PathNumber 50
int path[PathNumber + 1];

/*Structurs for storing map data*/
typedef struct {
	double x, y; /* Position x, y */
} Position; /* Structure for a position */

typedef struct {
	int id; /* Intersection id number */
	Position pos; 
	double wait; /* Average wait time */
	char jname[MaxName]; /* Intersection name in Japanese */ 
	char ename[MaxName]; /* Intersection name in English*/
	int points; /* Number of cross roads */
	int next[5]; /* Adjacent intersection number */
	double distance;
} Crossing;

Crossing cross[CrossingNumber];

int map_read(char *filename)
{
	FILE *fp;
	int i, j;
	int crossing_number;        

	if ((fp = fopen(filename, "r")) == NULL) {
		exit(1);
	}
	fscanf(fp, "%d", &crossing_number); /* Read the number of crossings */
	for (i = 0; i < crossing_number; i++) {
		/* Write the crossings from file to a cross structure */
		fscanf(fp, "%d,%lf,%lf,%lf,%[^,],%[^,],%d",
			&(cross[i].id), &(cross[i].pos.x), &(cross[i].pos.y),
			&(cross[i].wait), cross[i].jname,
			cross[i].ename, &(cross[i].points));
		for (j = 0; j < cross[i].points; j++) {
			fscanf(fp, ",%d", &(cross[i].next[j]));
		}
	}
	fclose(fp);
	return crossing_number;
}


int search_cross(int crossing_number)
{
	int i;
	char buff[256], answer[50];
	int f = -1;      /* Flag set when item is found */

	printf("Input the name of the crossing->");
	scanf("%s", buff);
	for (i = 0; i < crossing_number; i++)  /* Loop until end of data */
	{
		if (strcmp(cross[i].ename, buff) == 0)  /*Exact match*/
		{
			f = i;
			printf("***Found: %s***\n", cross[i].ename);
		}
	}
	while (f == -1) {
		for (i = 0; i < crossing_number; i++) {
			if (strstr(cross[i].ename, buff) != NULL) /*Partial match*/
			{
				printf("Did you mean: %s?\n->Type 'Yes' or 'No':", cross[i].ename);
				scanf("%s", answer);
				if (strstr("YESYEsYesyes", answer) != NULL) {
					f = i;
					printf("%s was chosen\n", cross[i].ename);
					break;
				}
			}
			else if (i == (crossing_number - 1) && f == -1) {
				printf("Couldn't find any matching crossing\n");
				exit(1);
			}
		}
	}
	return f;
}


double distance(int a, int b)
{
	return hypot(cross[a].pos.x - cross[b].pos.x, cross[a].pos.y - cross[b].pos.y);
}


int dijkstra(int crossing_number, int start, int goal, int path[])
{
	int i, j;
	int U;  /* Current crossing's ID*/
	double min_distance;   /* A variable to search for the crossing with smallest distance */
	int visited[crossing_number];   /* Set Visited:1 or Not visited:0 to this flag */
	int prev[crossing_number];   /* To store parents */
	int neighbour;
	double dist;

	/* Set an unrealistically large distance value as the initial value */
	/* Set all crossings have not been visited */
	/* Set distance of starting point as 0 */
	for (j = 0; j < crossing_number; j++) {
		cross[j].distance = 10000000000;
		visited[j] = 0;
	}
	min_distance = 10000000000;
	cross[start].distance = 0;
	U = start;    /* Set current position at starting point */

	while (U != goal)  /* Loop until the destination is reached */
	{
		/* Update distances and parents (prev) of unvisited neighbors */
		for (j = 0; j < cross[U].points; j++)
		{
			neighbour = cross[U].next[j]; /*ID of node being currently checked*/
			dist = distance(U, neighbour) + cross[U].distance;/*Distance from start point to current node*/
			if (dist < cross[neighbour].distance) {
				cross[neighbour].distance = dist;
				prev[neighbour] = U;
			}
		}
		visited[U] = 1; /* Mark current crossing as visited */
		/* Select next crossing to visit (the one with the smallest distance) */
		for (j = 0; j < crossing_number; j++) {
			if (visited[j] != 1 && cross[j].distance < min_distance) {
				min_distance = cross[j].distance;
				U = j;
			}
		}
		min_distance = 1000000000;
	}
	/* Trace back the path from destination to origin using parents */
	path[0] = U; /* U = goal since it stopped there in previous while loop */
	i = 1;
	while (U != start)
	{
		path[i] = prev[U];
		U = prev[U];
		i++;
	}
	path[i++] = -1; /* To mark the end */
	return 0;
}


FTGLfont *font;
char FONT_FILENAME[MAX_BUF];


void setupfont() {
	currentDirectory();
	strcpy(FONT_FILENAME, CURRENT_PATH);
	strcat(FONT_FILENAME, FONT_NAME);
	font = ftglCreateExtrudeFont(FONT_FILENAME);
	if (font == NULL) {
		perror(FONT_FILENAME);
		fprintf(stderr, "Could not load font\n");
		exit(1);
	}
	ftglSetFontFaceSize(font, 24, 24);
	ftglSetFontDepth(font, 0.01);
	ftglSetFontOutset(font, 0, 0.1);
	ftglSetFontCharMap(font, ft_encoding_unicode);
}
void outtextxy(double x, double y, char const *text) {
	double const scale = 0.005;
	glPushMatrix();
	glTranslated(x, y, 0.0);
	glScaled(scale, scale, scale);
	ftglRenderFont(font, text, FTGL_RENDER_ALL);
	glPopMatrix();
}


void circle(double x, double y, double r)
{
	int const N = 12; /* Divide the circumference by 12 and draw in line segments */
	int i;

	glBegin(GL_LINE_LOOP);
	for (i = 0; i < N; i++)
		glVertex2d(x + cos(2 * M_PI * i / N) * r, y + sin(2 * M_PI * i / N) * r);
	glEnd();
}


void line(double x0, double y0, double x1, double y1)
{
	glBegin(GL_LINES);
	glVertex2d(x0, y0);
	glVertex2d(x1, y1);
	glEnd();
}


void map_show(int crossing_number)
{
	int i, j, k;
	double x0, y0, x1, y1; /*Coordinates*/
	/*Loop going through all of the crossings*/
	for (i = 0; i < crossing_number; i++) {
		x0 = cross[i].pos.x;
		y0 = cross[i].pos.y;
		glColor3d(1.0, 0.5, 0.5);
		circle(x0, y0, 0.09);
		glColor3d(1.0, 1.0, 0.0);
		outtextxy(x0, y0, cross[i].ename);
		glColor3d(0.1, 1.0, 1.0);

		for (j = 0; j < cross[i].points; j++) {
			k = cross[i].next[j];
			x1 = cross[k].pos.x;
			y1 = cross[k].pos.y;
			line(x0, y0, x1, y1);
		}
	}
}


int inputKeyboard(int start, int goal, int crossing_number) {
	printf("Start point: ");
	start = search_cross(crossing_number);
	printf("Destination point: ");
	goal = search_cross(crossing_number);
	printf("Start:%s Goal:%s\n", cross[start].ename, cross[goal].ename);
	if (start < crossing_number && goal < crossing_number && start >= 0 && goal >= 0)
	{
		dijkstra(crossing_number, goal, start, path); /*Find shortest path*/
	}
	return 0;
}

int main(void)
{
	int crossing_number; /* Number of intersections */
	int vehicle_pathIterator = 0; // Indicates the portion of the path we are moving (n)
	int vehicle_stepOnEdge = 0; // Tracks progress in the current stretch (n to n+1)
	double vehicle_x = 0.0, vehicle_y = 0.0; //Store x,y positions of the marker
	float X = 0, Y = 0, Z = 0;
	int i;
	char choice[50];


	/*Define variables for the projection matrix*/
	float ORIGIN_X = -3.0;
	float ORIGIN_Y = 0.0;
	float REAL_SIZE_X = 20.0;  /*Size of x-axis*/
	float REAL_SIZE_Y = 20.0; /*Size of y-axis*/

	/*Viewport size*/
	int width, height;
	/*Set up the path*/
	int start = -1, goal = -1;

	/* Read a file */
	crossing_number = map_read("map2.dat");
	printf("Loaded %d crossings\n", crossing_number);

	/*Welcome message&settings*/
	printf("Welcome to the Sendai navigation system.\nWould you like to see the list of registered crossings?\n->Type 'Yes' or 'No': ");
	scanf("%s", choice);
	if (strstr("YESYEsYesyes", choice) != NULL) {
		for (i = 0; i < crossing_number; i++) {
			printf("%d.%s (jap.%s)\n", cross[i].id, cross[i].ename, cross[i].jname);
		}
	}

	/*Input*/
	inputKeyboard(start, goal, crossing_number);

	/*** Initialize the graphic environment and open a window ***/
	glfwInit();
	glfwOpenWindow(1000, 800, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);
	setupfont(); /*Set up the font*/

	for (;;) {
		
		X = ORIGIN_X - vehicle_x;
		Y = ORIGIN_Y - vehicle_y;
		while (REAL_SIZE_X > 7) {
			REAL_SIZE_X *= 0.99;
			REAL_SIZE_Y *= 0.99;
		}
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity(); /*Initialize the matrix*/
		glOrtho(ORIGIN_X + REAL_SIZE_X * -0.5, ORIGIN_X + REAL_SIZE_X * 0.5, ORIGIN_Y + REAL_SIZE_Y * -0.5, ORIGIN_Y + REAL_SIZE_Y * 0.5, -1.0, 1.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(X, Y, Z);

		/* End when Esc is pressed or the window is closed */
		if (glfwGetKey(GLFW_KEY_ESC) || !glfwGetWindowParam(GLFW_OPENED))
			break;

		glfwGetWindowSize(&width, &height); /* Get the size of the current window */
		glViewport(0, 0, width, height); /* Set the whole window as a viewport */
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT); /* Fill the back buffer with black */
		map_show(crossing_number);

		/*Motion of the moving object*/
		if (path[vehicle_pathIterator + 0] != -1 && path[vehicle_pathIterator + 1] != -1) {
			double x0 = cross[path[vehicle_pathIterator + 0]].pos.x;
			double y0 = cross[path[vehicle_pathIterator + 0]].pos.y;
			double x1 = cross[path[vehicle_pathIterator + 1]].pos.x;
			double y1 = cross[path[vehicle_pathIterator + 1]].pos.y;
			double distance = hypot(x1 - x0, y1 - y0);
			int steps = (int)(distance / 0.1);
			vehicle_stepOnEdge++;
			vehicle_x = x0 + (x1 - x0) / steps * vehicle_stepOnEdge;
			vehicle_y = y0 + (y1 - y0) / steps * vehicle_stepOnEdge;
			if (vehicle_stepOnEdge >= steps) {
				vehicle_pathIterator++;
				vehicle_stepOnEdge = 0;
			}
		}
		/*Display the moving object and the current position*/
		glColor3d(1.0, 0.0, 0.0); //red
		circle(vehicle_x, vehicle_y, Radius_Marker);

		glfwSwapBuffers(); /* Swap the front buffer with the back buffer */
		usleep(50 * 1000); /* Wait for about 50 millisecond */
	}
	glfwTerminate();
	return 0;
}
