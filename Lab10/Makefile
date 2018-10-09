all: rpg

rpg.o: rpg.c
	gcc -c rpg.c

Player.o: Player.c Player.h
	gcc -c Player.c

Game.o: Game.c Game.h
	gcc -c Game.c

rpg: rpg.o Game.o Game.h Player.o Player.h rpg.o UNIXBOARD.h
	gcc rpg.o Game.o Player.o -o rpg

clean:
	rm *.o