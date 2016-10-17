/**
  * ------------------------- PROJECT ASSIGNMENT 1---------------------------
  * @author Sneha Dipankar Roy (sr0069@uah.edu)
  * @class CS590-01 Summer 2016
  * @date 28th June 2016
  * @desc C program to copy files (mycp) using the file I/O routines on Linux
  *--------------------------------------------------------------------------
  */

// Header files
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define BUFFSIZE 4096

int main( int argc, char *argv[] ) {
	// Variable declaration 
	int n, i;
	char buffer[BUFFSIZE];
	int fd1, fd2;
	char usrip;
	char path[30] = "";
	int acc, acc2;

	/*-------------------------Copy one file to another file-------------------------*/
	if (argc == 3) {
		strcat(path, argv[2]);
        	strcat(path, "/");
        	strcat(path, argv[1]);
		// Opens source file in read-only mode
		fd1 = open(argv[1], O_RDONLY );
		
		// Checks whether source file exists
		if (fd1 == -1) {
			printf("%s : Source file does not exist. \n", argv[1]);
			return 1;
		}

		// Checks whether source file and target file are same
		if (strcmp(argv[1],argv[2]) == 0) {
			printf("%s : Source file and %s : target file are the same. Can't copy a file to itself. \n", argv[1], argv[2]);
			return 1;
			}

		// Checks whether target file is a directory
		if (open(argv[2], O_DIRECTORY) == -1 ) {
			/* Opens target file in write-only mode
			   Creates target file in Read-Write-Execute mode if it does not exist 
			   Generates an error if target file already exists */
			fd2 = open(argv[2], O_CREAT | O_WRONLY | O_EXCL, S_IRWXU); 
			if (fd2 != -1) {
				while ((n = read(fd1, buffer, BUFFSIZE)) > 0 ) {
					if (write(fd2, buffer, n) != n)
						printf("Write Error! \n");
				}
				printf("%s : File is copied to %s. \n", argv[1], argv[2]);
				return 0;
			}	
			else {	// fd2 == -1
				// Checks whether user wants to overwrite the existing file
				printf("%s : Target file already exists. Do you want to overwrite the file? (y/n) ", argv[2]);
				scanf("%c", &usrip);
				if ( usrip == 'y' ) {
					fd2 = open(argv[2], O_WRONLY | O_TRUNC);
					while ((n = read(fd1, buffer, BUFFSIZE)) > 0 ) {
        	                        	if (write(fd2, buffer, n) != n)
        	                                	printf("Write Error! \n");
					}
					printf("%s : File is copied to %s. \n", argv[1], argv[2]);
					return 0;
				}
				else {	// usrip != 'y'
					printf("%s : Target file is not overwritten. \n", argv[2]);
					return 1;
				}
			}	
		}
		else { //open(argv[2], O_DIRECTORY) != -1
			// Checks whether user has permissions to write to target directory
			acc = access(argv[2], W_OK);
			if (acc == 0) {
				//Checks whether source file already exists in target directory
				acc2 = access(path, F_OK);
        	        	if (acc2 == -1) {
        	                	fd2 = open(path, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
        	                	while ((n = read(fd1, buffer, BUFFSIZE)) > 0 ) {
        	                		if (write(fd2, buffer, n) != n)
        	                        		printf("Write Error! \n");
        	                	}
					printf("%s : File is copied to %s. \n", argv[1], argv[2]);
					return 0;	
        	        	}
        	        	else { //acc2 != -1
					// Checks whether user wants to overwrite the existing file
        	                	printf("%s : Source file already exists in the directory. Do you want to overwrite the file? (y/n) ", argv[1]);
        	                	scanf("%c", &usrip);
        	                	if ( usrip == 'y' ) {
        	                        	fd2 = open(path, O_WRONLY | O_TRUNC);
        	                        	while ((n = read(fd1, buffer, BUFFSIZE)) > 0 ) {
        	                                	if (write(fd2, buffer, n) != n)
        	                                        	printf("Write Error! \n");
        	                        	}
						printf("%s : File is copied to %s. \n", argv[1], argv[2]);
        	                        	return 0;
        	                	}
        	                	else {  //usrip != 'y'
        	                        	printf("%s : File is not copied to %s. \n", argv[1], argv[2]);
        	                        	return 1;
	                        	}
	                	}	
			}
			else {	//acc != 0
				printf("%s : Write permission denied on directory \n", argv[2]);
				return 1;
			}
			return 0;
		
		}
	}
	/*-------------------------Copy multiple files to a directory-------------------------*/
	else if (argc > 3) {
		 // Checks whether target directory exists
		 if (open(argv[argc-1], O_DIRECTORY) != -1) {
			// Checks whether user has permissions to write to target directory
			acc = access(argv[argc-1], W_OK);
			if (acc == 0) {
				for(i = 1; i <= argc-2; i++) {
					strcat(path, argv[argc-1]);
        				strcat(path, "/");
					strcat(path, argv[i]);
					// Opens input file in read-only mode to check if it exists
					fd1 = open(argv[i], O_RDONLY );	
					if (fd1 != -1){
						/* Opens target file in write-only mode
                           			   Creates target file in Read-Write-Execute mode if it does not exist
                           			   Generates an error if target file already exists */
						fd2 = open(path, O_CREAT | O_WRONLY | O_EXCL, S_IRWXU);
                				if (fd2 != -1) {
                        				while ((n = read(fd1, buffer, BUFFSIZE)) > 0 ) {
                                				if (write(fd2, buffer, n) != n)
                                        				printf("Write Error! \n");
                                			}
							printf("%s : File is copied to %s. \n", argv[i], argv[argc-1]);
                        			}
               					else {  // fd2 == -1
							// Checks whether user wants to overwrite the existing file
                        				printf("%s already exists. Do you want to overwrite the file? (y/n) ", argv[i]);
                        				scanf("%c", &usrip);
							getchar();	
                        				if ( usrip == 'y' ) {
                                				fd2 = open(path, O_WRONLY | O_TRUNC);
                                				while ((n = read(fd1, buffer, BUFFSIZE)) > 0 ) {
                                        				if (write(fd2, buffer, n) != n)
                                                				printf("Write Error! \n");
                                				}
								printf("%s : File is copied to %s. \n", argv[i], argv[argc-1]);
                        				}
                        				else {  //usrip != 'y'
                                				printf("%s : Target file is not overwritten. \n", argv[i]);
                        				}
                				}
					}
					else {
						printf("%s : Input file does not exist. \n", argv[i]);
					}
					memset(&path[0], 0, sizeof(path));	// Clears the path variable
				}
				return 0;			
			}
			else { // acc == -1
				printf("%s : Write permission denied on directory. \n", argv[argc-1]);
			}
        	}
        	else { // open(argv[argc], O_DIRECTORY) == -1
                	printf("%s : Directory does not exist. \n", argv[argc-1]);
        	}		
	} 		
	else { // Prints error message if user enters less than 3 arguments
		printf("Insufficient arguments provided. \n");
	}
	return 0;
}
