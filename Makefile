CC=clang++

all: clean trimfile

clean:
	@rm -rf out

configure:
	@mkdir -p out

trimfile: configure
	@$(CC) trimfile.cpp -o out/trimfile
