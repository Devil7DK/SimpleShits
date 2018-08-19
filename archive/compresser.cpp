#include <vector>
#include <string>
#include <fcntl.h>

#include <archive.h>
#include <archive_entry.h>

#include <utils/dir.hpp>

string parentdir;

const char* trim(string fullname) {
	return fullname.erase(0, parentdir.length() + 1).c_str();
}

void write_archive(string archivename, vector<string> files) {
	struct archive *a;
	struct archive_entry *entry;
	struct stat st;
	char buff[8192];
	int len;
	int fd;
	
	a = archive_write_new();
	archive_write_add_filter_gzip(a);
	archive_write_set_format_cpio(a);
	archive_write_open_filename(a, archivename.c_str());
	for (string file : files) {
		string filename = file;
		stat(file.c_str(), &st);
		entry = archive_entry_new();
		archive_entry_set_pathname(entry, trim(filename));
		archive_entry_set_size(entry, st.st_size);
		archive_entry_set_filetype(entry, AE_IFREG);
		archive_entry_set_perm(entry, 0644);
		archive_write_header(a, entry);
		fd = open(file.c_str(), O_RDONLY);
		len = read(fd, buff, sizeof(buff));
		while ( len > 0 ) {
			archive_write_data(a, buff, len);
			len = read(fd, buff, sizeof(buff));
		}
		close(fd);
		archive_entry_free(entry);
	}
	archive_write_close(a);
	archive_write_free(a);
}

int usage(const char* name) {
	printf("usage\t: %s <directory>\n", name);
	printf("example\t: %s archive.cpio.gz ~/test\n", name);
	return 1;
}

int main(int argc, const char **argv) {
	if (argc != 3)
		return usage(argv[0]);
	
	string archivename = argv[1];
	parentdir = realpath(argv[2], NULL);
	
	vector<string> files = find_files(parentdir, false);
	write_archive(archivename, files);
	return 0;
}