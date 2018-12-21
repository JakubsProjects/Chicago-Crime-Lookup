#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Declaring struct to store data in
typedef struct datainfo
{
	char *caseNumber;
	char *IUCR;
	char *dateNtime;
	char arrested[1];

} datainfo;

//Making global variable(as allowed)
int NumOfCollision;

//Function that reads in file and strip characters
char *getFileName()
{
	char filename[512];
	int  fnsize = sizeof(filename) / sizeof(filename[0]);

	// input filename from the keyboard:
	fgets(filename, fnsize, stdin);
	filename[strcspn(filename, "\r\n")] = '\0';  // strip EOL char(s):

												 // make sure filename exists and can be opened:
	FILE *infile = fopen(filename, "r");
	if (infile == NULL)
	{
		printf("**Error: unable to open '%s'\n\n", filename);
		exit(-1);
	}

	fclose(infile);

	// duplicate and return filename:
	char *s = (char *)malloc((strlen(filename) + 1) * sizeof(char));
	strcpy(s, filename);

	return s;
}


//Checks whether user input is an error
int errorCheck(char* random) {
	//Getting the area code
	char *a = random + 1;
	int numVals = atoi(a);
	//If the area code is bigger or equal to 78
	if (numVals >= 78) {
		return 0; //return 0;
	}

	//If the string is less than 8 characters
	if (strlen(random - 1) < 8) {
		return 0; return 0;
	}
	return 1; //else return 1;
}
int hash(char *random, long N)
{
	//initializing hashvalue
	long hashvalue;
	hashvalue = 0;

	//Getting area code
	char *a = random + 1;
	int numVals = atoi(a);
	//Moving pointer and getting last 4 digits
	char *b = random + 4;
	int lastfourdig = atoi(b);
	//lastfourdig = lastfourdig % sectionsize;
	char*c = random + 3;
	int d = atoi(c);
	d = d - 'A';
	

	hashvalue = numVals*31+ d*31 +lastfourdig*17;
		

	//abs'ing it, incase its not pos number
	long hashcode = abs(hashvalue);

	//Return the value modded by size of hashtable
	return hashcode % N;
}

//Stores the information into hasahtable
void HTStoreData(datainfo** hashtable, char* caseNumber, datainfo* struct1, long sizeofHT) {
	long a = hash(caseNumber, sizeofHT); //Getting hashvaluee for input
	int count = 0;

	//Doing linear probing to handle collision
	while (1) {
		//If it is null(meaning empty
		if (hashtable[a] == NULL) {
			//Then store it into that position
			hashtable[a] = struct1;
			//printf("* %s %s %s %s\n", hashtable[a]->caseNumber, hashtable[a]->dateNtime, hashtable[a]->IUCR, hashtable[a]->arrested);
			break; //Then break
		}
		//Otherwise if that position is not empty
		else if ((hashtable[a] != NULL)) {
			//Check if next position is empty
			while ((hashtable[a + count] != NULL)) {
				//If next pos not empty, increment global variable
				NumOfCollision += 1;
				//IF the next one is empty, then set it to that pos in array
				if (hashtable[a + count] == NULL) {
					(hashtable[a + count]) = struct1;
					break; // then break
				}
				count++; //Incrementing count to move through array
			}
			break; //Once here break
		}
		//Otherwise
		else {
			a++; //increment a
			if (a == sizeofHT) { //check if a is same size as hashtable size
				a = 0; //If it is, loop back around in the array
			}
		}
	}
}

//This function parses the data and then stores it into struct
void ParsingFunction(char *crimes, long file1size, long sizeofHT, int *NumOfCrimes, datainfo **hashtable) {
	//Allocating memory for the struct
	datainfo *struct1 = (datainfo*)malloc(sizeof(datainfo) * file1size);
	//Inputline to grab each line from file
	char inputline[500];
	//opening the file 
	FILE *file1 = fopen(crimes, "r");
	//Gets the first line
	fgets(inputline, 512, file1);
	int a = 0; //set a to 0
	char *token; //Declaring tokens for parsing purposes
	char *token2;
	char* token3;
	char* token4;
	char* token5;
	char* token6;
	char* token7;
	while (fgets(inputline, 512, file1) != NULL) { //gets the next line of input
												   //Strips off the end character
		inputline[strcspn(inputline, "\r\n")] = '\0';

		//Parsing the data
		token = strtok(inputline, ",");
		//mallocing enough memory for it
		struct1[a].caseNumber = (char*)malloc((strlen(token) + 1) * sizeof(char));
		//storing it into malloced memory
		strcpy(struct1[a].caseNumber, token);

		//printf("%s ", struct1.caseNumber);
		////Parsing the data
		token2 = strtok(NULL, ",");
		//mallocing enough memory for IUCR
		struct1[a].IUCR = (char*)malloc((strlen(token2) + 1) * sizeof(char));
		//Strcpy'ing it into the struct
		strcpy(struct1[a].IUCR, token2);

		//printf("%s ", struct1.IUCR);

		//Getting the info
		token3 = strtok(NULL, ",");
		//Allocating enough memory for it
		struct1[a].dateNtime = (char*)malloc((strlen(token3) + 1) * sizeof(char));
		//strcpy'ing it into the the struct
		strcpy(struct1[a].dateNtime, token3);

		//printf("%s ", struct1.dateNtime);

		//skipping over some data
		token4 = strtok(NULL, ",");
		//skipping over some data
		token5 = strtok(NULL, ",");
		//skipping voer some data
		token6 = strtok(NULL, ",");

		//Parsing the info
		token7 = strtok(NULL, ",");
		//strcpy'ing the data into it
		strcpy(struct1[a].arrested, token7);
		//printf("%s \n", struct1.arrested);

		//INcrement the number of crimes
		(*NumOfCrimes) += 1;
		//Storing the parsed data into hashtable
		HTStoreData(hashtable, struct1[a].caseNumber, &struct1[a], sizeofHT);
		//Incrementing a
		a++;
	}
	fclose(file1); //Closing file
}

//This parses the data for my areas and their names
void ParsingFunction2(char *areas, char **StringArea) {
	char inputline[500]; //Declaring input line
						 //opening file
	FILE *file1 = fopen(areas, "r");
	char* token; //Declaring tokens
	char *token2; //Declaring tokens

				  //Getting the first line
	fgets(inputline, 512, file1);
	int a = 0;
	while (fgets(inputline, 512, file1) != NULL) { //While line is not NULL
		inputline[strcspn(inputline, "\r\n")] = '\0'; //strip off end character

													  // skip the first info
		token = strtok(inputline, ",");
		//Parsing data
		token2 = strtok(NULL, ",");
		//mallocing memory for the string name
		StringArea[a] = (char*)malloc((strlen(token2) + 1) * sizeof(char));
		//Storing the string into struct
		strcpy(StringArea[a], token2);
		a++;//incrementasting a

	}
	fclose(file1); //closing the file
}

//Gets the size of the file in bytes
long getFileSizeInBytes(char *filename) {
	FILE *file = fopen(filename, "r");   if (file == NULL)     return -1;

	fseek(file, 0, SEEK_END);   long size = ftell(file);

	fclose(file);

	return size;
}

//Initializing hashtable setting all to NULL
void initializeHT(datainfo **hashtable, long size) {
	int a = 0; //a = 0
			   //Uses for loop to set all pointers to NULL
	for (a = 0; a < size; a++) {
		hashtable[a] = NULL;
		//printf("%d\n", hashtable[a]);
	}

}

//Area look up function
char* arealookup(char* inputSearch, char** StringArea, int *areaNum)
{
	//Gets the first two numbers(area)
	char *a = inputSearch + 1;
	(*areaNum) = atoi(a); //converts t into numbers

	return StringArea[*areaNum]; //Returns the character string

}

//Function to retrieve the node in the data
datainfo* lookup(datainfo** hashtable, char *inputSearch, long sizeofHT, long hashsearchval) {

	//printf(" %s %s\n", hashtable[hashsearchval]->caseNumber, inputSearch);
	inputSearch[strcspn(inputSearch, "\r\n")] = '\0';
	//printf("")
	while (1) { // if the hasttable is empty
		if (hashtable[hashsearchval] == NULL) {
			return NULL; //return NULL
		}

		//else if the match then return the struct at that position
		if (strcmp(hashtable[hashsearchval]->caseNumber, inputSearch) == 0)
		{
			return hashtable[hashsearchval];
		}

		else {
			hashsearchval++; //otherwise increase the index of array

			if (hashsearchval == sizeofHT) { //if array index matches sizeofHT then set back to 0
				hashsearchval = 0;
			}
		}
	}

	return NULL; //otherwise if some error found return NULL
}

int main() {
	char *crimes, *areas; //Declaring variables
	int areaNum, NumOfCrimes = 0; //Declaring variables
	crimes = getFileName(); //opening the file
	areas = getFileName(); //opening the file
	NumOfCollision = 0; //Setting num of collision to 0
	long file1size = getFileSizeInBytes(crimes); //Gets the bytes of file1size
	long sizeofHT = (file1size / 50) * 13; //Multiply by 8.5
	char inputSearch[100]; //array to read in each line
	int inputSeachSize = sizeof(inputSearch) / sizeof(inputSearch[0]);
	//Allocating memory for the hastable
	datainfo** hashtable = (datainfo**)malloc(sizeof(datainfo*) * sizeofHT);
	initializeHT(hashtable, sizeofHT); //Initializng hashtable to empty in each position
	if (hashtable == NULL) {
		printf("** Error: malloc failed to allocate hash table (%ld elements).\n", sizeofHT);
		exit(-1);
	}
	//Parses the first file & storing data from crimes file
	ParsingFunction(crimes, file1size, sizeofHT, &NumOfCrimes, hashtable);
	//Allocating memory for areas file
	char **StringArea = (char**)malloc(sizeof(char*) * 78);
	ParsingFunction2(areas, StringArea); //Parsing the second file in order to store areas
										 //Print statement to display information
	printf(">> Crime file: %ld\n", file1size);
	printf(">> # crimes %d\n", NumOfCrimes);
	printf(">> HT size: %ld\n", sizeofHT);
	printf(">> Collision: %d\n", NumOfCollision);
	printf("Enter a case Number> ");
	//Gets the userinput
	while (fgets(inputSearch, inputSeachSize, stdin) != NULL) {
		inputSearch[strcspn(inputSearch, "\r\n")] = '\0'; //strips end character
		int hashsearchval = hash(inputSearch, sizeofHT); //Gets the hashval for userinput

		if (errorCheck(inputSearch) == 0) { //Error checking
			printf("** invalid case #. try again... \n\n");

		}
		else { //Looksup the position in the hashtable
			datainfo *searchup = lookup(hashtable, inputSearch, sizeofHT, hashsearchval);
			char* areaString = arealookup(inputSearch, StringArea, &areaNum); //Looksup the area 
																			  //If its null then print out this
			if (searchup == NULL) {
				printf(">> hash index: %d <<\n", hashsearchval);
				printf("** Case not found...\n\n");
			}
			else {//otherwise data was found so display the data
				printf(">> hash index: %d <<\n", hashsearchval);
				printf("%s:\n", searchup->caseNumber);
				printf(" date/time: %s\n", searchup->dateNtime);
				printf(" city/area : %d => %s\n", areaNum, areaString);
				printf(" IUCR code: %s\n", searchup->IUCR);
				//Comparing to see if we need to output true or false
				if (strcmp(searchup->arrested, "F") == 0) {
					printf(" arrested: false\n\n");
				}
				else if (strcmp(searchup->arrested, "T") == 0) {
					printf(" arrested: true\n\n");
				}
			}
		}
		printf("Enter a case Number> "); //prompts user to enter in next input
	}
	return 0;
}

