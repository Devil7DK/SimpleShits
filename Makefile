# Export Standalone Android Toolchains path to STANDALONE_NDK

CC=clang++
ACC=$(STANDALONE_NDK)/bin/clang++
SUCCESS="Successfully compiled..."

all: clean trimfile reader extracter

clean:
	@rm -rf out

configure:
	@mkdir -p outc

trimfile: configure
	@$(CC) trimfile.cpp -o out/trimfile && echo $(SUCCESS)

trimfile_android: configure
	@$(ACC) trimfile.cpp -o out/trimfile -std=c++11 -pie && echo $(SUCCESS)

reader: configure
	@$(CC) archive/reader.cpp -o out/reader -larchive && echo $(SUCCESS)

extracter: configure
	@$(CC) archive/extracter.cpp -o out/extracter -larchive && echo $(SUCCESS)

compresser: configure
	@$(CC) archive/compresser.cpp utils/dir.cpp -o out/compresser -larchive -I./ && echo $(SUCCESS)
