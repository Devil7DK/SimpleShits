#include <string>
#include <vector>

using namespace std;

vector<string> find_files(string dir, bool trim_parent, bool ignore_dirs);
bool dir_exists(const char* path);
string readsymlink(string path);