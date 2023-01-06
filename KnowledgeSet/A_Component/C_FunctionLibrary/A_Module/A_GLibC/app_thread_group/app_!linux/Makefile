
#一个简单的Makefile

app_dbg = -g
app_std = -std=gnu99

app_system = -lpthread

app_header = -I ../                                                         \
             -I ../app_arch/                                                \
             -I ../app_arch/app_arch_adaptor                                \
             -I ../app_arch/app_arch_ext_mem                                \
             -I ../app_os/                                                  \
             -I ../app_os/app_os_adaptor                                    \
             -I ../app_os/app_os_ext_mem                                    \
             -I ../app_sys/                                                 \
             -I ../app_sys/app_sys_pipe                                     \
             -I ../app_sys/app_sys_ring_buffer                              \
             -I ../app_thread/                                              \
             -I ../app_thread/app_thread_master                             \
             -I ../app_thread/app_thread_master/app_thread_adaptor          \
             -I ../app_thread/app_thread_master/app_module_ext_mem          \
             -I ../app_thread/app_thread_master/app_module_system           \
             -I ../app_thread/app_thread_mix_custom                         \
             -I ../app_thread/app_thread_mix_custom/app_module_alarm        \
             -I ../app_thread/app_thread_mix_custom/app_module_clock        \
             -I ../app_thread/app_thread_mix_custom/app_module_countdown    \
             -I ../app_thread/app_thread_mix_custom/app_module_dump         \
             -I ../app_thread/app_thread_mix_custom/app_module_stopwatch    \
             -I ../app_thread/app_thread_mix_custom/app_module_world_time   \
             -I ../app_thread/app_thread_mix_irq                            \

app_source = ./*.c                                                          \
             ../app_arch/*.c                                                \
             ../app_arch/app_arch_adaptor/*.c                               \
             ../app_arch/app_arch_ext_mem/*.c                               \
             ../app_os/*.c                                                  \
             ../app_os/app_os_adaptor/*.c                                   \
             ../app_os/app_os_ext_mem/*.c                                   \
             ../app_sys/*.c                                                 \
             ../app_sys/app_sys_pipe/*.c                                    \
             ../app_sys/app_sys_ring_buffer/*.c                             \
             ../app_thread/*.c                                              \
             ../app_thread/app_thread_master/*.c                            \
             ../app_thread/app_thread_master/app_thread_adaptor/*.c         \
             ../app_thread/app_thread_master/app_module_ext_mem/*.c         \
             ../app_thread/app_thread_master/app_module_system/*.c          \
             ../app_thread/app_thread_mix_custom/*.c                        \
             ../app_thread/app_thread_mix_custom/app_module_alarm/*.c       \
             ../app_thread/app_thread_mix_custom/app_module_clock/*.c       \
             ../app_thread/app_thread_mix_custom/app_module_countdown/*.c   \
             ../app_thread/app_thread_mix_custom/app_module_dump/*.c        \
             ../app_thread/app_thread_mix_custom/app_module_stopwatch/*.c   \
             ../app_thread/app_thread_mix_custom/app_module_world_time/*.c  \
             ../app_thread/app_thread_mix_irq/*.c                           \

main: main.o
	gcc $(app_dbg) -o main ./*.o $(app_system)

main.o:
	gcc -c $(app_source) $(app_header) $(app_std)

clean:
	rm ./*.o ./main
