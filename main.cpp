/**
 * main.cpp
 **
 * Main application for yaycg, Yet Another YAML Configuration Generator
 **  
 *  Copyright (c) 2016 dsstudio
 *
 *  This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

inline string removeComment(const string& str){
	if(str.find_last_of("#") == string::npos) return str;
	else return str.substr(0, str.find_first_of("#"));
}

inline string convertTab(const string& str){
	string s("");
	for(size_t i = 0; i < str.size(); i++){
		if(str[i] == '\t'){
			s.append(4u, ' ');
		}
		else
			s.append(1u, str[i]);
	}
	return s;
}

inline string trim(const string& str){
	string s = convertTab(str);
	return s.substr(
			s.find_first_not_of(" "),
			s.find_last_not_of(" ")
			);
}

inline string leftPart(const string& str){
	/// warning: this is NOT a general function
	/// splitor is `:`
	return str.substr(0, str.find_first_of(":"));
}

inline string rightPart(const string& str){
	/// warning: this is NOT a general function
	/// splitor is `:`
	return str.substr(str.find_first_of(":") + 1);
}

int main(int argc, char** argv){
	bool inEnumArea = false;
	string curTemplate("");
	string buffer("");
	vector<vector<string>*> enumList;
	vector<string> bufferedTemplate;
	int curId;
	bool hasTabKey = false;

	// copyrights. Ugh...
	cout << "yaycg version 0.2, Copyright (C) 2016 dsstudio" << endl;
	cout << "yaycg comes with ABSOLUTELY NO WARRANTY." << endl;
	cout << "For details please check the LICENSE in the ";
	cout << "source code directory." << endl;
	cout << "This is free software, and you are free to copy, modify and " << endl;
	cout << "redistribute it under certain conditions. For details please " << endl;
	cout << "check the LICENSE in the source code directory." << endl;

	if(argc > 1){
		// assuming argv[0] is target template
		cout << "Using " << argv[0] << " as template" << endl;
		curTemplate = argv[0];
	}else{
		cout << "Using template.txt as template" << endl;
		curTemplate = "template.txt";
	}

	if(argc > 2){
		cout << "Illegal param. Exiting." << endl;
		return 1;
	}
	
	ifstream file(curTemplate.c_str());
	if(file.fail()){
		cout << "Cannot read template file. (Permissions problem?)" <<
		endl;
	}

	// resolve enum
	int curEnumIndex = 0;
	while(file.good() && !file.eof()){
		string curLine;
		getline(file, curLine);
		if(!hasTabKey && curLine.find_first_of("\t") != string::npos){
			hasTabKey = true;
			cout << "Warning: your template has \\t" << endl;
			cout << "Please do NOT mix \\t and space." << endl;
			cout << "Every \\t would be converted to 4 spaces." << endl;
		}
		if(curLine == "----------"){
			// enum area start
			inEnumArea = true;
			continue;
		}
		if(inEnumArea){
			if(curLine.empty()){
				// skip empty line
				continue;
			}
			if(curLine[0] == '%'){
				// enum notation
				curEnumIndex = stoi(
								curLine.substr(
									curLine.find_first_not_of("%")));
				continue;
			}
			else{
				if(enumList.size() < curEnumIndex){
					enumList.push_back(new vector<string>());
				}
				enumList[curEnumIndex - 1]->push_back(curLine);
			}
		}
		else{
			if(hasTabKey)
				curLine = convertTab(curLine);
			if(!curLine.empty()){
				if(trim(curLine)[0] != '#'){ // ignore comment lines
					bufferedTemplate.push_back(removeComment(curLine));
				}else
					;
			}
			else
				bufferedTemplate.push_back(curLine);
		}
	}

	// now everything is in memory
	// loading everything may cause some problem on low-ends
	file.close();

	// then jump into fill the blank mode
	cout << "By default, id starts with 0, 1 or higher?: ";
	// XXX: note that we are not checking input
	string curInput("");
	getline(cin, curInput);
	if(curInput.empty()){
		curId = 1;
	}else{
		curId = stoi(curInput);
	}

	for(;;curId++){
		for(string curLine : bufferedTemplate){
			if(curLine.empty()) continue;
			string curLeftPart(leftPart(curLine));
			string curRightPart(rightPart(curLine));
			if(curLine == "$id:"){
				buffer += (to_string(curId) + ":" + "\n");
				cout << curId << ":" << endl;
			}
			else{
				buffer += (curLeftPart + ":");
				cout << curLeftPart << ": ";
				if(!curRightPart.empty()){
					if(curRightPart == "LIST"){
						// List editing mode, aw snap
						// in list editing mode
						// ### to save & exit
						// ^^^ to edit last line
						// ___ to delete last line
						cout << "Now editing a list" << endl;
						buffer += "\n";
						vector<string> list;
						string curIndent = curLine.substr(0, curLine.find_first_not_of(" "));
						for(;;){
							getline(cin, curInput);
							if(curInput == "###"){
								// save and exit
								for(string curLineInList : list){
									buffer += (curLineInList + "\n");
								}
								break;
							}
							else if(curInput == "^^^"){
								// last line
								cout << "Editing last line (would be covered by new one)" << endl;
								list.pop_back();
								getline(cin, curInput);
								list.push_back(curInput);
							}
							else if(curInput == "___"){
								cout << "Deleting last line" << endl;
								list.pop_back();
							}
							else
								list.push_back(curIndent + " - " + curInput);	
						}
					}
					else if(curRightPart[0] == '%'){
						// enum input mode, aw crap
						vector<string> curEnumList = *(enumList[stoi(
												curRightPart.substr(1)) - 1]);
						int userInput = 0;
						cout << endl;
						for(;;){	
							userInput = 0;
							for(int j = 0; j < curEnumList.size(); j++){
								cout << j << "=" << curEnumList[j] << endl;
							}
							getline(cin, curInput);
							if(curInput.empty()) continue; // XXX: info
							userInput = stoi(curInput);
							if(userInput >= 0 
									&& userInput < curEnumList.size()){
								buffer += (curEnumList[userInput] + "\n");
								break;
							}else{
								cout << "Invalid input!" << endl;
							}
						}
					}
					else{
						cout << "(" << curRightPart << ") ";
						getline(cin, curInput);
						if(curInput.empty()){
							// use default value
							buffer += (curRightPart + "\n");
						}
						else{
							// use input value
							buffer += (curInput + "\n");
						}
					}
				}
				else{
					// string, without default value
					getline(cin, curInput);
					buffer += (curInput + "\n");
				}
			}
		}
		cout << "Is there any more?(Y/n) ";
		getline(cin, curInput);
		if(curInput == "n" || curInput == "N") break;
		else if(curInput.empty()) continue;
		else continue;
	}
	
	// flush into disk
	ofstream output("config.yml", std::ofstream::binary);
	output.write(buffer.c_str(), buffer.length());
	output.flush();
	output.close();

	// finalize enums loaded	
	for(int i = 0; i < enumList.size(); i++){
		delete enumList[i];
	}

	cout << buffer << endl;
	cout << "File has been written to config.yml." << endl;

	return 0;
}

