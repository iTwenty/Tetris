APP		= tetris
SRCS	= tetris.c
OBJS	= $(SRCS:.c=.o)
CFLAGS	= -Wall -pedantic --std=c99  -g `sdl-config --cflags`
LDFLAGS	= `sdl-config --libs`
CC	= gcc
RM	= rm

all : $(APP)

$(APP) : $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJS) : $(SRCS)
	$(CC) -c $^ $(CFLAGS)

clean:
	$(RM) $(OBJS)
	$(RM) $(APP)
