
/* ����Ψһ������ */
#include "SGuiPort.h"

#if SGUI_USE_LINUX
// note:
// struct itimerval 
// {
//      struct timeval it_interval; //��ʱ���
//      struct timeval it_value;    //��ʱʱ��(�ź���ʱ����)
// };
// struct timeval 
// {
//      time_t      tv_sec;         //��
//      suseconds_t tv_usec;        //΢��, 1/1'000'000��
// };
// ����ԭ�ͣ�
// int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
// which:
//       ITIMER_REAL(���ͳ�SIGALRM�ź�):         ��ϵͳ��ʵ��ʱ��������
//       ITIMER_VIRTUAL(���ͳ�SIGVTALRM�ź�):    �Ըý������û�̬�»��ѵ�ʱ��������
//       ITIMER_PROF(���ͳ�SIGPROF�ź�):         �Ըý������û�̬�º��ں�̬�����ѵ�ʱ��������
// old_value��ͨ���ò���,����ΪNULL,�洢��һ�δ�����¼
// new_value: it_intervalΪ��ʱ���,it_valueΪ��ʱʱ��
// it_valueΪ0�ǲ��ᴥ���źŵ�,����Ҫ�ܴ����ź�,it_value�ô���0
// it_intervalΪ��,ֻ����ʱ,���ᶨʱ

#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

//ȫ�ֻ�Ψһ1ms��ʱ��ʵ��
static SGuiTimerMSHandler ms_handler = NULL;

void SGuiTimerCfg(SGuiTimerMSHandler handler)
{
    //���ûص�
    ms_handler = handler;
}

static void SGuiTimerSignalHandler(int signal)
{
    switch (signal) {
    //��ϵͳ��ʵ��ʱ��������
    case SIGALRM:
        if (ms_handler != NULL)
            ms_handler();
        break;
    //�Ըý������û�̬�»��ѵ�ʱ��������
    case SIGVTALRM:
        break;
    //�Ըý������û�̬�º��ں�̬�����ѵ�ʱ��������
    case SIGPROF:
        break;
    }
}

void SGuiTimerInit(void)
{
    struct itimerval old_value = {0};
    struct itimerval new_value = {0};
    //���źŹ���ָ���Ļص�
    signal(SIGALRM, SGuiTimerSignalHandler);
    //���ü������ʱ��
    new_value.it_interval.tv_sec = 0;
    new_value.it_interval.tv_usec = 1000;//1ms
    new_value.it_value.tv_sec = 0;
    new_value.it_value.tv_usec = 1;//����Ϊ0
    //���ö�ʱ��
    setitimer(ITIMER_REAL, &new_value, &old_value);
}

/* �����������ƽ̨
#elif SGUI_USE_UNKNOWN
*/
#else

#endif
