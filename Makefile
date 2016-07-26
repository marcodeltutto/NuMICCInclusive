all:
	`root-config --cxx --cflags --glibs` -fopenmp -I/home/deltutto/NuMICCInclusive -L/home/deltutto/NuMICCInclusive -o NuMICCInclusive NuMICCInclusive.cpp SelectionTools.cpp AnaNuMI.C `root-config --glibs`	
