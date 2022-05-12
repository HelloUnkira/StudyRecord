//C std lib
#include <stdint.h>
#include <stdbool.h>
/*****************************************************************************/
//问题:C不支持浮点数移位,如果使用整形
//当i达到某一个值时,2^-i是一个非常小的值以至于
//x_current和y_current左移时都为0,此时迭代陷入到困境中
//而且实际精度远不受precision影响而是初始化时其扩大的值影响
//但即使是这样子精度也很低,只能保证其前几位够用(三位后便开始漂移)
//Cordix算法(第一象限):
//x1 =  xcos(t) + ysin(t);
//y1 = -xsin(t) + ycos(t);
//矩阵表达式:                 简化为:
//[x1][ cos(t) sin(t)][X] ==> [x1]cos(t)[   1    tan(t)][X]
//[y1][-sin(t) cos(t)][Y] ==> [y1]cos(t)[-tan(t)   1   ][Y]
/*****************************************************************************/
const double PIE = 3.14159265358;  //圆周率PIE(acos(-1))
/*****************************************************************************/
//弧度值表(计算机出现前,关于三角函数都是查表计算(倍角公式)):
#define ARCTAN_TABLE_MAX 32
/*****************************************************************************/
static double ArctanTable[ARCTAN_TABLE_MAX] = {
    0.785398163397448280000000000000, //Arctan(1/(2^0))
    0.463647609000806090000000000000, //Arctan(1/(2^1))
    0.244978663126864140000000000000, //Arctan(1/(2^2))
    0.124354994546761440000000000000, //Arctan(1/(2^3))
    0.062418809995957350000000000000, //Arctan(1/(2^4))
    0.031239833430268277000000000000, //Arctan(1/(2^5))
    0.015623728620476831000000000000, //Arctan(1/(2^6))
    0.007812341060101111100000000000, //Arctan(1/(2^7))
    0.003906230131966971800000000000, //Arctan(1/(2^8))
    0.001953122516478818800000000000, //Arctan(1/(2^9))
    0.000976562189559319460000000000, //Arctan(1/(2^10))
    0.000488281211194898290000000000, //Arctan(1/(2^11))
    0.000244140620149361770000000000, //Arctan(1/(2^12))
    0.000122070311893670210000000000, //Arctan(1/(2^13))
    0.000061035156174208773000000000, //Arctan(1/(2^14))
    0.000030517578115526096000000000, //Arctan(1/(2^15))
    0.000015258789061315762000000000, //Arctan(1/(2^16))
    0.000007629394531101970000000000, //Arctan(1/(2^17))
    0.000003814697265606496100000000, //Arctan(1/(2^18))
    0.000001907348632810187000000000, //Arctan(1/(2^19))
    0.000000953674316405960840000000, //Arctan(1/(2^20))
    0.000000476837158203088840000000, //Arctan(1/(2^21))
    0.000000238418579101557970000000, //Arctan(1/(2^22))
    0.000000119209289550780680000000, //Arctan(1/(2^23))
    0.000000059604644775390552000000, //Arctan(1/(2^24))
    0.000000029802322387695303000000, //Arctan(1/(2^25))
    0.000000014901161193847655000000, //Arctan(1/(2^26))
    0.000000007450580596923828100000, //Arctan(1/(2^27))
    0.000000003725290298461914100000, //Arctan(1/(2^28))
    0.000000001862645149230957000000, //Arctan(1/(2^29))
    0.000000000931322574615478520000, //Arctan(1/(2^30))
    0.000000000465661287307739260000, //Arctan(1/(2^31))
};
/*****************************************************************************/
static inline void CordixAngleSimplify(double *Angle, double *Flag)
{
    double Period1 = 1.0 * PIE;
    double Period2 = 2.0 * PIE;
    /* 利用:sin(Angle) = sin(Angle +- 2PAI)化简 */
    /* 利用:cos(Angle) = cos(Angle +- 2PAI)化简 */
    while (*Angle - Period2 >= 0.0 && (*Angle -= Period2));
    while (*Angle + Period2 <= 0.0 && (*Angle += Period2));
    if (*Angle > 0.0 && *Angle - Period2 > -*Angle)
        *Angle -= Period2;
    if (*Angle < 0.0 && *Angle + Period2 < -*Angle)
        *Angle += Period2;
    /* 利用:-sin(Angle) = sin(Angle +- PAI)化简 */
    /* 利用:-cos(Angle) = cos(Angle +- PAI)化简 */
    if (*Angle > 0.0 && *Angle - Period1 > -*Angle) {
        *Angle -= Period1;
        *Flag = -*Flag;
    }
    if (*Angle < 0.0 && *Angle + Period1 < -*Angle) {
        *Angle += Period1;
        *Flag = -*Flag;
    }
}
/*****************************************************************************/
static void CordixAngleRun(double Angle, double Result[2])
{
    /* 简化Angle==>[-PAI/2,PAI/2] */
    double Flag = 1.0;
    CordixAngleSimplify(&Angle, &Flag);
    /* 计算补偿因子:SUM(I=(0->N-1)) (1 + 2**(-2I))**(-1/2) */
    /* 初始化状态量 */
    double NextX = 0, CurrentX = 0.60725935;
    double NextY = 0, CurrentY = 0.0;
    /* 开始逆时针旋转 */
    for (uint32_t I = 0; I < ARCTAN_TABLE_MAX; I++) {
        //逆时针伪旋转公式:
        if (Angle >  0.0) {
            NextX = CurrentX - CurrentY / (double)(1 << I);
            NextY = CurrentY + CurrentX / (double)(1 << I);
            Angle -= ArctanTable[I];
        }
        if (Angle <= 0.0) {
            NextX = CurrentX + CurrentY / (double)(1 << I);
            NextY = CurrentY - CurrentX / (double)(1 << I);
            Angle += ArctanTable[I];
        }
        
        CurrentX = NextX;
        CurrentY = NextY;
    }
    Result[0] = Flag * CurrentY;
    Result[1] = Flag * CurrentX;
}
/*****************************************************************************/
double CordixSin(double Angle)
{
    double Result[2] = {0};
    CordixAngleRun(Angle, Result);
    return Result[0];
}
/*****************************************************************************/
double CordixCos(double Angle)
{    
    double Result[2] = {0};
    CordixAngleRun(Angle, Result);
    return Result[1];
}
/*****************************************************************************/
double CordixTan(double Angle)
{
    double Result[2] = {0};
    CordixAngleRun(Angle, Result);
    return Result[0] / Result[1];
}
/*****************************************************************************/
double CordixArctan(double Y, double X)
{
    double Result = 0.0, Flag = 0.0;
    //坐标归化为第一四象限的值:
    (X < 0 && Y > 0 && (X = -X, Y = -Y, Flag = 1.0));
    (X < 0 && Y < 0 && (X = -X, Y = -Y, Flag = 1.0));
    //比例放大
    int64_t NextX = 0, CurrentX = X * 1024;
    int64_t NextY = 0, CurrentY = Y * 1024;
    //开始旋转
    for (uint32_t I = 0; I < ARCTAN_TABLE_MAX; I++) {
        //伪旋转公式:
        if (CurrentY >  0) {
            NextX = CurrentX + CurrentY >> I;
            NextY = CurrentY - CurrentX >> I;
            Result += ArctanTable[I];
        }
        if (CurrentY <= 0) {
            NextX = CurrentX - CurrentY >> I;
            NextY = CurrentY + CurrentX >> I;
            Result -= ArctanTable[I];
        }

        CurrentX = NextX;
        CurrentY = NextY;
    }
    return Result + Flag * PIE;
}
/*****************************************************************************/
