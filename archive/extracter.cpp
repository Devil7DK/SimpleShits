#include <iostream>

#include <archive.h>
#include <archive_entry.h>

using namespace std;

bool exists(string name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

static int copy_data(struct archive *ar, struct archive *aw) {
	int r;
	const void *buff;
	size_t size;
	la_int64_t offset;

	for (;;) {
		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (r < ARCHIVE_OK)
			return (r);
		r = archive_write_data_block(aw, buff, size, offset);
		if (r < ARCHIVE_OK) {
			fprintf(stderr, "%s\n", archive_error_string(aw));
			return (r);
		}
	}
}

int usage(char* prog) {
	printf("usage : %s <filename> <outdir>\n\n", prog);
	printf("example : %s archive.tar.gz extracted\n\n", prog);
	return 1;
}

int main(int argc, char** argv) {
	if (argc < 3)
		return usage(argv[0]);

	string filename = argv[1];
	string outdir = argv[2];

	if (!exists(filename)) {
		printf("file \"%s\" doesn't exist..!\n\n", filename.c_str());
		return 1;
	}

	struct archive *a;
	struct archive *ext;
	struct archive_entry *entry;
	int flags;
	int r;

	flags = ARCHIVE_EXTRACT_TIME;
	flags |= ARCHIVE_EXTRACT_PERM;
	flags |= ARCHIVE_EXTRACT_ACL;
	flags |= ARCHIVE_EXTRACT_FFLAGS;

	a = archive_read_new();
	archive_read_support_format_all(a);
	archive_read_support_compression_all(a);
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, flags);
	archive_write_disk_set_standard_lookup(ext);
	if ((r = archive_read_open_filename(a, filename.c_str(), 102400)))
		return 1;
	for (;;) {
		r = archive_read_next_header(a, &entry);
		if (r == ARCHIVE_EOF)
			break;
		if (r < ARCHIVE_OK)
			fprintf(stderr, "%s\n", archive_error_string(a));
		if (r < ARCHIVE_WARN)
			return 1;
		const char* currentFile = archive_entry_pathname(entry);
		const string fullOutputPath = outdir + "/" + currentFile;
		archive_entry_set_pathname(entry, fullOutputPath.c_str());
		printf("extracting %s\n", currentFile);
		r = archive_write_header(ext, entry);
		if (r < ARCHIVE_OK)
			fprintf(stderr, "%s\n", archive_error_string(ext));
		else if (archive_entry_size(entry) > 0) {
			r = copy_data(a, ext);
			if (r < ARCHIVE_OK)
				fprintf(stderr, "%s\n", archive_error_string(ext));
			if (r < ARCHIVE_WARN)
				return 1;
		}
		r = archive_write_finish_entry(ext);
		if (r < ARCHIVE_OK)
			fprintf(stderr, "%s\n", archive_error_string(ext));
		if (r < ARCHIVE_WARN)
			return 1;
	}
	archive_read_close(a);
	archive_read_free(a);
	archive_write_close(ext);
	archive_write_free(ext);
	return 0;
}
