#include <bits/stdc++.h>
#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <stdio.h>

using namespace std;

int check_type(string path) {
	struct stat s;
	if(stat(path.c_str(), &s) == 0) {
		if(s.st_mode & S_IFDIR ) {
			return 1;
		} else if( s.st_mode & S_IFREG ) {
			return 0;
		} else {
			return 2;
		}
	}
	return -1;
}

bool exists(string name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void usage(char* cmd) {
	printf("This program removes unwanted spaces in empty lines of plain text files (useful for source files ex: cpp)\n");
	printf("Usage   : %s <filename(s)>\n", cmd);
	printf("Example : %s file1.txt file2.txt\n", cmd);
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
		string in_path = argv[i];
		string out_path = in_path + ".tmp";

		switch (check_type(in_path)) {
			case 1:
				printf("skipped directory: %s\n", in_path.c_str());
				continue;
			case 2:
				printf("error, unknown type: %s\n", in_path.c_str());
				continue;
			case -1:
				printf("error, unable to determine type: %s\n", in_path.c_str());
				continue;
		}

		ifstream inputfile(in_path.c_str());
		ofstream outputfile(out_path.c_str());

		if (!inputfile.is_open()) {
			printf("unable to open input : %s\n", in_path.c_str());
			continue;
		}

		if (!outputfile.is_open()) {
			printf("unable to open output : %s\n", out_path.c_str());
			continue;
		}

		string s;
		printf("processing file : %s...", in_path.c_str());
		while (getline(inputfile, s)) {
			string tmp = s;
			trim(tmp);
			if (strcmp(tmp.c_str(), "") == 0) {
				outputfile << "\n";
			} else {
				rtrim(s);
				outputfile << s << "\n";
			}
		}

		outputfile.close();
		inputfile.close();

		rename(out_path.c_str(), in_path.c_str());
		printf(" done.\n");
	}

	return 0;
}
