#include <bits/stdc++.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include<stdio.h>

using namespace std;

void usage(char* cmd) {
	cout << "This program removed unwanted spaces in empty lines of plain text files (useful for source files ex: cpp)" << endl << endl;
	cout << "Usage   : " << cmd << " <filename(s)>" << endl;
	cout << "Example : " << cmd << " file1.txt file2.txt" << endl;
}

void ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
        return !isspace(ch);
    }));
}

void rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
        return !isspace(ch);
    }).base(), s.end());
}

void trim(string &s) {
    ltrim(s);
    rtrim(s);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		usage(argv[0]);
	}

	for (int i = 1; i < argc; i++) {
		char* in_path = argv[i];
		char* out_path;
		strcpy(out_path, in_path);
		strcat(out_path, ".tmp");

		ifstream inputfile(in_path);
		ofstream outputfile(out_path);

		string s;

		while (getline( inputfile, s )) {
			string tmp = s;
			trim(tmp);
			if (strcmp(tmp.c_str(),"") == 0) {
				outputfile << "\n";
			} else {
				rtrim(s);
				outputfile << s << "\n";
			}
		}

		outputfile.close();
		inputfile.close();

		rename(out_path, in_path);
	}
}
