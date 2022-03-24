/*Joanna Masikowska B9TB1710 - final project for Computer Seminar I, June 2021*/
#pragma once 

#ifndef FINAL_H
#define FINAL_H

#define CrossingNumber 100 /*Number of intersections*/
#define MaxName 50 /* Maximum 50 characters (single byte characters) for the crossing name*/
#define Radius_Marker 0.2 

/*Font*/
#ifndef FONT_NAME
#define FONT_NAME "/ReggaeOne-Regular.ttf" 
#endif

/*Buffer for the directory*/
#ifndef MAX_BUF
#define MAX_BUF 200
#endif

/*Define path to find the font file*/
void currentDirectory(void);


/*Read map data from a file*/
int map_read(char *filename);


/*
 * Find a crossing  in the database of size crossing_number by its name.
 * Return 0 if the crossing is found. Terminate program if the crossing is not found.
 */
int search_cross(int crossing_number);


/* Take ID of two crossings (if crossings are not sorted) and find the distance between two crossings.*/
double distance(int a, int b);


/*
 * Implementation of Dijkstra algorithm to find the shortest distance. 
 * Input -> number of crossings in the database, ID of departure point, ID of destination point, array to store IDs of path
*/
int dijkstra(int crossing_number, int start, int goal, int path[]);


/***INTERFACE INSTRUCTIONS***/
//FTGLfont *font;
//char FONT_FILENAME[MAX_BUF];

/*Find font file and set up the font*/
void setupfont();

/*Draw text in the specified location (x,y)*/
void outtextxy(double x, double y, char const *text);


/*Draw a circle*/
void circle(double x, double y, double r);


/*Draw a line*/
void line(double x0, double y0, double x1, double y1);


/*Draw every crossing as a circle and print its name; Connect neighbour crossings by line*/
void map_show(int crossing_number);


/*Take name of a crossing as a user input (string) and find the crossing in the database*/
int inputKeyboard(int start, int goal, int crossing_number);

#endif

