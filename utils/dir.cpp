#include <vector>
#include <string>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "dir.hpp"

using namespace std;

string path;
bool trim;
bool ignore;
string real_path;

void process_directory(string directory, vector<string>* files);
void process_entity(struct dirent* entity, string parent, vector<string>* files);

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

vector<string> find_files(string dir, bool trim_parent, bool ignore_dirs) {
	vector<string> files;
	trim = trim_parent;
	ignore = ignore_dirs;
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
	
	string fullpath = parent + "/" + string(entity->d_name);
	if (trim)
		fullpath.erase(0, real_path.length());
	
    if(entity->d_type == DT_DIR) {
        process_directory(string(entity->d_name), files);
		if (!ignore)
			files->push_back(fullpath);
    } else {
		files->push_back(fullpath);
	}
	
	return;
}

string readsymlink(string path) {
	char buf[1024];
	ssize_t len;
	if ((len = readlink(path.c_str(), buf, sizeof(buf)-1)) != -1) {
		buf[len] = '\0';
		return string(buf);
	}
	return "";
}