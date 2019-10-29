#include "IOStream.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

char* occurrences(char* string_given, char* line, int line_count, int line_length)
{
	cout << "OCCURRENCE" << line << endl;
	char * pch;
	bool found = false;
	char * return_string = new char[line_count*line_length+line_count];
	char** return_file = new char*[line_count];
	char** file = new char*[line_count];
	for(int i = 0; i < strlen(line); ++i)
	{
	     if(line[i] >= 'A' && line[i] <= 'Z')
	     {
		 line[i] = (line[i] + 32);
             }
	}
	for(int i = 0; i < line_count; ++i)
	{
		file[i] = new char[line_length];
		return_file[i] = new char[line_length];
	}
	//printf("Splitting \"%s\" \n", line);
	pch = strtok(line, "\n");
	for(int i = 0; pch != NULL; ++i)
	{
		//cout << "HERE" << endl;
		strcpy(*(file+i), pch);
		pch = strtok(NULL, "\n");
	}
	char* temp = new char[line_length];
	//Alphabetical sort
	/*for(int i = 0; i < line_count; ++i)
	{
		for(int j = 0; j < line_count; ++j)
		{
			if(strcmp(file[i],file[j])>0)
			{
				strcpy(temp, *(file+i));
				strcpy(*(file+i),*(file+j));
				strcpy(*(file+j),temp);
			}
		}
	}*/
	//cout << "linelength-stringlength " << line_length-strlen(string_given) << endl;
	cout << "LOOKING FOR OCCURRENCES" << endl;
	int occurrences = 0;
	for(int i = 0; i < line_count; ++i)
	{
		for(int k = 0; k < (line_length-strlen(string_given)); ++k)
		{
			int j = 0;
			while ((j < (strlen(string_given))) && 
			      (string_given[j]==file[i][k+j])){
					//cout << "COMP: " << string_given[j] << " "
				     	//<< "FILE I K+J" << i << k+j << file[i][k+j] << endl;
					++j;
			}
		if(j == strlen(string_given))
		{
			if((file[i][k-1] < 'A' || file[i][k-1] > 'z')
			&& ((file[i][k+j+1] < 'A') || (file[i][k+j+1] > 'z'))) 
			{
				//cout << "RETURN:" << return_string << endl;
				strcat(return_file[occurrences], strcat(file[i],"\n"));
				++occurrences;
				found = true;
			}
		}
		}
	}
	char* temp2 = new char[occurrences];
	cout << "ALPHABETIZING OCCURRENCES" << endl;
	for(int i = 0; i < occurrences; ++i)
	{
		for(int  j = i+1; j < occurrences; ++j)
		{
			if(strcmp(return_file[i], return_file[j])>0)
			{
				strcpy(temp2, return_file[i]);
				strcpy(return_file[i], return_file[j]);
				strcpy(return_file[j], temp2);
			}
		}
	}
	cout << "ADDING STRING TO RETURN STRING" << endl;
	cout << "OCCURRENCES: " << occurrences << endl;
	for(int i = 0; i < occurrences; ++i)
	{
		strcat(return_string, return_file[i]);
	}
	if(found){
		cout << "OCCURRENCE CODE FINISHED" << endl;
		return return_string;
	} else {
		return "not found";
	}
};


int main(int argc, char** argv)

{	
	int sv[2];
	char* buf;

	if(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1)
	{
		perror("socketpair");
		exit(1);
	}

	if(!fork()) // child
	{
		int line_length;
		int line_count;
		int size_of_string;
		int size_of_file;
		read(sv[1], &size_of_string, sizeof(int));
		read(sv[1], &size_of_file, sizeof(int));
		char* string_searched_for = new char[size_of_string];
		char* file_as_string = new char[size_of_file];
		read(sv[1], string_searched_for, size_of_string);
		cout << "SEARCHED FOR: " << string_searched_for << endl;
		read(sv[1], &line_count, sizeof(int));
		read(sv[1], &line_length, sizeof(int));
		//for(int i=0; i < line_count; ++i)
		read(sv[1], file_as_string, (line_count*line_length)+line_count);
		//cout << "BUF: " << file_as_string << endl;
		//cout << "CHILD READ: " << buf << endl;
		char* returnChar = occurrences(string_searched_for, buf, line_count, line_length);
		//cout << returnChar << endl;
		write(sv[1], returnChar, (line_length*line_count)+line_count);
		//printf("child: sent file\n");
	} else { // parent 
		//cout << (file_line_length*file_line_count) << endl;
		//INPUT
		string searched_for = argv[2];
		char* filename = argv[1];
		//cin >> filename;
		IOStream new_file(filename);
		//cin >> searched_for;
		cout << searched_for << endl;
		// WRITING FILE
		int size_string = searched_for.size();
		//cout << "here" << endl;
		new_file.ReadFile();
		//cout << "here now " << endl;
		int file_line_count = new_file.getCharLineCount();
		int file_line_length = new_file.getCharLineLength();
		int file_size = (file_line_count*file_line_length)+file_line_count;
		char** file = new_file.getCharArray();
		//buf = new char[file_size];
		cout << "FILE SIZE:" << file_size << endl;
		char* file_as_string = new char[file_size];
		//ADDS FILE TO FILE AS STRING
		//strcpy(file_as_string, *(file+0));
		//strcat(file_as_string,"\n");
		for(int i = 0; i < file_line_count; ++i)
		{
			strncat(file_as_string, *(file+i), file_line_length);
			strcat(file_as_string,"\n");
		}
		strcat(file_as_string, "\0");
		int size_of_file = file_size;
		buf = new char[size_of_file];
		cout << file_as_string << endl;
		//WRITES FILE LENGTH and STRING SEARCHED FOR
		write(sv[0], &size_string, sizeof(int));
		write(sv[0], &file_size, sizeof(int));
		write(sv[0], searched_for.c_str(), size_string);
		//write(sv[0], searched_for, size_string);
		write(sv[0], &file_line_count, sizeof(int));
		write(sv[0], &file_line_length, sizeof(int));
		//WRITES FILE LINE BY LINE
		//for(int i = 0; i < file_line_count; ++i)
		//{
		//cout << file_as_string << endl;
		//cout << file_size+file_line_count << endl;
		write(sv[0], file_as_string, file_size);
		//cout << "PARENT SENT: " << file_as_string << endl;
		//}
		//printf("parent: sent file\n");
		//Output
		read(sv[0], buf, file_size);
		printf("parent: read file\n");
		cout << "LINES FOUND AT: \n" << buf << endl;
		wait(NULL);
	}
	


}
