TARGET = test

PREFIX_BIN =

CXX=g++  

ifeq (0,${debug}) 
	CPPFLAGS = -DNDEBUG -g2 -O2 
else
	CPPFLAGS = -ggdb -O2 
endif 

#CFLAGS += -DLUA_USE_MKSTEMP 
CFLAGS += -DLUA_USE_POSIX

CPPFLAGS += -Wall -I../../../mysql-connector-c-6.0.2-linux-glibc2.3-x86-64bit/include/
CPPFLAGS += -I../../../boost_1_53_0/include/ 

LIBS = -lboost_thread -lboost_date_time -lboost_system -lmysqlclient -lcurl -lpthread -lrt
LINKFLAGS = -L../../../mysql-connector-c-6.0.2-linux-glibc2.3-x86-64bit/lib/ -L/usr/local/lib 
LINKFLAGS += -L../../../boost_1_53_0/lib/

INCLUDEDIRS = ./ ./luasrc/
INCLUDES = $(foreach tmp, $(INCLUDEDIRS), -I $(tmp))

MYSOURCEDIRS = ./ ./luasrc/

SOURCEDIRS = $(MYSOURCEDIRS)


C_SOURCES = $(foreach tmp, $(SOURCEDIRS), $(wildcard $(tmp)*.c))
C_OBJS = $(patsubst %.c, %.o, $(C_SOURCES))


CPP_SOURCES = $(foreach tmp, $(SOURCEDIRS), $(wildcard $(tmp)*.cpp))
CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SOURCES))

all:compile
.PHONY :all

.c.o:
	$(CC) -c -o $*.o $(CFLAGS) $(INCLUDES) $*.c
.cpp.o:
	$(CXX) -c -o $*.o $(CPPFLAGS) $(INCLUDES) $*.cpp

compile: $(CPP_OBJS) $(C_OBJS) $(OTHERS_C_OBJS) $(OTHERS_CPP_OBJS)
	$(CXX) $(LINKFLAGS) -o $(TARGET) $^ $(LIBS)

.PHONY : clean
clean:
	rm -f $(CPP_OBJS) $(C_OBJS)
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(PREFIX_BIN)

uninstall:
	rm -f $(PREFIX)/$(PREFIX_BIN)

rebuild: clean




