CC	= g++
CFLAGS 	= -g
TARGET	= readGML
OBJS	= tes.o

$(TARGET):	$(OBJS)
		$(CC) -o $(TARGET) $(OBJS)

tes.o:	tes.cpp timer.h
		$(CC) $(CFLAGS) -c tes.cpp

clean:
		/bin/rm -f *.o $(TARGET)

