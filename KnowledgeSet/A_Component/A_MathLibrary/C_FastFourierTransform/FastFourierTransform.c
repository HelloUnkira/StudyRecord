//C std lib
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
const double PIE = 3.14159265358;  //圆周率PIE(acos(-1))
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
typedef struct FastFourierTransformDataBody {
    int32_t  *ListRev;  //递归的最后一层下标表(辅助数组)
    double   *ListReal; //要处理的复数集实数部
    double   *ListImag; //要处理的复数集虚数部
    uint32_t  Length;   //复数集的长度
    int8_t    Flag;
} FastFourierTransformData;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static void FastFourierTransformRun(FastFourierTransformData *Data)
{
    uint32_t *RevList  = Data->ListRev;
    double   *RealList = Data->ListReal;
    double   *ImagList = Data->ListImag;
    
    for (uint32_t I = 0; I < Data->Length; RevList[I++] = 0);
    //1.回溯前的准备,先进入到递归的最后一层,也即回溯的第一层
    {
        uint32_t Offset = -1;
        while ((1 << (++Offset)) < Data->Length);
        for (uint32_t I = 0; I < Data->Length; I++)
            RevList[I]=(RevList[I >> 1] >> 1) | ((I & 1) << (Offset - 1));
    }
    //2.接下来交换元素到递归的最后一层(为回溯做好准备)
    for (uint32_t I = 0; I < Data->Length; I++) {
        if (I >= RevList[I])
            continue;
        uint32_t J  = RevList[I];
        double   T1 = RealList[I];
        double   T2 = ImagList[I];
        RealList[I] = RealList[J];
        ImagList[I] = ImagList[J];
        RealList[J] = T1;
        ImagList[J] = T2;
    }
    //3.从递归的最后一层开始进行回溯(h即表示当前层级单元元素量)
    //对应于分治树的最后一层,此时该层每一个单元只有一个元素,递归的底部
    for (uint32_t H = 1; H < Data->Length; H *= 2) {
        //计算单位根,注意:E**(i * X) = cos(X) + i * sin(X)
        double Real_W1 = cos(Data->Flag * PIE / H);
        double Imag_W1 = sin(Data->Flag * PIE / H);
        //计算每一个子单元,注意这里J的跨度是2*H,跳过了后面的H个单元
        for (uint32_t J = 0; J < Data->Length; J += 2 * H) {
            double Real_Wn = 1.0;//起始运动节点在Wn(W0)时
            double Imag_Wn = 0.0;//每次进动后都偏移一个单位根
            //这里的k跨度是h,所以x为左半部分元素,y为右半部分元素
            for (uint32_t K = J; K < J + H; K++) {
                double Real_X = RealList[K];       //左半部分元素
                double Imag_X = ImagList[K];
                double Real_Y = RealList[K + H];   //右半部分元素
                double Imag_Y = ImagList[K + H];
                double T1 = 0.0;
                double T2 = 0.0;
                //右半部分比左半部分多出w的偏移,为右半部分旋转wn(计算y*=wn)
                //复数*运算:z1*z2 = (x1x2 - y1y2) + I(x1y2 + x2y1)
                T1 = Real_Wn * Real_Y - Imag_Wn * Imag_Y;
                T2 = Real_Wn * Imag_Y + Imag_Wn * Real_Y;
                Real_Y = T1;
                Imag_Y = T2;
                //复数±运算:z1±z2 = (x1 ± x2) + I(y1 ± y2)
                RealList[K]     = Real_X + Real_Y;
                ImagList[K]     = Imag_X + Imag_Y;
                RealList[K + H] = Real_X - Real_Y;
                ImagList[K + H] = Imag_X - Imag_Y;
                //计算下一个单位根(几何意义是单位根旋转,运算是wn*=w1)
                T1 = Real_Wn * Real_W1 - Imag_Wn * Imag_W1;
                T2 = Real_Wn * Imag_W1 + Imag_Wn * Real_W1;
                Real_Wn = T1;
                Imag_Wn = T2;
            }
        }
    }
    //4.判断是否是DFT还是IDFT
    for(uint32_t I = 0; Data->Flag == -1 && I < Data->Length; I++) {
        //复数/运算:z1/z2 = ((x1x2 + y1y2) / (x2**2 + y2**2)) + I
        //                  ((x2y1 - x1y2) / (x2**2 + y2**2))
        //当y2 == 0时,退化为:z1/x2 = x1 / x2 + I (y1 / x2)
        RealList[I] /= Data->Length;
        ImagList[I] /= Data->Length;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void FFT(double *Real, double *Imag, uint32_t Len, uint32_t *Rev)
{
    if (Real == NULL || Imag == NULL || Rev == NULL || Len == 0)
        return;
    FastFourierTransformData Data = {
        .ListRev  = Rev,
        .ListReal = Real,
        .ListImag = Imag,
        .Length   = Len,
        .Flag     = +1,
    };
    FastFourierTransformRun(&Data);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void IFFT(double *Real, double *Imag, uint32_t Len, uint32_t *Rev)
{
    if (Real == NULL || Imag == NULL || Rev == NULL || Len == 0)
        return;
    FastFourierTransformData Data = {
        .ListRev  = Rev,
        .ListReal = Real,
        .ListImag = Imag,
        .Length   = Len,
        .Flag     = -1,
    };
    FastFourierTransformRun(&Data);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
