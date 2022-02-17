
#一个简单的Makefile

SGuiSystem = -lpthread

SGuiHeader = -I ../             \
             -I ../SGuiCommon/  \
             -I ../SGuiPort/    \
             -I ../SGuiCore/    \
             -I ../SGuiDraw/

test: test.o
	gcc -o test   ./*.o  $(SGuiSystem)

test.o:

	gcc -c ../SGuiCommon/SGuiCommon/*.c     $(SGuiHeader)
	gcc -c ../SGuiCore/SGuiCore/*.c         $(SGuiHeader)
	gcc -c ../SGuiDraw/SGuiDraw/*.c         $(SGuiHeader)
	gcc -c ../SGuiPort/SGuiPort/*.c         $(SGuiHeader)
	gcc -c  ./*.c                           $(SGuiHeader)



