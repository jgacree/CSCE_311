nclude "IOStream.h"
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
	char * pch;
		bool found = false;
			char * return_string = new char[line_count*line_length+line_count];
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
																	}
																		//printf("Splitting \"%s\" \n", line);
																			pch = strtok(line, "\n");
																				for(int i = 0; pch != NULL; ++i)
																					{
																							//cout << "HERE" << endl;
																									strcpy(*(file+i), pch);
																											pch = strtok(NULL, "\n");
																												}
																													//cout << "linelength-stringlength " << line_length-strlen(string_given) << endl;
																														for(int i = 0; i < line_count; ++i)
																															{
																																	for(int k = 0; k < (line_length-strlen(string_given)); ++k)
																																			{
																																						int j = 0;
																																									while ((j < (strlen(string_given))) && 
																																												      (string_given[j]==file[i][k+j])){
																																												      					//cout << "COMP: " << string_given[j] << " "
																																																					     	//<< file[i][k+j] << endl;
																																																											++j;
																																																														}
																																																																if(j == strlen(string_given))
																																																																		{
																																																																					if((file[i][k-1] < 'A' || file[i][k-1] > 'z')
																																																																								&& ((file[i][k+j+1] < 'A') || (file[i][k+j+1] > 'z'))) 
																																																																											{

																																																																															strcat(return_string, strcat(file[i],"\n"));
																																																																																			found = true;
																																																																																						}
																																																																																								}
																																																																																										}
																																																																																											}
																																																																																												if(found){
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
																																																																																																																					read(sv[1], &size_of_string, sizeof(int));
																																																																																																																							char* string_searched_for = new char[size_of_string];
																																																																																																																									read(sv[1], string_searched_for, size_of_string);
																																																																																																																											//cout << "SEARCHED FOR: " << string_searched_for << endl;
																																																																																																																													read(sv[1], &line_count, sizeof(int));
																																																																																																																															read(sv[1], &line_length, sizeof(int));
																																																																																																																																	//for(int i=0; i < line_count; ++i)
																																																																																																																																			read(sv[1], buf, (line_length*line_count)+line_count);
																																																																																																																																					//cout << "CHILD READ: " << buf << endl;
																																																																																																																																							char* returnChar = occurrences(string_searched_for,buf,line_count,line_length);
																																																																																																																																									//cout << returnChar << endl;
																																																																																																																																											write(sv[1], returnChar, (line_length*line_count)+line_count);
																																																																																																																																													//printf("child: sent file\n");
																																																																																																																																														} else { // parent 
																																																																																																																																																//cout << (file_line_length*file_line_count) << endl;
																																																																																																																																																		//INPUT
																																																																																																																																																				string searched_for;
																																																																																																																																																						string filename;
																																																																																																																																																								cout << "PLEASE ENTER FILE NAME" << endl;
																																																																																																																																																										cin >> filename;
																																																																																																																																																												IOStream new_file(filename);
																																																																																																																																																														cout << "PLEASE ENTER STRING SEARCHED FOR" << endl;
																																																																																																																																																																cin >> searched_for;
																																																																																																																																																																		cout << searched_for << endl;
																																																																																																																																																																				// WRITING FILE
																																																																																																																																																																						int size_string = searched_for.size();
																																																																																																																																																																								new_file.ReadFile();
																																																																																																																																																																										int file_line_count = new_file.getCharLineCount();
																																																																																																																																																																												int file_line_length = new_file.getCharLineLength();
																																																																																																																																																																														int file_size = (file_line_count*file_line_length)+file_line_count;
																																																																																																																																																																																char** file = new_file.getCharArray();
																																																																																																																																																																																		buf = new char[file_size];
																																																																																																																																																																																				char file_as_string[file_size+file_line_count];
																																																																																																																																																																																						//ADDS FILE TO FILE AS STRING
																																																																																																																																																																																								for(int i = 0; i < file_line_count; ++i)
																																																																																																																																																																																										{
																																																																																																																																																																																													strncat(file_as_string, *(file+i), file_line_length);
																																																																																																																																																																																																strcat(file_as_string, "\n");
																																																																																																																																																																																																		}
																																																																																																																																																																																																				//WRITES FILE LENGTH and STRING SEARCHED FOR
																																																																																																																																																																																																						write(sv[0], &size_string, sizeof(int));
																																																																																																																																																																																																								write(sv[0], searched_for.c_str(), size_string);
																																																																																																																																																																																																										//write(sv[0], searched_for, size_string);
																																																																																																																																																																																																												write(sv[0], &file_line_count, sizeof(int));
																																																																																																																																																																																																														write(sv[0], &file_line_length, sizeof(int));
																																																																																																																																																																																																																//WRITES FILE LINE BY LINE
																																																																																																																																																																																																																		//for(int i = 0; i < file_line_count; ++i)
																																																																																																																																																																																																																				//{
																																																																																																																																																																																																																						write(sv[0], file_as_string, file_size);
																																																																																																																																																																																																																								//cout << "PARENT SENT: " << file_as_string << endl;
																																																																																																																																																																																																																										//}
																																																																																																																																																																																																																												//printf("parent: sent file\n");
																																																																																																																																																																																																																														//Output
																																																																																																																																																																																																																																read(sv[0], buf, file_size);
																																																																																																																																																																																																																																		printf("parent: read file\n");
																																																																																																																																																																																																																																				cout << "LINES FOUND AT:" << buf << endl;
																																																																																																																																																																																																																																						wait(NULL);
																																																																																																																																																																																																																																							}
																																																																																																																																																																																																																																								


																																																																																																																																																																																																																																								}

