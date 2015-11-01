BUILD := executable
INCDIR = ./include
SRCDIR = src
OBJS_TEST = util.o log_record.o log_appender.o log_container.o log_container_iterator.o main_test.o
OBJS = util.o log_record.o log_appender.o log_container.o log_container_iterator.o main.o
CXX = clang++
DEBUG = -g
LFLAGS = -Wall $(DEBUG) -lpthread
cxxflags.test := -Wall -O1 -c $(DEBUG) -pthread -std=c++14 #-fsanitize=thread
cxxflags.executable := -Wall -O3 -c -std=c++14
CXXFLAGS := ${cxxflags.${BUILD}}

all: $(BUILD)

test: $(OBJS_TEST)
	@echo "Build test..."
	$(CXX) $(OBJS_TEST) -o main $(LFLAGS)

executable: $(OBJS)
	@echo "Build executable..."
	$(CXX) $(OBJS) -o main $(LFLAGS)

util.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/util.cpp

log_record.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/log_record.cpp

log_appender.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/log_appender.cpp

log_container.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/log_container.cpp
	
log_container_iterator.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $(SRCDIR)/log_container_iterator.cpp

main.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) main.cpp
	   
main_test.o:
	$(CXX) $(CXXFLAGS) -I$(INCDIR) main_test.cpp

clean:
	rm -f *.o main; rm -rf main.dSYM