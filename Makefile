# Export Standalone Android Toolchains path to STANDALONE_NDK

CC=clang++
ACC=$(STANDALONE_NDK)/bin/clang++

all: clean trimfile

clean:
	@rm -rf out

configure:
	@mkdir -p out

trimfile: configure
	@$(CC) trimfile.cpp -o out/trimfile

trimfile_android: configure
	@$(ACC) trimfile.cpp -o out/trimfile -std=c++11 -pie
