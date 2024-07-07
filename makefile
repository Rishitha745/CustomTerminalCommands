all:take_input.o mycp.o myls.o mymv.o mygrep.o myps.o
	gcc take_input.o mycp.o myls.o mymv.o mygrep.o myps.o -lreadline -lm
take_input.o:take_input.c
	gcc -c take_input.c 
mycp.o:mycp.c
	gcc -c mycp.c
myls.o:myls.c
	gcc -c myls.c
mymv.o:mymv.c
	gcc -c mymv.c
mygrep.o:mygrep.c
	gcc -c mygrep.c
myps.o:myps.c
	gcc -c myps.c