OBJS 	= main.o visuals.o
SOURCE	= main.cpp visuals.cpp
HEADER  = visuals.h
OUT  	= a.out
CC		= g++
FLAGS 	= -g -Wall -O2
LIBS = -lglut -lGL -lGLU -lGLEW -lm
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(OUT) $(FLAGS)

main.o: main.cpp
	$(CC) -c main.cpp $(FLAGS) $(LIBS) 

visuals.o: visuals.cpp
	$(CC) -c visuals.cpp $(FLAGS) $(LIBS) 

# clean house
clean:
	rm -f $(OBJS) $(OUT)

clena:
	rm -f $(OBJS) $(OUT)

claen:
	rm -f $(OBJS) $(OUT)

celan:
	rm -f $(OBJS) $(OUT)
