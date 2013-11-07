PROJECT
==================================================================

	Gina Guerrero

	CpSc8170 - Fall 2013

	Project #4 - Springy Meshes (Butterfly Wings Simulation)

	C++/OpenGL/ImageMagick



DESCRIPTION
==================================================================

	Springy Meshes - Butterfly Wings Simulation

	Gauss/Matrix/Vector/Utility by Dr. House
	Model Class modified to create an isosphere & plane

	Color/Face/File/Group/ImageFile/Line/MakeSpace/Material
	MTLFile/OBJFile/Pixmap/PolySurf by Dr. House
	These were re-used from CpSc 405 - Spring 2011's Hwk #6

	OBJ/MTL files generated in Maya

	Usage: particles [param_filename] [obj_file.obj]
	ex,    particles p_c testcloth.obj

KEY COMMANDS
==================================================================
	d or D		start simulation
	s or S		switches simulation from step to continous
	w or W		switches simulation from wireframe to shaded
	p or P		switches from ortho to perspective views (default: perspective)
	r or R      	resets the simulation, so if you change parameters - this will re-read them
	q or ESC	quit


MOUSE COMMANDS
==================================================================
	CAMERA: 	left button		 |	middle button  	|	right button
	left drag	(-) rotation: model's y	 |  (+)-r: camera's y	|  (+) translation: camera's z
	right drag	(+) rotation: model's y	 |  (-)-r: camera's y	|  (-) translation: camera's z
	down drag	(+) rotation: model's x	 |  (+)-r: camera's x	|  (+) translation: camera's z
	up drag		(-) rotation: model's x	 |  (-)-r: camera's x	|  (-) translation: camera's z


FILES
==================================================================
	particles.cpp (main program)
	State.cpp, State.h
	Strut.cpp, Strut.h
	Hinge.cpp, Hinge.h


MISC FILES
==================================================================
	README.md
	Makefile
	MakeSpace.h
	Color.cpp, Color.h
	Face.cpp, Face.h
	File.cpp, File.h
	Group.cpp, Group.h
	ImageFile.cpp, ImageFile.h
	Line.cpp, Line.h
	Material.cpp, Material.h
	MTLFile.cpp, MTLFile.h
	OBJFile.cpp, OBJFile.h
	Pixmap.cpp, Pixmap.h
	PolySurf.cpp, PolySurf.h
	Matrix.cpp, Matrix.h
	Utility.cpp, Utility.h
	Vector.cpp, Vector.h
	gauss.cpp, gauss.h
	p


SAMPLE OUTPUT FILES
==================================================================
	N/A


TOTAL FILE COUNT
==================================================================
	~42
