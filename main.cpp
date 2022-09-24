// TODO: remove and replace this file header comment
// This is a .h file you will edit and turn in.

// Remove starter comments and add your own
// comments on each function and on complex code sections.

#pragma once

#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdio.h>
#include <string>
using namespace std;


template <typename t>
void displayResult(t print){
	for(auto& obj : print)
		cout << obj << endl;
}

string convertLower(string line){
	string lower;
	for(auto& ch : line){
		lower += tolower(ch);
	}
	return lower;
}

/*
@param:
s: string of the token to be "cleaned' have returned

Takes in a string, verifys it has letters

Trims leading & trailing puncuation

Converts to lower case

@return tempS:  trimmed string
*/
string cleanToken(string s) {
	int sSize = s.size(); // Size of string
	bool alpha = false;   // Check for if s contains letter

	/**If this comes up again, create helper function**/
	for(auto& ch : s){ // First checks if any letters are contained
		if(isalpha(ch)){
			alpha = true;
			break;
		}
	}

	if(sSize == 0 || alpha != true)
		return ""; //

	 // if(!alpha)//No letters found in s -> alpha = false
	 // return "";

	int letterIndex = 0; // Index of first letter occurance
	for(char& ch : s){
		if(ispunct(ch)){
			letterIndex++;
			// cout << "String: " << s << " : " << ch << " : " << letterIndex << endl;
		} else
			break;
	}

	string tempS;
	tempS = s.substr(letterIndex, sSize);

	sSize = tempS.size(); // Size of parsed string
	letterIndex = sSize;  // End of tempS
	for(int k = sSize - 1; k > 0; k--){
		if(ispunct(tempS[k]) || tempS[k] == ' '){
			letterIndex--;
		} else
			break;
	}

	if(letterIndex != sSize)               // Trim trail punc
		tempS = tempS.substr(0, letterIndex); //?? Make sure it's not cutting short

	 /**If this comes up again create helper function**/
	 //?? make sure it's proplery addressing the memory location to lowercase
	for(auto& ch : tempS) // Converts to lower case
		ch = tolower(ch);

	return tempS;
}

/*
@param:'
text: string containg entire body from a single web page

Gets passed the entire body of a webpage


return set<string>:
*/
set<string> gatherTokens(string text) {
	set<string> tokens; // Holds the cleaned body of text
	stringstream ss(text);
	string cleaned, token;
	int tSize = text.size();

	if(tSize == 0){
		cout << "Empty body" << endl;
	} else{
		while(getline(ss, token, ' ')){
			cleaned = cleanToken(token);

			tokens.insert(cleaned);
		}
	}

	return tokens;
}

/**
 * @brief opens a file,
 * @param readIn (PBR)
 */
void openFile(string const file, ifstream& readIn) {
	readIn.open(file);

	if(readIn.is_open())
		cout << "Opened: " << file << endl;
	else
		cout << "Failed to open: " << file << endl;
}

/**
 * @brief
 * @param filename: name of  database file
 * @param index: (PBR) Map to be populated with inverted data
 * @return count of processed webpages that were ADD to the index
 */
int buildIndex(string filename, map<string, set<string>>& index) {
	int inserts = 0; // Number of unique inserts into map ??
	int lineCount = 1;
	string body, bodyToken, file, token;
	ifstream readIn;

	openFile(filename, readIn); // File is now opened

	while(getline(readIn, token)){ // Loop through file line-by-line
		if(lineCount % 2 != 0){      // Odd file line
			file = token;
			index[file];
			inserts++;
		} else{
			index[file] = gatherTokens(token);
			file.clear(); // Set empty, because end of pairing for (filename, body)
		}
		lineCount++;
	}
	readIn.close();
	return inserts;
}

/*
Helper function
*/
void searchParameters(set<string>& include, set<string>& exclude, string sentence){
	string token;
	char condition = '+';//Default adds to include
	stringstream ss(sentence);

	while(getline(ss, token, ' ')){//Everything in between spaces
		//cout << "Token: " << token << endl;
		if(token[0] == '+'){//Else there is no condition change
			condition = '+';
			token.erase(0, 1);//Trim condition
		} else if(token[0] == '-'){
			condition = '-';
			token.erase(0, 1);
		}
		token = convertLower(token);

		//Inserts token based on search condition status
		if(condition == '+')
			include.insert(token);
		else if(condition == '-')
			exclude.insert(token);
		//cout << "Token Cleaned: " << token << endl;
	}
}

/**
 * @brief Collects all url's that contain matching words
 * @param index 
 * @param include 
*/
void searchIncludes(map<string, set<string>>& index, set<string>& include, set<string>& result){
	for(auto& term : include){//Itterate through search terms
		for(auto& site : index){//Loops through all urls checking for term
			if(site.second.count(term))
				result.insert(site.first);
		}
	}
}

/**
 * @brief Loops through results, removing any urls that contain excluded terms
 * @param exclude 
 * @param result 
*/
void removeExcludes(map<string, set<string>>& index, set<string>& exclude, set<string>& result){
	set<string> results;
	for(auto& term : exclude){//Itterate through exclude terms
		for(auto& site : result){//Itterate through collected site
			if(index[site].count(term) == 0){//Exclude term not found
				results.emplace(site);//Include in return results
			}
		}
	}
	if(results.size() != 0)
		result = results;
}

set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
	set<string>  result, include, exclude;

	searchParameters(include, exclude, sentence);//Parses out the search parameters
	searchIncludes(index, include, result);
	removeExcludes(index, exclude, result);

	return result;
}

void run(map<string, set<string>>& index, string file){
	int inserts = buildIndex(file, index);
	cout << "Stand by while building index..." << endl;
	cout << "Indexed " << inserts << " pages containing xxxx unique terms\n" << flush;

}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string filename) {
	map<string, set<string>> index;
	set<string> pages;
	string input;

	run(index, filename);

	do{
		cout << "Enter query sentence(press enter to quit): ";
		getline(cin, input);

		pages = findQueryMatches(index, input);

		displayResult(pages);
	} while(input != " ");
	//Constructs inverted index (see main)
	
	//Captures inserts (see main)
	//tally unique words from all
		//ITT index, count size of secnd
			//Function call, count index.second.size();

	//Loop to query pages (see main)
		//" " ends
	

	// TODO:  Write this function.
}
