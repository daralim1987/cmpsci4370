CC	= g++ -std=c++11
CFLAGS 	= -g
TARGET	= readGML
OBJS	= main.o

$(TARGET):	$(OBJS)
		$(CC) -o $(TARGET) $(OBJS)

main.o:	main.cpp timer.h
		$(CC) $(CFLAGS) -c main.cpp

clean:
		/bin/rm -f *.o $(TARGET)

