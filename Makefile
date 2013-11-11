CC      = g++
C	= cpp
H	= h

CFLAGS = -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lm -L/usr/local/lib -lGLU
  endif
endif

HFILES = Quaternion.${H} RBSystem.${H} StateVector.${H} RigidBody.${H} Geometry.${H} EntryTable.${H} OverlapList.${H} ExtentList.${H} ContactList.${H} Plane.${H} Witness.${H} Matrix.${H} Vector.${H} Utility.${H}
OFILES = Quaternion.o RBSystem.o StateVector.o RigidBody.o Geometry.o EntryTable.o OverlapList.o ExtentList.o ContactList.o Plane.o Witness.o Matrix.o Vector.o Utility.o
PROJECT = rbody

${PROJECT}:	${PROJECT}.o ${OFILES}
	${CC} ${CFLAGS} -o ${PROJECT} ${PROJECT}.o ${OFILES} ${LDFLAGS}

${PROJECT}.o: ${PROJECT}.${C} ${HFILES}
	${CC} ${CFLAGS} -c ${PROJECT}.${C}

RBSystem.o: RBSystem.${C} ${HFILES} 
	${CC} ${CFLAGS} -c RBSystem.${C}

StateVector.o: StateVector.${C} StateVector.${H} Vector.${H} Utility.${H}
	${CC} ${CFLAGS} -c StateVector.${C}

RigidBody.o: RigidBody.${C} RigidBody.${H} Geometry.${H} Plane.${H} Witness.${H} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} ${CFLAGS} -c RigidBody.${C}

Geometry.o: Geometry.${C} Geometry.${H} Plane.${H} Vector.${H} Utility.${H} 
	${CC} ${CFLAGS} -c Geometry.${C}
	
EntryTable.o: EntryTable.${C} EntryTable.${H} 
	${CC} ${CFLAGS} -c EntryTable.${C}

OverlapList.o: OverlapList.${C} OverlapList.${H} EntryTable.${H} RigidBody.${H} Geometry.${H} Plane.${H} Witness.${H} Matrix.${H} Vector.${H} Utility.${H}
	${CC} ${CFLAGS} -c OverlapList.${C}

ExtentList.o: ExtentList.${C} ExtentList.${H} OverlapList.${H} EntryTable.${H} RigidBody.${H} Geometry.${H} Plane.${H} Witness.${H} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} ${CFLAGS} -c ExtentList.${C}

ContactList.o: ContactList.${C} ContactList.${H} OverlapList.${H} EntryTable.${H} RigidBody.${H} Geometry.${H} Plane.${H} Vector.${H} Utility.${H} 
	${CC} ${CFLAGS} -c ContactList.${C}

Plane.o: Plane.${C} Plane.${H} Vector.${H} Utility.${H} 
	${CC} ${CFLAGS} -c Plane.${C}

Witness.o: Witness.${C} Witness.${H} Plane.${H} Vector.${H} Utility.${H} 
	${CC} ${CFLAGS} -c Witness.${C}

Quaternion.o: Quaternion.${C} Quaternion.${H} Matrix.${H} Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c Quaternion.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} ${CFLAGS} -c Matrix.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H} 
	${CC} ${CFLAGS} -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} ${CFLAGS} -c Utility.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm core.*; rm *.o; rm *~; rm ${PROJECT}
