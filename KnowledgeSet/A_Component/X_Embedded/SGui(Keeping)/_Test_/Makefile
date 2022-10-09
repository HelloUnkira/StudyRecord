
#一个简单的Makefile

SGuiSystem = -lpthread

SGuiHeader = -I ../                     \
             -I ../SGui_Core/           \
             -I ../SGui_Draw/           \
             -I ../SGui_Port/           \
             -I ../SGui_Layout          \
             -I ../SGui_Run

test: test.o
	gcc -o test   ./*.o  $(SGuiSystem)

test.o:

	gcc -c ../SGui_Core/*.c     $(SGuiHeader)
	gcc -c ../SGui_Draw/*.c     $(SGuiHeader)
	gcc -c ../SGui_Port/*.c     $(SGuiHeader)
	gcc -c ../SGui_Layout/*.c   $(SGuiHeader)
	gcc -c ../SGui_Run/*.c      $(SGuiHeader)
	gcc -c  ./*.c               $(SGuiHeader)
