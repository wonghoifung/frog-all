TARGET = server 
# SERVER_TARGET = server 
# CLIENT_TARGET = client 

PREFIX_BIN =

CXX=g++  

ifeq (0,${debug}) 
	CPPFLAGS = -DNDEBUG -g2 -O2
else
	CPPFLAGS = -ggdb -O2 
endif 

CPPFLAGS += -Wall -std=c++11
CPPFLAGS += -I./
CPPFLAGS += -I/home/alber/env/caf/include/

LIBS = -lcaf_core -lcaf_io -lpthread
LINKFLAGS = -L/usr/local/lib
LINKFLAGS += -L/home/alber/env/caf/lib/

INCLUDEDIRS = ./
INCLUDES = $(foreach tmp, $(INCLUDEDIRS), -I $(tmp))

MYSOURCEDIRS = ./

SOURCEDIRS = $(MYSOURCEDIRS)

CPP_SOURCES = ./server.cpp
CPP_OBJS = ./server.o

# SERVER_SOURCES = ./server.cpp
# SERVER_OBJS = ./server.o

# CLIENT_SOURCES = ./client.cpp
# CLIENT_OBJS = ./client.o

# all:compile compile_server compile_client
all:compile
.PHONY :all

.c.o:
	$(CC) -c -o $*.o $(CFLAGS) $(INCLUDES) $*.c
.cpp.o:
	$(CXX) -c -o $*.o $(CPPFLAGS) $(INCLUDES) $*.cpp

compile: $(CPP_OBJS) $(C_OBJS) $(OTHERS_C_OBJS) $(OTHERS_CPP_OBJS)
	$(CXX) $(LINKFLAGS) -o $(TARGET) $^ $(LIBS)

# compile_server: $(SERVER_OBJS)
# 	$(CXX) $(LINKFLAGS) -o $(SERVER_TARGET) $^ $(LIBS)

# compile_client: $(CLIENT_OBJS)
# 	$(CXX) $(LINKFLAGS) -o $(CLIENT_TARGET) $^ $(LIBS)

.PHONY : clean
clean:
	rm -f $(CPP_OBJS) $(C_OBJS)
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(PREFIX_BIN)

uninstall:
	rm -f $(PREFIX)/$(PREFIX_BIN)

rebuild: clean

