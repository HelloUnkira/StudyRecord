
#一个简单的Makefile

SGuiSystem = -lpthread

SGuiHeader = -I ../                             \
             -I ../SGui_System/                 \
             -I ../SGui_Core/                   \
             -I ../SGui_Driver/                 \
             -I ../SGui_Engine/                 \
             -I ../SGui_Layout/                 \
             -I ../SGui_Draw/                   \
             -I ../SGui_Draw/SGUI_DrawFont      \
             -I ../SGui_Draw/SGUI_DrawPicture   \
             -I ../SGui_Draw/SGUI_DrawGraph     \


test: test.o
	gcc -o test   ./*.o  $(SGuiSystem)

test.o:

	gcc -c ../SGui_System/*.c                   $(SGuiHeader)
	gcc -c ../SGui_Core/*.c                     $(SGuiHeader)
	gcc -c ../SGui_Driver/*.c                   $(SGuiHeader)
	gcc -c ../SGui_Engine/*.c                   $(SGuiHeader)
	gcc -c ../SGui_Layout/*.c                   $(SGuiHeader)
	gcc -c ../SGui_Draw/SGUI_DrawFont/*.c       $(SGuiHeader)
	gcc -c ../SGui_Draw/SGUI_DrawPicture/*.c    $(SGuiHeader)
	gcc -c ../SGui_Draw/SGUI_DrawGraph/*.c      $(SGuiHeader)
	gcc -c ../SGui_Draw/*.c                     $(SGuiHeader)
	gcc -c  ./*.c                               $(SGuiHeader)
