#include <iostream>
#include <string>
#include <ctime>
using namespace std;

void displayUi();
char *getDocs();

int main() {
	//initializes each doc/line in the library with null
	char *library_docs[1024];
	for (int i = 0; i < 1024; i++) {
		library_docs[i] = NULL;
	}

	//initializes each doc/line in the recent list with null
	char *list_docs[128];
	for (int i = 0; i < 128; i++) {
		list_docs[i] = NULL;
	}

	cout << "Starting Program 1 \n";
	/*
	initializes each doc/line in the library with random letters
	*/
	cout << "Initializing library w/ random letters \n";
	for (int i = 0; i < 1024; i++) {
		char *doc = getDocs();
		library_docs[i] = doc;
	}

	/*
	initializes each doc/line in the recent list with random letters
	*/
	cout << "Initializing recent_list w/ random letters \n";
	for (int i = 0; i < 128; i++) {
		char *doc = getDocs();
		list_docs[i] = doc;
	}


	while (true) {
		displayUi();
		int input;
		int ejected = 0;
		cin >> input;

		if (input > 0 && input < 16) {
			for (int i = 0; i < 128; i++) {
				size_t found;
				string s = list_docs[i];
				switch (input) {	//inserts input and finds the corresponding word in the recent list 
				case 1:
					found = s.find("FIRST");
					break;
				case 2:
					found = s.find("CPP");
					break;
				case 3:
					found = s.find("REVIEW");
					break;
				case 4:
					found = s.find("PROGRAM");
					break;
				case 5:
					found = s.find("ASSIGNMENT");
					break;
				case 6:
					found = s.find("CECS");
					break;
				case 7:
					found = s.find("BEACH");
					break;
				case 8:
					found = s.find("ECS");
					break;
				case 9:
					found = s.find("FALL");
					break;
				case 10:
					found = s.find("SPRING");
					break;
				case 11:
					found = s.find("OS");
					break;
				case 12:
					found = s.find("MAC");
					break;
				case 13:
					found = s.find("LINUX");
					break;
				case 14:
					found = s.find("WINDOWS");
					break;
				case 15:
					found = s.find("LAB");
					break;
				}
				if (found == string::npos) {
					//cout << "NOT FOUND \n";
					for (int j = i; j < 128; j++) {	//shifts each doc/line in the recent list up one 
						list_docs[j] = list_docs[j + 1];
					}
					char *doc = getDocs();	//returns a another set of reinitialized docs/lines
					library_docs[1023] = doc;	//sets end of the library with the new set of doc
					list_docs[127] = library_docs[0];	//sets end of recent list with the first doc of library

					for (int j = i; j < 1024; j++) {	//shifts each doc/line in the library up one
						library_docs[j] = library_docs[j + 1];
					}
					ejected++;	//increments if 
				}
				else {
					//cout << "FOUND \n";
				}
			}
		}
		cout << "#" << ejected << "ejected \n";
	}

	getchar();
}

//displays UI
void displayUi() {

	cout << "SELECT A WORD, 0 TO QUIT \n";

	cout << "1. FIRST \n" 
		<< "2. CPP \n"
		<< "3. REVIEW \n"
		<< "4. PROGRAM \n"
		<< "5. ASSIGNMENT \n"
		<< "6. CECS \n"
		<< "7. BEACH \n"
		<< "8. ECS \n"
		<< "9. FALL \n"
		<< "10. SPRING \n"
		<< "11. OS \n"
		<< "12. MAC \n"
		<< "13. LINUX \n"
		<< "14. WINDOWS \n"
		<< "15. LAB \n";
}

/*
initializes each doc in with a random letter and returns it
*/
char *getDocs() {
	char alphabets[26] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N',
		'O','P','Q','R','S','T','U','V','W','X','Y','Z'};
	int randomSize = rand() % (3000000000 - 2000000000 + 1) + 2000000000;

	char *docs = new char[randomSize];
	for (int i = 0; i < randomSize - 1; i++) {
		docs[i] = alphabets[rand() % 26];
	}

	return docs;
}
