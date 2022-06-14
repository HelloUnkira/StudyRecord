
#一个简单的Makefile

SGuiSystem = -lpthread

SGuiHeader = -I ../Library/LogMessage/                  \
             -I ../Library/MemoryManage/                \
             -I ../Library/PriorityQueue/               \
             -I ../Library/SoftwareTimer/               \
             -I ../Library/SynchronizationSemantic/     \
             -I ../Port/MiniOSTimerPort/                \
             -I ../MiniOS/
             
             
test: test.o
	gcc -o test   ./*.o  $(SGuiSystem)

test.o:

	gcc -c ../Library/LogMessage/*.c                $(SGuiHeader)
	gcc -c ../Library/MemoryManage/*.c              $(SGuiHeader)
	gcc -c ../Library/PriorityQueue/*.c             $(SGuiHeader)
	gcc -c ../Library/SoftwareTimer/*.c             $(SGuiHeader)
#	gcc -c ../Library/SynchronizationSemantic/*.c   $(SGuiHeader)
	gcc -c ../Port/MiniOSTimerPort/*.c              $(SGuiHeader)
	gcc -c ../MiniOS/*.c                            $(SGuiHeader)
	gcc -c ../Test/*.c                              $(SGuiHeader)
