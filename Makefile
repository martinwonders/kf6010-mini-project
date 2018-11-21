Image=BUILD/K64F/GCC_ARM/kf6010-mini-project.bin

all: crossing.pdf $(Image)

crossing.pdf:crossing.tex
	latexmk -pdf -quiet crossing


$(Image):src/main.cpp
	mbed compile
