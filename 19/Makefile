NAME?=test1
MCU?=atmega88
PROG=${NAME}-${MCU}
CFLAGS=-Os -mmcu=${MCU} -Wall

SRCS=main.c
OBJS=${SRCS:%.c=%.o}

CC=avr-gcc
OBJDUMP=avr-objdump
OBJCOPY=avr-objcopy

.SUFFIXES: .c .o .S .s

.c.o:
	${CC} ${CFLAGS} -c -o $@ $<

.c.s:
	${CC} ${CFLAGS} -S -o $@ $<

.S.o:
	${CC} ${CFLAGS} -c -o $@ $<

.s.o:
	${CC} ${CFLAGS} -c -o $@ $<

all: ${PROG}.elf ${PROG}.hex

${PROG}.elf: ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS}
	${OBJDUMP} -d $@ > ${PROG}.dump

${PROG}.hex: ${PROG}.elf
	${OBJCOPY} -I elf32-avr -O ihex ${PROG}.elf ${PROG}.hex

clean:
	rm -f *.s *.o *.dump *.hex *.elf

edit:
	avrdude -c usbasp -p m88 -t

write:
	avrdude -c usbasp -p m88 -U flash:w:${PROG}.hex
