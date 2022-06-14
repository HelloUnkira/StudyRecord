
#一个简单的Makefile

SGuiSystem = -lpthread

SGuiHeader = -I ../MemoryManage/        \
             -I ../LogMessage/          \
             -I ../SoftwareTimer/       \
             -I ../MiniOS/
             
             
test: test.o
	gcc -o test   ./*.o  $(SGuiSystem)

test.o:

	gcc -c ../MemoryManage/*.c      $(SGuiHeader)
	gcc -c ../LogMessage/*.c        $(SGuiHeader)
	gcc -c ../SoftwareTimer/*.c     $(SGuiHeader)
	gcc -c ../MiniOS/*.c            $(SGuiHeader)
	gcc -c ../Test/*.c              $(SGuiHeader)
