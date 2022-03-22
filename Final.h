/*Joanna Masikowska B9TB1710 - final project for Computer Seminar I, June 2021*/
#pragma once 
#ifndef FINAL_H

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

/*Font*/
#ifndef FONT_NAME
#define FONT_NAME "/ReggaeOne-Regular.ttf" 
#endif

#ifndef MAX_BUF
#define MAX_BUF 200
#endif


void currentDirectory(void);

/*Read map data from a file*/
int map_read(char *filename);

int search_cross(int crossing_number);

/*** CALCULATE THE DISTANCE ***/
/* Take ID of two crossings (if crossings are not sorted) and find the distance between two crossings.*/
double distance(int a, int b);


/***FIND SHORTEST DISTANCE***/
/*
 * Dijkstra algorithm;
 * Input -> number of crossings, ID of departure point, ID of destination point, array to store IDs of path
*/
int dijkstra(int crossing_number, int start, int goal, int path[]);


/***INTERFACE INSTRUCTIONS***/
/*Draw Text*/
FTGLfont *font;
char FONT_FILENAME[MAX_BUF];
void setupfont();

void outtextxy(double x, double y, char const *text);

/***DRAW MAP***/
/*Draw a circle*/
void circle(double x, double y, double r);

/*Draw a line*/
void line(double x0, double y0, double x1, double y1);

/*Draw every crossing as a circle and print its name; Connect neighbour crossings by line*/
void map_show(int crossing_number);

/*INPUT*/
int inputKeyboard(int start, int goal, int crossing_number);
#endif

