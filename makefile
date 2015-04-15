CC = gcc
COPS = -c -g -Wall

all:  parser
parser:  driver.o scan.o parse.o
	$(CC) -o parser driver.o scan.o parse.o
driver.o:  driver.c
	$(CC) $(COPS) driver.c
scan.o:  scan.c
	$(CC) $(COPS) scan.c 
parse.o:  parse.c
	$(CC) $(COPS) parse.c
clean:
	rm -f -v *~ *o
