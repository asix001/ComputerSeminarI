#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef MAX_BUF
#define MAX_BUF 200
#endif

char CURRENT_PATH[MAX_BUF];

void currentDirectory(void) 
{
	char path[MAX_BUF];
	//char *font;
	
	getcwd(path, MAX_BUF);
	//font = "\ReggaeOne-Regular.ttf";
	
	//char dir[MAX_BUF];//+sizeof(font)];
	
	//strcpy(dir, path);
	//strcat(dir, font);
	strcpy(CURRENT_PATH, path);
	
}



