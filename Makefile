# Export Standalone Android Toolchains path to STANDALONE_NDK

CC=clang++
ACC=$(STANDALONE_NDK)/bin/clang++
SUCCESS="Successfully compiled..."
ifdef DEBUG
ACFLAGS= -g
endif

all: clean trimfile reader extracter compresser

clean:
	@rm -rf out

configure:
	@mkdir -p out

trimfile: configure
	@$(CC) $(ACFLAGS) trimfile.cpp -o out/trimfile && echo $(SUCCESS)

trimfile_android: configure
	@$(ACC) $(ACFLAGS) trimfile.cpp -o out/trimfile -std=c++11 -pie && echo $(SUCCESS)

reader: configure
	@$(CC) $(ACFLAGS) archive/reader.cpp -o out/reader -larchive && echo $(SUCCESS)

extracter: configure
	@$(CC) $(ACFLAGS) archive/extracter.cpp -o out/extracter -larchive && echo $(SUCCESS)

compresser: configure
	@$(CC) $(ACFLAGS) archive/compresser.cpp utils/dir.cpp -o out/compresser -larchive -I./ && echo $(SUCCESS)
