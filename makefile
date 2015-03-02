
TARGET=timer_util_test
ROOT_DIR=$(shell pwd)
THRIFT_CMD=thrift
THRIFT_DIR=$(ROOT_DIR)/thrift
THRIFT_GEN_DIR=$(ROOT_DIR)/thrift/gen-cpp
CPPFLAGS=-std=c++11 -g -Werror -O0
CXX=/opt/rh/devtoolset-2/root/usr/bin/g++

DEP_LIBS=-lfcgi\
	 -lpthread\
	 -lhiredis\
	 -lthriftnb\
	 -lthrift\
	 -ljson-c\
	 -lglog\
	 -lgflags\
	 -lactivemq-cpp\
	 -levent\
	 -lrt\
	 -lboost_system\
	 -lboost_filesystem\
	 -L/usr/lib\
	 -L/usr/local/lib\

DEP_INCS=-I/usr/include/thrift\
	 -I/usr/local/include/activemq-cpp-3.8.3\
	 -I/usr/include/boost\
	 -I/usr/include\
	 -I/usr/local/include/hiredis\
	 -I/usr/local/include/json-c\
	 -I/usr/local/include\
	 -I./thrift/gen-cpp\
	 -I./thrift/client\
	 -I./thrift/server\
	 -I./thrift\
	 -I./config\
	 -I./core\
	 -I./handler\
	 -I./util\
	 -I./hiredis\
	 -I./activemq\

DIRS=$(shell find . -maxdepth 3 -type d)
FILES=$(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
OBJS=$(patsubst %.cpp,%.o, $(FILES))

VPATH = .
.cpp.o:
	$(CXX) -c $(CPPFLAGS) $< -o $@ $(DeP_LIBS) $(DEP_INCS) 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $@ $(OBJS) $(DEP_LIBS) $(DEP_INCS)

clean:
	rm -rf $(TARGET) $(OBJS) $(THRIFT_GEN_DIR) $(THRIFT_DIR)/*.thrift
