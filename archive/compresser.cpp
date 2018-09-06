#include <vector>
#include <string>
#include <cstring>
#include <fcntl.h>

#include <archive.h>
#include <archive_entry.h>

#include <utils/dir.hpp>

#define CLR_R "\x1b[31m" // Red
#define CLR_G "\x1b[32m" // Green
#define CLR_Y "\x1b[33m" // Yellow
#define CLR_B "\x1b[34m" // Blue
#define CLR_M "\x1b[35m" // Magenta
#define CLR_C "\x1b[36m" // Cyan
#define CLR_N "\x1b[0m"  // Reset

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
		string filename = trim(file);
		lstat(file.c_str(), &st);
		entry = archive_entry_new();
		archive_entry_set_pathname(entry, filename.c_str());
		archive_entry_set_size(entry, st.st_size);

		string type, link;
		switch (st.st_mode & S_IFMT) {
			case S_IFBLK:
				type = "block device";
				archive_entry_set_filetype(entry, AE_IFBLK);
				break;
			case S_IFCHR:
				type = "character device";
				archive_entry_set_filetype(entry, AE_IFCHR);
				break;
			case S_IFDIR:
				type = "directory";
				archive_entry_set_filetype(entry, AE_IFDIR);
				break;
			case S_IFIFO:
				type = "FIFO/pipe";
				archive_entry_set_filetype(entry, AE_IFIFO);
				break;
			case S_IFLNK:
				link = readsymlink(file);
				type = "symlink";				
				archive_entry_set_filetype(entry, AE_IFLNK);
				archive_entry_set_symlink(entry, link.c_str());
				break;
			case S_IFREG:
				type = "regular file";
				archive_entry_set_filetype(entry, AE_IFREG);
				break;
			case S_IFSOCK:
				type = "socket";
				archive_entry_set_filetype(entry, AE_IFSOCK);
				break;				
			default:
				type = "unknown";
				archive_entry_set_filetype(entry, AE_IFREG);
				break;
		}
		
		if (strcmp(link.c_str(),"") != 0)
			link = " -> \'" + link + "\'";
		
		printf("Adding " CLR_R "%s" CLR_N " to archive : " CLR_G "%s " CLR_Y "%s" CLR_N "\n", type.c_str(), filename.c_str(), link.c_str());
		
		archive_entry_set_perm(entry, st.st_mode & 07777);
				
		archive_write_header(a, entry);
		if (archive_entry_size(entry) > 0) {
			fd = open(file.c_str(), O_RDONLY);
			len = read(fd, buff, sizeof(buff));
			while ( len > 0 ) {
				archive_write_data(a, buff, len);
				len = read(fd, buff, sizeof(buff));
			}
			close(fd);
		}
		archive_entry_free(entry);
	}
	
	archive_write_close(a);
	archive_write_free(a);
}

int usage(const char* name) {
	printf("usage\t: %s <file> <directory>\n", name);
	printf("example\t: %s archive.cpio.gz ~/test\n", name);
	return 1;
}

int main(int argc, const char **argv) {
	if (argc != 3)
		return usage(argv[0]);
	
	string archivename = argv[1];
	parentdir = realpath(argv[2], NULL);
	
	vector<string> files = find_files(parentdir, false, false);
	write_archive(archivename, files);
	return 0;
}