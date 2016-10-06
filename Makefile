SRC_PATH=source
FILES=source/SelectionTools.cpp source/Spectrum.cpp source/Spectrum2D.cpp source/AnaNuMI.C source/PlotHandler.cpp
SOURCES = $(FILES:%.cpp=$(SRC_PATH)/%.cpp)

CFLAGS=-Wall

all:
	`root-config --cxx --cflags --glibs` ${CFLAGS} -fopenmp -I./include -o NuMICCInclusive NuMICCInclusive.cpp ${FILES} `root-config --glibs`	
