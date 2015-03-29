TARGET = libfrog_all.so

PREFIX_BIN =

CXX=g++  

ifeq (0,${debug}) 
	CPPFLAGS = -DNDEBUG -g2 -O2 
else
	CPPFLAGS = -ggdb -O2 
endif 

CPPFLAGS += -DTIXML_USE_STL -Wall 

#CPPFLAGS += -I../boost_1_53_0/include
CPPFLAGS += -I/Users/wonghoifung/myenvironment/include/ 
#CPPFLAGS += -I../mysql-connector-c-6.0.2-linux-glibc2.3-x86-64bit/include/
CPPFLAGS += -I/usr/local/include/mysql/
#CPPFLAGS += -I../postgresql-9.3.5/include/
CPPFLAGS += -I/usr/local/include/
CPPFLAGS += -I/Users/wonghoifung/Desktop/libmemcached/1.0.18/include/

#LIBS = -lboost_thread -lboost_date_time -lboost_system -lrt -lmysqlclient -lpq -lmemcached
LIBS = -lboost_thread -lboost_date_time -lboost_system -lmysqlclient -lpq -lmemcached
LINKFLAGS = -L/usr/local/lib 
#LINKFLAGS += -L../boost_1_53_0/lib 
LINKFLAGS += -L/Users/wonghoifung/myenvironment/lib/
#LINKFLAGS += -L../mysql-connector-c-6.0.2-linux-glibc2.3-x86-64bit/lib/
LINKFLAGS += -L/usr/local/lib/
#LINKFLAGS += -L../postgresql-9.3.5/lib/
LINKFLAGS += -L/Users/wonghoifung/Desktop/libmemcached/1.0.18/lib/

INCLUDEDIRS = ./ ./utils/ ./generic/ ./db/ ./cache/ ./scriptbinding/ ./scriptbinding/luasrc/
INCLUDES = $(foreach tmp, $(INCLUDEDIRS), -I $(tmp))

MYSOURCEDIRS = ./ ./utils/ ./generic/ ./db/ ./cache/ ./scriptbinding/ ./scriptbinding/luasrc/

SOURCEDIRS = $(MYSOURCEDIRS)


C_SOURCES = $(foreach tmp, $(SOURCEDIRS), $(wildcard $(tmp)*.c))
C_OBJS = $(patsubst %.c, %.o, $(C_SOURCES))


CPP_SOURCES = $(foreach tmp, $(SOURCEDIRS), $(wildcard $(tmp)*.cpp))
CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SOURCES))

all:compile
.PHONY :all

.c.o:
	$(CC) -fPIC -c -o $*.o $(CFLAGS) $(INCLUDES) $*.c
.cpp.o:
	$(CXX) -fPIC -c -o $*.o $(CPPFLAGS) $(INCLUDES) $*.cpp

compile: $(CPP_OBJS) $(C_OBJS) $(OTHERS_C_OBJS) $(OTHERS_CPP_OBJS)
	$(CXX) $(LINKFLAGS) -shared -o $(TARGET) $^ $(LIBS)

.PHONY : clean
clean:
	rm -f $(CPP_OBJS) $(C_OBJS)
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(PREFIX_BIN)

uninstall:
	rm -f $(PREFIX)/$(PREFIX_BIN)

rebuild: clean




