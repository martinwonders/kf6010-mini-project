Image=BUILD/K64F/GCC_ARM/kf6010-mini-project.bin

all: crossing.pdf $(Image)

crossing.pdf: crossing.tex
	latexmk -pdf -quiet crossing


$(Image): mbed-os src/main.cpp
	mbed compile

mbed-os: mbed-os.lib
	mbed deploy

.PHONY: flash
flash: 
	mbed compile --flash

