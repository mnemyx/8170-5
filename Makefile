CC      = g++
C	= cpp
H	= h

CFLAGS = -g
LFLAGS = -g 

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lm -L /user/local/lib -lGLU
  endif
endif

HFILES = StateVector.${H} RBSystem.${H} Strut.${H} Quaternion.${H} RBody.${H} Model.${H} Matrix.${H} Vector.${H} Utility.${H}
OFILES = StateVector.o RBSystem.o Strut.o Quaternion.o RBody.o Model.o Matrix.o Vector.o Utility.o
PROJECT = rb

${PROJECT}:	${PROJECT}.o ${OFILES}
	${CC} ${CFLAGS} -o ${PROJECT} ${PROJECT}.o ${OFILES} ${LDFLAGS}

${PROJECT}.o: ${PROJECT}.${C} ${HFILES}
	${CC} ${CFLAGS} -c -Wall ${PROJECT}.${C}

StateVector.o: StateVector.${C} StateVector.${H}
	${CC} ${CFLAGS} -c StateVector.${C}

Strut.o: Strut.${C} Strut.${H}
	${CC} ${CFLAGS} -c Strut.${C}

RBSystem.o: RBSystem.${C} RBSystem.${H} RBody.${H}
	${CC} ${CFLAGS} -c RBSystem.${C}

RBody.o: RBody.${C} RBody.${H}
	${CC} ${CFLAGS} -c RBody.${C}

Model.o: Model.${C} Model.${H}
	${CC} ${CFLAGS} -c Model.${C}

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
