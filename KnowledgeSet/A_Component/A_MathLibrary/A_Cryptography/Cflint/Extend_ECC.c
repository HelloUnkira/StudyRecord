#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef EXTEND_USE_ECC
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define Ecc_NativeToBytes(Bytes, Natives, Length) \
        Cflint_Native4ToBytes((uint8_t *)(Bytes), (Natives), (Length))
#define Ecc_BytesToNative(Bytes, Natives, Length) \
        Cflint_Native4ToBytes((uint8_t *)(Bytes), (Natives), (Length))
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 椭圆曲线坐标: */
typedef struct EccPointBody {
    CFLINT_TYPE X[Ecc_Curve_Size1];
    CFLINT_TYPE Y[Ecc_Curve_Size1];
} EccPoint;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static const    EccPoint CURVE_G                  = Ecc_Curve_G;
static const CFLINT_TYPE CURVE_P[Ecc_Curve_Size1] = Ecc_Curve_P;
static const CFLINT_TYPE CURVE_B[Ecc_Curve_Size1] = Ecc_Curve_B;
static const CFLINT_TYPE CURVE_N[Ecc_Curve_Size2] = Ecc_Curve_N;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static    EccPoint *Curve_G = (   EccPoint *)(&CURVE_G);
static CFLINT_TYPE *Curve_P = (CFLINT_TYPE *)( CURVE_P);
static CFLINT_TYPE *Curve_B = (CFLINT_TYPE *)( CURVE_B);
static CFLINT_TYPE *Curve_N = (CFLINT_TYPE *)( CURVE_N);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static bool (*Ecc_RNG)(CFLINT_TYPE *Data, uint32_t Size) = NULL;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Ecc_RNG_Function(bool (*Rng)(CFLINT_TYPE *Data, uint32_t Size))
{
    Ecc_RNG = Rng;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static inline bool EccPoint_IsZero(EccPoint *Point)
{
    return Cflint_IsZero(Point->X, Ecc_Curve_Size1) &&
           Cflint_IsZero(Point->Y, Ecc_Curve_Size1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*仿射坐标:(X, Y) --- Jacobian加重射影坐标:(X, Y, Z)
 *转化机制:射影坐标(X, Y, Z) ===> 仿射坐标(X / (Z**2), Y / (Z**3))
 *椭圆曲线方程(仿射坐标):
 *(Y**2) = (X**3) + A(系数) * X + B(系数)
 *椭圆曲线方程(Jacobian加重射影坐标):
 *(Y**2) = (X**3) + A(系数) * X * (Z**4) + B(系数) * (Z**6)
 *!!!下面椭圆曲线计算是
 *!!!在Jacobian加重射影坐标下运算
 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* R = P + P: */
static void EccPoint_Double(CFLINT_TYPE *X, CFLINT_TYPE *Y, CFLINT_TYPE *Z)
{
    CFLINT_TYPE *CP = Curve_P;
    /* T1 = X, T2 = Y, T3 = Z */
    CFLINT_TYPE T4[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE T5[Ecc_Curve_Size1] = {0};
    /* 额外缓冲开销 */
    CFLINT_TYPE  Temp1[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE  Temp2[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE *TT[] = {Temp1, Temp2};
    /* 特殊检查:Z == 0 */
    if (Cflint_IsZero(Z, Ecc_Curve_Size1) == true)
        return;
    /* 下面是针对secp256k1有的Jacobian加重射影优化 */
#if (Ecc_SCEP == Ecc_secp256k1)
    /* T5 = (Y**2) */
    /* T4 = (Y**2 * X) = A */
    /* T1 = (X**2) */
    /* T5 = (Y**4) */
    /* T3 = (Y * Z) = Z3 */
    Cflint_ModuloSquare(T5,   CP, Y,      TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(T4, CP, X,  T5, TT, Ecc_Curve_Size1);
    Cflint_ModuloSquare(X,    CP, X,      TT, Ecc_Curve_Size1);
    Cflint_ModuloSquare(T5,   CP, T5,     TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Z,  CP, Y,  Z,  TT, Ecc_Curve_Size1);
    /* T2 = 2 * (X**2) */
    /* T2 = 3 * (X**2) */
    Cflint_ModuloAddition(Y, CP, X, X, TT, Ecc_Curve_Size1);
    Cflint_ModuloAddition(Y, CP, Y, X, TT, Ecc_Curve_Size1);
    /* T2 = 3 * (X**2) / 2 = B */
    CFLINT_TYPE Overflow = 0;
    if (Cflint_IsEven(Y, Ecc_Curve_Size1) == false)
        Overflow = Cflint_Addition(Y, Y, CP, Ecc_Curve_Size1);
    Cflint_ShiftRight2(Y, Ecc_Curve_Size1, 1);
    if (Overflow != 0)
        Y[Ecc_Curve_Size1 - 1] |= CFLINT_BYTEHIGHEST;
    /* T1 = (B**2) */
    /* T1 = (B**2) - A */
    /* T1 = (B**2) - A * 2 = X3 */
    Cflint_ModuloSquare(X,      CP, Y,     TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(X, CP, X, T4, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(X, CP, X, T4, TT, Ecc_Curve_Size1);
    /* T4 = A - X3 */
    /* T2 = B * (A - X3) */
    /* T2 = B * (A - X3) - (Y**4) = y3 */
    Cflint_ModuloSubtraction(T4, CP, T4, X,  TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Y,     CP, Y,  T4, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(Y,  CP, Y,  T5, TT, Ecc_Curve_Size1);
#else
    /* T4 = (Y**2) */
    /* T5 = (Y**2 * X) = A */
    /* T4 = (Y**4) = (T4**2) */
    /* T2 = (Y * Z) = Z3 */
    /* T3 = (Z**2) */
    Cflint_ModuloSquare(T4,   CP, Y,      TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(T5, CP, X,  T4, TT, Ecc_Curve_Size1);
    Cflint_ModuloSquare(T4,   CP, T4,     TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Y,  CP, Y,  Z,  TT, Ecc_Curve_Size1);
    Cflint_ModuloSquare(Z,    CP, Z,      TT, Ecc_Curve_Size1);
    /* T1 = X + Z**2 */
    /* T3 = 2 * Z**2 */
    /* T3 = X - Z**2 */
    /* T1 = X**2 - Z**4 = (X + Z**2)(X - Z**2) */
    Cflint_ModuloAddition(X,    CP, X, Z, TT, Ecc_Curve_Size1);
    Cflint_ModuloAddition(Z,    CP, Z, Z, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(Z, CP, X, Z, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(X,    CP, X, Z, TT, Ecc_Curve_Size1);
    /* T3 = 2 * (X**2 - Z**4) */
    /* T1 = 3 * (X**2 - Z**4) */
    Cflint_ModuloAddition(Z,    CP, X, X, TT, Ecc_Curve_Size1);
    Cflint_ModuloAddition(X,    CP, X, Z, TT, Ecc_Curve_Size1);
    /* T1 = 3 * (X**2 - Z**4) / 2 = B */
    CFLINT_TYPE Overflow = 0;
    if (Cflint_IsEven(X, Ecc_Curve_Size1) == false)
        Overflow = Cflint_Addition(X, X, CP, Ecc_Curve_Size1);
    Cflint_ShiftRight2(X, Ecc_Curve_Size1, 1);
    if (Overflow != 0)
        X[Ecc_Curve_Size1 - 1] |= CFLINT_BYTEHIGHEST;
    /* T3 = B**2 */
    /* T3 = B**2 - A */
    /* T3 = B**2 - A * 2 = X3 */
    /* T5 = A - X3 */
    /* T1 = B * (A - X3) */
    /* T4 = B * (A - X3) - Y**4 = Y3  */
    Cflint_ModuloSquare(Z,       CP, X,      TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(Z,  CP, Z,  T5, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(Z,  CP, Z,  T5, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(T5, CP, T5, Z,  TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(X,     CP, X,  T5, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(T4, CP, X,  T4, TT, Ecc_Curve_Size1);
    /* Z  == X3 */
    /* Y  == Z3 */
    /* T4 == Y3 */
    Cflint_Copy(X, Z,  Ecc_Curve_Size1);
    Cflint_Copy(Z, Y,  Ecc_Curve_Size1);
    Cflint_Copy(Y, T4, Ecc_Curve_Size1);
#endif
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* P(X, Y) ==> (X * Z**2, Y * Z**3) */
static void EccPoint_ApplyZ(CFLINT_TYPE *X, CFLINT_TYPE *Y, CFLINT_TYPE *Z)
{
    CFLINT_TYPE *CP = Curve_P;
    /*  */
    CFLINT_TYPE T1[Ecc_Curve_Size1] = {0};
    /* 额外缓冲开销 */
    CFLINT_TYPE  Temp1[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE  Temp2[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE *TT[] = {Temp1, Temp2};
    /* T1 = (Z**2) */
    /* X  = (Z**2) * X */
    /* T1 = (Z**3) */
    /* X  = (Z**3) * Y */
    Cflint_ModuloSquare(T1,   CP, Z,     TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(X,  CP, X, T1, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(T1, CP, Z, T1, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Y,  CP, Y, T1, TT, Ecc_Curve_Size1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* P(X1, Y1) => 2 * P, (X2, Y2) =>P' */
static void EccPoint_DoubleInit(CFLINT_TYPE *X1, CFLINT_TYPE *Y1,
                                CFLINT_TYPE *X2, CFLINT_TYPE *Y2,
                                CFLINT_TYPE *Zi)
{
    CFLINT_TYPE Z[Ecc_Curve_Size1] = {0};
    if (Zi != NULL)
        Cflint_Copy(Z, Zi, Ecc_Curve_Size1);
    if (Zi == NULL) {
        Cflint_SetValue(Z, Ecc_Curve_Size1, 0);
        Cflint_AdditionBit(Z, Ecc_Curve_Size1, 1);
    }
    /* (X2, Y2) = (X1, Y1) */
    Cflint_Copy(X2, X1, Ecc_Curve_Size1);
    Cflint_Copy(Y2, Y1, Ecc_Curve_Size1);
    /* (X1, Y1) ==>(X1, Y1, Z) */
    EccPoint_ApplyZ(X1, Y1, Z);
    /* (X1, Y1) ==> P * 2 */
    EccPoint_Double(X1, Y1, Z);
    /* (X2, Y2) ==> P' */
    EccPoint_ApplyZ(X2, Y2, Z);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Input:  P(X1, Y1, Z), Q(X2, Y2, Z) */
/* Output: P => (P')(X1', Y1', Z3), Q => (P + Q)(X3, Y3, Z3) */
static void EccPoint_Addition1(CFLINT_TYPE *X1, CFLINT_TYPE *Y1,
                               CFLINT_TYPE *X2, CFLINT_TYPE *Y2)
{
    CFLINT_TYPE *CP = Curve_P;
    /* T1 = X1, T2 = Y1, T3 = X2, T4 = Y2 */
    CFLINT_TYPE T5[Ecc_Curve_Size1] = {0};
    /* 额外缓冲开销 */
    CFLINT_TYPE  Temp1[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE  Temp2[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE *TT[] = {Temp1, Temp2};
    /* T5 = (X2 - X1) */
    /* T5 = (X2 - X1)**2 = A */
    /* T1 = (X1 * A) = B */
    /* T3 = (X2 * A) = C */
    /* T4 = (Y2 - Y1) */
    /* T5 = (Y2 - Y1)**2 = D */
    Cflint_ModuloSubtraction(T5, CP, X2, X1, TT, Ecc_Curve_Size1);
    Cflint_ModuloSquare(T5,      CP, T5,     TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(X1,    CP, X1, T5, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(X2,    CP, X2, T5, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(Y2, CP, Y2, Y1, TT, Ecc_Curve_Size1);
    Cflint_ModuloSquare(T5,      CP, Y2,     TT, Ecc_Curve_Size1);
    /* T5 = (D - B) */
    /* T5 = (D - B - C) = X3 */
    /* T3 = (C - B) */
    /* T2 = (C - B) * Y1 */
    /* T3 = (B - X3) */
    /* T4 = (B - X3) * (Y2 - Y1) */
    /* T4 = (B - X3) * (Y2 - Y1) - (C - B) * Y1 = Y3 */
    Cflint_ModuloSubtraction(T5, CP, T5, X1, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(T5, CP, T5, X2, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(X2, CP, X2, X1, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Y1,    CP, Y1, X2, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(X2, CP, X1, T5, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Y2,    CP, X2, Y2, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(Y2, CP, Y2, Y1, TT, Ecc_Curve_Size1);
    /* T3 = T5 */
    Cflint_Copy(X2, T5, Ecc_Curve_Size1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Input:  P(X1, Y1, Z), Q(X2, Y2, Z) */
/* Output: P => (P - Q)(X3', Y3', Z3), Q => (P + Q)(X3, Y3, Z3) */
static void EccPoint_Addition2(CFLINT_TYPE *X1, CFLINT_TYPE *Y1,
                               CFLINT_TYPE *X2, CFLINT_TYPE *Y2)
{
    CFLINT_TYPE *CP = Curve_P;
    /* T1 = X1, T2 = Y1, T3 = X2, T4 = Y2 */
    CFLINT_TYPE T5[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE T6[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE T7[Ecc_Curve_Size1] = {0};
    /* 额外缓冲开销 */
    CFLINT_TYPE  Temp1[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE  Temp2[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE *TT[] = {Temp1, Temp2};
    /* T5 = (X2 - X1) */
    /* T5 = (X2 - X1)**2 = A */
    /* T1 = (X1 * A) = B */
    /* T3 = (X2 * A) = C */
    /* T5 = (Y2 + Y1) */
    /* T4 = (Y2 - Y1) */
    Cflint_ModuloSubtraction(T5, CP, X2, X1, TT, Ecc_Curve_Size1);
    Cflint_ModuloSquare(T5,      CP, T5,     TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(X1,    CP, X1, T5, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(X2,    CP, X2, T5, TT, Ecc_Curve_Size1);
    Cflint_ModuloAddition(T5,    CP, Y2, Y1, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(Y2, CP, Y2, Y1, TT, Ecc_Curve_Size1);
    /* T6 = (C - B) */
    /* T2 = (C - B) * Y1 = E */
    /* T6 = (B + C) */
    /* T3 = (Y2 - Y1)**2 = D */
    /* T3 = (D - (B + C)) = X3 */
    Cflint_ModuloSubtraction(T6, CP, X2, X1, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Y1,    CP, Y1, T6, TT, Ecc_Curve_Size1);
    Cflint_ModuloAddition(T6,    CP, X1, X2, TT, Ecc_Curve_Size1);
    Cflint_ModuloSquare(X2,      CP, Y2,     TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(X2, CP, X2, T6, TT, Ecc_Curve_Size1);
    /* T7 = (B  - X3) */
    /* T4 = (Y2 - Y1)*(B - X3) */
    /* T4 = (Y2 - Y1)*(B - X3) - E = Y3 */
    Cflint_ModuloSubtraction(T7, CP, X1, X2, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Y2,    CP, Y2, T7, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Y2,    CP, Y2, Y1, TT, Ecc_Curve_Size1);
    /* T7 = (Y2 + Y1)**2 = F */
    /* T7 = (F - (B + C)) = X3' */
    /* T6 = (X3' - B) */
    /* T6 = (Y2 + Y1)*(X3' - B) */
    /* T2 = (Y2 + Y1)*(X3' - B) - E = Y3' */
    Cflint_ModuloSquare(T7,      CP, T5,     TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(T7, CP, T7, T6, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(T6, CP, T7, X1, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(T6,    CP, T6, T5, TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(Y1, CP, T6, Y1, TT, Ecc_Curve_Size1);
    /* T1 = T7 */
    Cflint_Copy(X1, T7, Ecc_Curve_Size1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Result = X**3 + A * X + B */
static void EccPoint_CurveX(CFLINT_TYPE *Result, CFLINT_TYPE *X)
{
    CFLINT_TYPE *R  = Result;
    CFLINT_TYPE *CP = Curve_P;
    CFLINT_TYPE *CB = Curve_B;
    /* 额外缓冲开销 */
    CFLINT_TYPE  Temp1[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE  Temp2[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE *TT[] = {Temp1, Temp2};
#if (Ecc_CURVE == Ecc_secp256k1)
    /* R = (X**2) */
    /* R = (X**3) */
    /* R = (X**3 + B) */
    Cflint_ModuloSquare(R,   CP, X,     TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(R, CP, X,  R, TT, Ecc_Curve_Size1);
    Cflint_ModuloAddition(R, CP, CB, R, TT, Ecc_Curve_Size1);
#else
    /* -A = 3 */
    CFLINT_TYPE N3[Ecc_Curve_Size1] = {3};
    /* R = (X**2) */
    /* R = (X**2) - 3 */
    /* R = (X**3) - 3 * X */
    /* R = (X**3) - 3 * X + B */
    Cflint_ModuloSquare(R,      CP, X,     TT, Ecc_Curve_Size1);
    Cflint_ModuloSubtraction(R, CP, R, N3, TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(R,    CP, R, X,  TT, Ecc_Curve_Size1);
    Cflint_ModuloAddition(R,    CP, R, CB, TT, Ecc_Curve_Size1);
#endif
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static void EccPoint_Multiply(EccPoint *Result, CFLINT_TYPE *Scalar,
                              EccPoint *Point,  CFLINT_TYPE *InitZ,
                               int64_t  Bits2)
{
    CFLINT_TYPE *CP = Curve_P;
    /* R0, R1 */
    CFLINT_TYPE RX[2][Ecc_Curve_Size1] = {0};
    CFLINT_TYPE RY[2][Ecc_Curve_Size1] = {0};
    CFLINT_TYPE Z[Ecc_Curve_Size1] = {0};
    bool Bit2 = false;
    int64_t Index = 0;
    /* 额外缓冲开销 */
    CFLINT_TYPE  Temp1[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE  Temp2[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE  Temp3[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE  Temp4[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE *TT[] = {Temp1, Temp2, Temp3, Temp4};
    /* (Rx[1], Ry[1]) <=== (Point->X, Point->Y) */
    Cflint_Copy(RX[1], Point->X, Ecc_Curve_Size1);
    Cflint_Copy(RY[1], Point->Y, Ecc_Curve_Size1);
    /* 初始化倍乘 */
    EccPoint_DoubleInit(RX[1], RY[1], RX[0], RY[0], InitZ);
    /* 逆序叠加 */
    for (Index = Bits2 - 2; Index > 0; Index--) {
        Bit2 = !Cflint_CheckBit2(Scalar, Ecc_Curve_Size1, Index);
        EccPoint_Addition2(RX[1 - Bit2], RY[1 - Bit2], RX[Bit2], RY[Bit2]);
        EccPoint_Addition1(RX[Bit2], RY[Bit2], RX[1 - Bit2], RY[1 - Bit2]);
    }
    {
        Bit2 = !Cflint_CheckBit2(Scalar, Ecc_Curve_Size1, Index);
        EccPoint_Addition2(RX[1 - Bit2], RY[1 - Bit2], RX[Bit2], RY[Bit2]);
    }
    /* 找到最后(1 / Z)值 */
    /* (X1 - X0) */
    /* (X1 - X0) * Yb */
    /* (X1 - X0) * Yb * PX */
    /* 1 / ((X1 - X0) * Yb * PX) */
    /* 1 / ((X1 - X0) * Yb * PX) * PY */
    /* 1 / ((X1 - X0) * Yb * PX) * PY * Xb */
    Cflint_ModuloSubtraction(Z, CP, RX[1], RX[0],        TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Z,    CP, Z,     RY[1 - Bit2], TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Z,    CP, Z,     Point->X,     TT, Ecc_Curve_Size1);
    Cflint_ModuloInverse(Z,  Z, CP,                      TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Z,    CP, Z,     Point->Y,     TT, Ecc_Curve_Size1);
    Cflint_ModuloMultiply(Z,    CP, Z,     RX[1 - Bit2], TT, Ecc_Curve_Size1);
    /* 结束(1 / Z)计算 */
    EccPoint_Addition1(RX[Bit2], RY[Bit2], RX[1 - Bit2], RY[1 - Bit2]);
    EccPoint_ApplyZ(RX[0], RY[0], Z);
    /* (Result->X, Result->Y) <=== (Rx[0], Ry[0]) */
    Cflint_Copy(Result->X, RX[0], Ecc_Curve_Size1);
    Cflint_Copy(Result->Y, RY[0], Ecc_Curve_Size1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define ECC_MAX_TRIES   64  /* 加密流程尝试最大迭代次数限制 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static bool Ecc_ComputeKey(EccPoint *Public, CFLINT_TYPE *Private)
{
    if (Cflint_IsZero(Private, Ecc_Curve_Size1) == true)
        return false;
#if (ECC_SCEP == ECC_secp160r1)
    int64_t Bits2 = Cflint_Numbers2(Private, Ecc_Curve_Size1) + 1;
    EccPoint_Multiply(Public, Curve_G, Private, NULL, Bits2);
    return (EccPoint_IsZero(Public) == true) ? false : true;
#else
    CFLINT_TYPE  Temp1[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE  Temp2[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE *TT[] = {Temp1, Temp2};
    CFLINT_TYPE  Overflow = 0;
    uint32_t     Length = CFLINT_BITS * Ecc_Curve_Size1 + 1;
    
    if (Cflint_Compare(Curve_N, Private, Ecc_Curve_Size1) != 1)
        return false;
    
    Overflow = 
    Cflint_Addition(Temp1, Private, Curve_N, Ecc_Curve_Size1);
    Cflint_Addition(Temp2, Temp1,   Curve_N, Ecc_Curve_Size1);
    Overflow = (Overflow == 0) ? 1 : 0;
    EccPoint_Multiply(Public, TT[Overflow], Curve_G, NULL, Length);
    return (EccPoint_IsZero(Public) == true) ? false : true;
#endif
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Ecc_MakeKey(CFLINT_TYPE PublicKey[Ecc_Curve_Size1 * 2],
                 CFLINT_TYPE PrivateKey[Ecc_Curve_Size1])
{
    EccPoint    Public = {0};
    CFLINT_TYPE Private[Ecc_Curve_Size1] = {0};
    
    for (uint32_t tries = 0; tries < ECC_MAX_TRIES; tries++) {
        /* 生成随机私钥 */
        if (Ecc_RNG(Private, Ecc_Curve_Size1) == false)
            continue;
        /* 尝试运算点乘,生成公钥 */
        if (Ecc_ComputeKey(&Public, Private) == false)
            continue;
        Ecc_NativeToBytes(PrivateKey, Private, Ecc_Curve_Size1);
        Ecc_NativeToBytes(PublicKey + Ecc_Curve_Size1 * 0, Public.X, Ecc_Curve_Size1);
        Ecc_NativeToBytes(PublicKey + Ecc_Curve_Size1 * 1, Public.Y, Ecc_Curve_Size1);
        return true;
    }
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Ecc_ShareSecret(CFLINT_TYPE PublicKey[Ecc_Curve_Size1 * 2],
                     CFLINT_TYPE PrivateKey[Ecc_Curve_Size1],
                     CFLINT_TYPE SecretKey[Ecc_Curve_Size1])
{
    EccPoint     Product;
    EccPoint     Public = {0};
    CFLINT_TYPE  Private[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE  Random[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE *InitZ = NULL;
    /* 生成随机Z */
    for (uint32_t tries = 0; tries < ECC_MAX_TRIES; tries++) {
        if (Ecc_RNG(Random, Ecc_Curve_Size1) == false)
            continue;
        if (Cflint_IsZero(Random, Ecc_Curve_Size1) == true)
            continue;
        InitZ = Random;
        break;
    }
    Ecc_BytesToNative(PrivateKey, Private, Ecc_Curve_Size1);
    Ecc_BytesToNative(PublicKey + Ecc_Curve_Size1 * 0, Public.X, Ecc_Curve_Size1);
    Ecc_BytesToNative(PublicKey + Ecc_Curve_Size1 * 1, Public.Y, Ecc_Curve_Size1);
#if (ECC_SCEP == ECC_secp160r1)
    int64_t Bits2 = Cflint_Numbers2(Private, Ecc_Curve_Size1) + 1;
    EccPoint_Multiply(&Product, Private, &Public, InitZ, Bits2);
#else
    CFLINT_TYPE  Temp[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE *TT[] = {Private, Temp};
    CFLINT_TYPE  Overflow = 0;
    uint32_t     Length = CFLINT_BITS * Ecc_Curve_Size1 + 1;
    
    Overflow = 
    Cflint_Addition(Private, Private, Curve_N, Ecc_Curve_Size1);
    Cflint_Addition(Temp,    Private, Curve_N, Ecc_Curve_Size1);
    Overflow = (Overflow == 0) ? 1 : 0;
    EccPoint_Multiply(&Public, TT[Overflow], Curve_G, InitZ, Length);
#endif
    Ecc_BytesToNative(SecretKey, Product.X, Ecc_Curve_Size1);
    return (EccPoint_IsZero(&Product) == true) ? false : true;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool Ecc_ValidKey(CFLINT_TYPE PublicKey[Ecc_Curve_Size1 * 2])
{
    EccPoint     Public = {0};
    CFLINT_TYPE  Temp_1[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE  Temp_2[Ecc_Curve_Size1] = {0};
    CFLINT_TYPE  Temp1[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE  Temp2[Ecc_Curve_Size1 * 2] = {0};
    CFLINT_TYPE *TT[] = {Temp1, Temp2};
    
    Ecc_BytesToNative(PublicKey + Ecc_Curve_Size1 * 0, Public.X, Ecc_Curve_Size1);
    Ecc_BytesToNative(PublicKey + Ecc_Curve_Size1 * 1, Public.Y, Ecc_Curve_Size1);
    
    if (EccPoint_IsZero(&Public) == true)
        return false;
    if (Cflint_Compare(Curve_P, Public.X, Ecc_Curve_Size1) != 1)
        return false;
    if (Cflint_Compare(Curve_P, Public.Y, Ecc_Curve_Size1) != 1)
        return false;
    
    Cflint_ModuloSquare(Temp_1, Curve_P, Public.Y, TT, Ecc_Curve_Size1);
    EccPoint_CurveX(Temp_2, Temp_1);
    
    return Cflint_Equal(Temp_1, Temp2, Ecc_Curve_Size1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

