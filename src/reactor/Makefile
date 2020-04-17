
TARGET = libreactor.a
FLAGS = -g --std=c++11

INC = -I. -I../common
LIBS = -lpthread
SRCS = $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))

.PHONY: all

$(OBJS): $(SRCS)
	g++ $(FLAGS) $(INC) -c $^

all: $(OBJS)
	ar rc $(TARGET) $(OBJS)

clean:
	rm -rf *.o
