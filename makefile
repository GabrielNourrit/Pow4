CC=gcc
OPTIONS=-Wall `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV`
PART2=`pkg-config --libs-only-l MLV`

prog : Game_Base.o Int_Graph_MLV.o Main.o
	$(CC) $(OPTIONS) Game_Base.o Int_Graph_MLV.o Main.o $(PART2) -o Pow4

Main.o : Main.c type.h
	$(CC) $(OPTIONS) -c Main.c $(PART2)

Game_Base.o : Game_Base.c Game_Base.h type.h 
	$(CC) $(OPTIONS) -c Game_Base.c $(PART2)

Int_Graph_MLV.o : Int_Graph_MLV.c Int_Graph_MLV.h type.h
	$(CC) $(OPTIONS) -c Int_Graph_MLV.c $(PART2)

clean :
	rm *.o *~

