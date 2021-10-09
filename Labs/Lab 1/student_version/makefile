# -------------------------------------------------------------------------
#  Default rule to build the final executable (life)
# -------------------------------------------------------------------------
life: world.o agent.o driver.o 
	g++ -Wall -std=c++11 -o life driver.o world.o agent.o `pkg-config --cflags --libs opencv4`

# -------------------------------------------------------------------------
#  Below are rules to build each object (.o) file.  Note that each object file
#  has a specific list of files that it "needs" to be there in order 
#  to be compiled.
# -------------------------------------------------------------------------

# -------------------------------------------------------------------------
#  This rule says that driver.o depends on driver.cpp and world.h/agent.h
#  and explains that to "make" driver.o you must run "g++ -ansi -Wall -c driver.c"
# -------------------------------------------------------------------------
driver.o: driver.cpp world.h agent.h
	g++ -Wall -std=c++11 -c driver.cpp `pkg-config --cflags --libs opencv4`

# -------------------------------------------------------------------------
#  This rule says that world.o depends on world.cpp, world.h 
#  and agent.h and is "made" by running "g++ -ansi -Wall -c world.cpp"
# -------------------------------------------------------------------------
world.o: world.cpp world.h agent.h
	g++ -Wall -std=c++11 -c world.cpp `pkg-config --cflags --libs opencv4`

# -------------------------------------------------------------------------
#  This rule says that agent.o depends on agent.cpp and agent.h
#  and is "made" by running "g++ -ansi -Wall -c agent.cpp"
# -------------------------------------------------------------------------
agent.o: agent.cpp agent.h
	g++ -Wall -std=c++11 -c agent.cpp

clean:
	rm *.o *.jpg life

run:
	./life