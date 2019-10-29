/*********************************************************************************
 * Name: Joshua Acree
 * Function: IOSTREAM
 *
 *
 *
 * *******************************************************************************/
#include <string>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

class IOStream {
	public:
	IOStream();
	IOStream(char*);
	void ReadFile();
	char** getCharArray();
	int getCharLineCount();
	int getCharLineLength();
	char getCharAt(int,int);
	
	private:
	char** character_array;
	char* filename;
	int char_line_count;
	int char_line_length;



};	
