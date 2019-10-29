#include "IOStream.h"


IOStream::IOStream()
{
	filename = "text.txt";

}
IOStream::IOStream(char* new_filename)
{
	filename = new_filename;
	character_array;
	char_line_count;
	char_line_length;	
}
void IOStream::ReadFile()
{
	//cout << filename << endl;
	ifstream file(filename);
	//char** return_array;
	string line;
	int line_count = 0;
	int max_line_length = 0;
	while(getline(file, line))
	{
		int current_line_length = line.length();
		if(max_line_length < current_line_length)
			max_line_length = current_line_length;
		//cout << line_count << endl;
		++line_count;
	}
	//cout << "MAX LENGTH" << max_line_length << endl;
	//cout << "NUM OF LINES: " << line_count << endl;
	character_array = new char*[line_count];
	for(int i=0; i < line_count; ++i)
	{
		character_array[i] = new char[max_line_length+1];
	}
	file.clear();
	file.seekg(0);

	int index = 0;

	while(getline(file, line))
	{
		int line_length = line.length();
		strcpy(*(character_array+index), line.c_str());
		//cout << *(character_array+index) << endl;
		++index;
	}
	for(int i=0; i < line_count; ++i)
	{
		//cout << "MAX LINE LENGTH: " << max_line_length+1 << endl;
		character_array[i][max_line_length+1] = '\0'; 
	}
	
	char_line_length = max_line_length+1;
	char_line_count = line_count;
	
}
char** IOStream::getCharArray()
{
	return character_array;
}
int IOStream::getCharLineLength()
{
	return char_line_length;
}
int IOStream::getCharLineCount()
{
	return char_line_count;
}
char IOStream::getCharAt(int i, int j)
{
	return character_array[i][j];
}
