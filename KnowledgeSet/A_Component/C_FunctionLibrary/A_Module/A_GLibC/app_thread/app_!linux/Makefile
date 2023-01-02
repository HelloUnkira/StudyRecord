
#一个简单的Makefile

app_dbg = -g
app_std = -std=gnu99

app_system = -lpthread
app_header = -I ../             \
             -I ../app_os/      \
             -I ../app_sys/     \
             -I ../app_thread/  \


main: main.o
	gcc $(app_dbg) -o main ./*.o $(app_system)


main.o:
	gcc -c ../app_os/*.c        $(app_header) $(app_std)
	gcc -c ../app_sys/*.c       $(app_header) $(app_std)
	gcc -c ../app_thread/*.c    $(app_header) $(app_std)
	gcc -c ./*.c                $(app_header) $(app_std)


clean:
	rm ./*.o ./main
