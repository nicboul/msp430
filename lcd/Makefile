TARGET	:= lcd.elf
CC		:= msp430-gcc
CFLAGS	:= -0s -Wall -mmcu=msp430g2553
OBJS	:= main.o lcd.o

$(TARGET): $(OBJS)
		$(CC) -o $(TARGET) $(CFLAGS) $(OBJS)

clean:
		rm -rf $(TARGET) $(OBJS)

prog: $(TARGET)
	mspdebug -q rf2500 "prog $(TARGET)"
