FLAGS =	-std=c++0x -pthread -O2 -Wall -fmessage-length=0

OBJS =		main.o mqueue.o

LIBS = -lpthread

TARGET =	mqueue

$(TARGET):	$(OBJS)
	g++ -o $(TARGET) $(OBJS) $(LIBS)

main.o	:	main.cpp mqueue.h
	g++ -c $(FLAGS) main.cpp

mqueue.o	:	mqueue.cpp mqueue.h
	g++ -c $(FLAGS) mqueue.cpp

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
