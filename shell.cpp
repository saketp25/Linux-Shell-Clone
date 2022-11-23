			#include<iostream>
			#include<stdio.h>
			#include<stdlib.h>
			#include<cstdlib>
			#include<string.h>			//FOR STRTOK
			#include<dirent.h>
			#include<unistd.h>			//chdir
			#include<fcntl.h>

			#define bufferSize 20
			#define delimeter " "
			#define stokBufferSize 20

			using namespace std;


			char *input()
			{
				char *line;
				int stringLength = bufferSize;
				int position =0;
				int c;

				// 	allocating memory dynamically

				line = (char*) malloc( stringLength*sizeof(char) );

				if(line == NULL){

					printf(" Error allocating memory \n");
					exit(EXIT_FAILURE);
				}

				//	looping to get the string from the user

				while(1)
				{
					c = getchar();

					if(c == EOF || c == '\n'){

						line[position] = '\0';
						return line;
					}


					line[position] = c;
					position++;


					//	if size execeeds then increasiing the size and reallocating

					if (position >= stringLength )
					{
						stringLength += bufferSize;

						line =(char*) realloc(line , stringLength);

						if (line == NULL){

							printf("Error allocating memory \n");
							exit(EXIT_FAILURE);

						}
					}

				}
			}

			char **splitLine(char* line)
			{

				int position = 0;
				int noOfStrings = stokBufferSize;
				char *string;
				char **strings;

				strings = (char**) malloc( sizeof(char*)*noOfStrings);

				if(strings == NULL)
				{
					printf("error allocating memory \n");
					exit(EXIT_FAILURE);
				}


				string = strtok(line , delimeter);

				while(string !=NULL)
				{
					strings[position] = string;
					position++;


					if( position >= noOfStrings ){

						noOfStrings += stokBufferSize;
						strings = (char**) realloc(strings, sizeof(char*) * noOfStrings );

						if(strings == NULL)
						{
							printf("error allocating memory \n");
							exit(EXIT_FAILURE);
						}
					}

					string  = strtok(NULL, delimeter);
				}

				strings[position] = NULL;
				return strings;
			}



			char *command_available[] = {
				"ls",
				"cd",
				"cat",
				"help",
				"exit"
			};


			int command_length()
			{
				return sizeof(command_available)/sizeof(char*);
			}


			int ls(char *argv[])
			{

				DIR *pdir = NULL;

				struct dirent *pent = NULL;

				pdir = opendir(argv[1]);

				if(pdir == NULL)
				{
					printf("file not opended \n");
					exit(1);
				}

				while(pent = readdir(pdir))
				{
					if(pent==NULL)
					{
						printf("error in reading file \n");
						exit(1);
					}

					printf("\n%s",pent->d_name);
				}
				printf("\n");
				closedir(pdir);

				return 1;
			}


			int cat(char **args)
			{
					if(args[1] == NULL)
					{
						printf("Enter file name\n");
						return 1;
					}

					int fo=open(args[1], O_RDONLY);
					if(fo==-1)
					{
						printf("invalid file\n");
						return 1;
					}

					char word[20];

					while( (read(fo,word,sizeof(word))) > 0 )
					{
						printf("%s",word);
					}

					return 1;
			}


			int cd(char **args)
			{

				if(args[1] == NULL)
				{
					printf("directory name ?\n");
				}
				else
				{
					if(chdir(args[1]) != 0)
					{
						printf("enter valid directory\n");
					}
				}


				return 1;
			}


			int help(char **args)
			{
				printf("Saket's Shell \n");
				printf("Type command name and arguments \n");
				printf("Working commands are : \n");

				for(int x=0;x<command_length();x++)
				{
					printf("%s ,",command_available[x]);
				}
				printf("\b \b \n");

				return 1;
			}


			int exit(char **args)
			{
				return 0;
			}


			int launch(char **args)
			{

				return 1;
			}


			int (*ar_pointer[])(char **) = { &ls , &cd , &cat , &help , &exit };


			int execute(char **args)
			{
				int x;

				if(args[0] == NULL)
				{
					return 1;
				}

				for(x=0;x<command_length();x++)
				{
					if(strcmp(args[0], command_available[x]) == 0 )
					{
						return (*ar_pointer[x])(args);
					}
				}

				return 1;	// return launch();
			}


			void menu(void)
			{

				char* line;
				char** args;
				int status,x;

				do
			 	{
			 		printf("-> ");
					line = input();
					args = splitLine(line);
					status = execute(args);

					free(line);
					free(args);

				}while(status);

			}



			int main(int argc, char const *argv[])
			{

				menu();

				return 0;
			}
