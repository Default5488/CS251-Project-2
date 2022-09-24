// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.

// Remove starter comments and add your own
// comments on each function and on complex code sections.

#include <iostream>
#include "search.h"
using namespace std;

template <typename t>
void displaySet(t print){
	for(auto& obj : print)
		cout << obj << " ";
}

void displayMap(map<string, set<string>>& print){
	map<string, set<string>>::iterator it;

	for(it = print.begin(); it != print.end(); it++){
		cout << "\n\nFile: " << it->first << flush << "\nBody:\n" << flush;
		displaySet(it->second);
		cout << endl;
	}
}

void testCleanToken(){
	string t1 = "...this is a, line! there are a pluthera of comma's, periods & puncuation through these lines that you're consuming..., hugh, ,";

	string t2 = ",)9We're wanting to see if they're capable, if they're worthy, can they complete the tast. Parse out all of the in correct, and unjust.";

	string t3 = "Whilst keeping the integradey of the interanl structure?";

	string t4 = "Wisconsin Lutheran College opened in the fall of 1973 with a part-time faculty and two dozen students. The school had its first full-time president two years later. In 1977 the school purchased five buildings on an 8.5-acre (3.4 ha) campus, and had a set of plans that allowed for growth and development. In 1982, the college purchased the academic library from Milton College. Volunteers moved and installed this 60,000 volume library. In 1983, the college purchased and installed the science laboratory furnishings of the University of Wisconsin Center at Medford. These major additions helped the college pursue its dream of becoming a four-year college.";

	cout << "Clean 1: \n" << cleanToken(t1) << endl << endl;

	cout << "Clean 2: \n" << cleanToken(t2) << endl << endl;

	cout << "Clean 3: \n" << cleanToken(t3) << endl << endl;

	cout << "Clean 4: \n" << cleanToken(t4) << endl << endl;
}

/**
 * @brief Needs a more indepth testing
 *
 * @param file: passed file name
*/
void testGatherToken(string file){
	set<string> cleanToken;
	fstream readin;
	string tmpToken, token;

	readin.open(file);

	if(!readin.is_open())
		cout << "Bad file read in.\n" << flush;

	while(getline(readin, tmpToken)){
		if(!readin.good()){
			cout << "Bad read in\n" << flush;
			continue;
		}
		token += " ";
		token += tmpToken;
	}

	cleanToken = gatherTokens(token);
	cout << "Token Size: " << cleanToken.size() << endl;
	displaySet(cleanToken);

}



void testReverseIndex(map<string, set<string>>& index, string file){
	int inserts;

	inserts = buildIndex(file, index);

	displayMap(index);
	cout << "\n\nInserts: " << inserts << endl;
}

void testSearch(map<string, set<string>>& index){
	string in;
	cout << "Enter tight search parameters\n";
	cin.clear();
	getline(cin, in);

	findQueryMatches(index, in);
}

int main() {
	map<string, set<string>> index;
	string loop;
	// cin >> loop;
	getline(cin, loop);

	string file;
	while(loop != "#"){

		if(loop == "ct"){
			cout << "----------Clean Token----------" << endl;
			testCleanToken();
		}
		
		if(loop == "gt"){
			cout << "\n----------Gather Token----------" << endl;
			cout << "Enter file: ";
			cin >> file;
			cout << endl;
			testGatherToken(file);
		}
		
		if(loop == "ri"){
			 	cout << "\n\n----------Inverted Index----------" << endl;
			 	cout << "Enter file: ";
			 	cin >> file;
			 	cout << endl;
			 	testReverseIndex(index, file);
		}

		if(loop == "s"){
			testSearch(index);
		}
		// cin >> loop;
		getline(cin, loop);
	}
	return 0;
}