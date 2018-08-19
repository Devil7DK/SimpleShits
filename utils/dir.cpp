#include <vector>
#include <string>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "dir.hpp"

using namespace std;

string path;
bool trim;
string real_path;

void process_directory(string directory, vector<string>* files);
void process_entity(struct dirent* entity, string parent, vector<string>* files);

int main(int argc, char** argv) {
	if (argc < 3)
		return 1;
	string dir = argv[1];
	vector<string> files = find_files(dir, atoi(argv[2]));
	for (string i : files)
		printf("%s\n", i.c_str());
	return 0;
}

bool dir_exists(const char* path) {
	struct stat statbuf;
	int isDir = 0;
	if (stat(path, &statbuf) != -1) {
		if (S_ISDIR(statbuf.st_mode)) {
			return true;
		}
	}
	return false;
}

vector<string> find_files(string dir, bool trim_parent) {
	vector<string> files;
	trim = trim_parent;
	path = realpath(dir.c_str(), NULL);
	real_path = path + "/";
	process_directory("", &files);
	return files;
}

void process_directory(string directory, vector<string>* files) {
    string dirToOpen = path + directory;
	
    auto dir = opendir(dirToOpen.c_str());
	
    path = dirToOpen + "/";
	
    if(NULL == dir)
    {
		fprintf(stderr,"error opening directory: %s\n", dirToOpen.c_str());
        return;
    }
	
    auto entity = readdir(dir);
	
    while(entity != NULL)
    {
        process_entity(entity, dirToOpen, files);
        entity = readdir(dir);
    }
	
    path.resize(path.length() - 1 - directory.length());
    closedir(dir);
}

void process_entity(struct dirent* entity, string parent, vector<string>* files) {
	if(entity->d_name[0] == '.')
		return;
	
    if(entity->d_type == DT_DIR)
    {
        process_directory(string(entity->d_name), files);	
        return;
    }

    if(entity->d_type == DT_REG)
    {
		string fullpath = parent + "/" + string(entity->d_name);
		if (trim)
			fullpath.erase(0, real_path.length());
		files->push_back(fullpath);
        return;
    }
}