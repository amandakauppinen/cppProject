// cppFinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//This is assignment number 7 from the project list
//it is a file database for storing contact information based on user input
//the file where everything will be stored does not exist, but will be created
//when the program is ran

#include "pch.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <iterator>
#include <stdlib.h>

using namespace std;

//declarations for non-member functions
void check();
void fileFormat();
void citySearch(ifstream & stream, string token);
void relativeSearch(ifstream & stream, string relative);

class Contact {
private:
	string cName, cEmail, cCity, cRel, cPhone;
	ofstream ofs;
public:
	//constructor-sets all parts to be empty upon creation
	Contact(string name = "", string email = "", string city = "", string rel = "", string phone = "") : cName(name), cEmail(email), cCity(city), cRel(rel), cPhone(phone) {
	}
	//gets inputted name, opens file and stores (*) character attached to name for easier searching
	void getName() {
		cin >> cName;
		cName.insert(0, "*");
		toFile(cName);
	}
	//gets inputted email, sends to toFile function
	void getEmail() {
		cin >> cEmail;
		toFile(cEmail);
	}
	//gets inputted city, sends to toFile function
	void getCity() {
		cin >> cCity;
		toFile(cCity);
	}
	//gets inputted relative, sends to toFile function
	void getRelative() {
		cin >> cRel;
		toFile(cRel);
	}
	//gets inputted phone, sends to toFile function
	void getPhone() {
		cin >> cPhone;
		toFile(cPhone);
	}
	//function to open file, set formatting, store the given content and close file again
	void toFile(string x) {
		ofs.open("records.txt", fstream::app);
		ofs << left << setw(25) << x;
		ofs.close();
	}

	//function to open file and add new line character, then close
	void newRecord() {
		ofs.open("records.txt", fstream::app);
		ofs << "\n";
		ofs.close();
	}

	//function for deleting a specific line given the contact's name
	void deleteLine() {
		//creates vector and string
		vector<string> fileContents;
		string temp;

		//open file, pushes back file contents line by line into the vector until the
		//end of file is reached
		ifstream infile("records.txt");
		while (!infile.eof()) {
			getline(infile, temp);
			fileContents.push_back(temp);
		}

		//close file
		infile.close();
		cin.ignore();

		//create string that will store user's input of the record requested for deletion
		string input;
		cout << "Please enter the name of contact you would like to delete:" << endl;
		getline(cin, input);

		//appends a (*) character to the name given, so that it will have an easier time finding
		//the record (contact's have (*) behind them, whereas relatives do not
		input.insert(0, "*");

		//loop to search the file until the end for the given input
		for (int i = 0; i < (int)fileContents.size(); ++i)
		{
			//searches for substring with same size and value as given input if found, erases the
			//line containing the input and resets the counter to 0 so that when called again, it
			//will start from the beginning of the file
			if (fileContents[i].substr(0, input.length()) == input)
			{
				fileContents.erase(fileContents.begin() + i);
				cout << "Record erased!" << endl << endl;
				i = 0; // Reset search
			}
		}

		//writes the new contents to the file starting from the beginning of the vector, iterating
		//through the end, then closes file
		ofstream out("records.txt", ios::out | ios::trunc);
		for (vector<string>::const_iterator i = fileContents.begin(); i != fileContents.end(); ++i)
		{
			out << *i << endl;
		}
		out.close();
	}

	//function to overwrite everything in file with empty character
	void clearRecords() {
		ofs.open("records.txt");
		ofs << "";
		ofs.close();
	}
	//~Contact();
};

int main()
{
	//variables to be used in program
	int selection;
	bool exit = true;
	string erase, name, city;
	int i = 0;
	Contact record;
	ifstream recordFile("records.txt");

	//calls function to check the contents of the file
	check();

	//while loop to hold switch-case
	while (exit) {
		cout << endl << endl << "Please choose an option below:" << endl << "1) Add your (or another person's) record to phonebook"
			<< endl << "2) Remove record from phonebook" << endl << "3) Search record by city name" << endl <<
			"4) Print all records" << endl << "5) Clear all records" << endl << "6) Exit" << endl << endl;
		cin >> selection;
		if (selection < 1 || selection > 6) {
			cout << "Please try again" << endl << endl;
			main();
		}

		switch (selection) {
			//first case is for entering details-when selected, the user is prompted to enter
			//all of the fields, they are added as data members to the Contact class, and sent to 
			//the function meant for writing the info to the file
		case 1:
			cout << "Please enter the following details;" << endl << "Name:" << endl;
			record.getName();
			cout << endl << "Email:" << endl;
			record.getEmail();
			cout << endl << "City:" << endl;
			record.getCity();
			cout << endl << "Relative:" << endl;
			record.getRelative();
			cout << endl << "Phone:" << endl;
			record.getPhone();
			record.newRecord();
			break;
			//calls deleteLine function to remove a contact based on a name inputted by user
		case 2:
			system("CLS");
			record.deleteLine();
			break;
			//searches by city, displaying only those contacts residing in the inputted city
			//calls citySearch sending the ifstream and input as parameters 
		case 3:
			system("CLS");
			cout << "Please enter the city you would like to search by:" << endl;
			getline(cin, city);
			//needed to put 2 getlines because of '\n' troubles
			getline(cin, city);
			citySearch(recordFile, city);
			break;
			//prints all records-program asks user for what relative they would like to search by
			//program prints relative's contact info first, and then every contact who has the input
			//listed as a relative
		case 4:
			//used to reset the file pointer
			system("CLS");
			recordFile.clear();
			recordFile.seekg(0, ios::beg);
			//sends ifstream and input as parameters to relativeSearch
			cout << "You have chosen to print all records" << endl << endl << "Please enter the name of the relative to sort by:"
				<< "Please note that if the relative is in the system, his/her record will appear first followed by his/her relatives" << endl;
			getline(cin, name);
			getline(cin, name);
			relativeSearch(recordFile, name);
			break;
			//option to erase all records, user is prompted again if they would like to erase, if yes
			//it goes to the clearRecords function, if no it returns to main menu
		case 5:
			system("CLS");
			cout << "Are you sure you wish to erase all records? [Yes/No]" << endl;
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(cin, erase);
			if (erase == "Yes" || erase == "yes") {
				cout << "You have chosen to erase all records" << endl << endl;
				record.clearRecords();
				fileFormat();
				break;
			}
			if (erase == "No" || erase == "no") {
				cout << "You will be returned to the main menu" << endl << endl;
				break;
			}
			else {
				cout << "You have entered an invalid character, please try again" << endl << endl;
			}
			break;
			//shuts down program
		case 6:
			system("CLS");
			cout << endl << "Program is closing" << endl;
			exit = false;
			break;
		default:
			cout << "Please enter a valid selection" << endl;
			break;
		}
	}

	return 0;
}

//function to check file for formatting-if the file is empty, it calls the formatting function
//if the file is not empty, it returns to main
void check() {
	ifstream myFile("records.txt");
	//checks if file is empty, formats it accordingly if it is
	if (myFile.peek() == std::ifstream::traits_type::eof()) {
		fileFormat();
		myFile.close();
	}
}

//function for formatting the file while empty
//opens file, formats with headers, and then closes file
void fileFormat() {
	system("CLS");
	ofstream myFile;
	myFile.open("records.txt", fstream::app);
	myFile << left << setw(25) << "*Name:"
		<< left << setw(25) << "Email:"
		<< left << setw(25) << "City:"
		<< left << setw(25) << "Relative:"
		<< left << setw(25) << "Phone:" << endl;
	myFile.close();
}

//functin to sort by city name, displaying only those results in city inputted by user
void citySearch(ifstream & stream, string city) {
	cout << endl << endl;
	string line;
	//loop that cycles through all lines in file-if the city is present, it will print the
	//line to the terminal
	while (getline(stream, line)) {
		if (line.find(city) != string::npos) {
			cout << line << endl;
		}
	}
	//resets file pointer
	stream.clear();
	stream.seekg(0, ios::beg);
}

//function to sort by relative when displaying all lines
void relativeSearch(ifstream & stream, string relative) {
	string line, firstLine;
	firstLine = "Name";
	cout << endl << endl;

	//first loop is to find all lines with relative, and print to screen
	//cycles through whole file
	while (getline(stream, line)) {
		if (line.find(relative) != string::npos) {
			cout << line << endl;
		}
	}
	//resets file pointer
	stream.clear();
	stream.seekg(0, ios::beg);
	//second loop again looks for the relative name, but when found does nothing and 
	//instead prints all other lines, ignoring the header so that it does not print twice
	while (getline(stream, line)) {
		if (line.find(firstLine) != string::npos) { ; }
		else if (line.find(relative) != string::npos) { ; }
		else {
			cout << line << endl;
		}
	}
}