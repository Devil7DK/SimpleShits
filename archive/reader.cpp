#include <iostream>

#include <archive.h>
#include <archive_entry.h>

using namespace std;

int main(int argc, char** argv)
{
    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    r = archive_read_open_filename(a, argv[1], 10240); // Note 1
    if (r != ARCHIVE_OK)
      return 1;
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
      cout << archive_entry_pathname(entry) << endl;
      archive_read_data_skip(a);
    }
    r = archive_read_free(a);
    if (r != ARCHIVE_OK)
      return 1;

}
