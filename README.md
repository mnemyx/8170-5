PROJECT
==================================================================

	Gina Guerrero

	CpSc8170 - Fall 2013

	Project #5 - Rigid Body Simulation

	C++/OpenGL/ImageMagick



DESCRIPTION
==================================================================

	Rigid Body Simulation

	Gauss/Matrix/Vector/Utility/Quaternion by Dr. House

	Usage: rb param

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
	RigidB.cpp, RidigB.h


MISC FILES
==================================================================
	README.md
	Makefile
	MakeSpace.h
	Quaternion.cpp, Quaternion.h
	Matrix.cpp, Matrix.h
	Utility.cpp, Utility.h
	Vector.cpp, Vector.h
	gauss.cpp, gauss.h
	param


SAMPLE OUTPUT FILES
==================================================================
	N/A


TOTAL FILE COUNT
==================================================================
	??
