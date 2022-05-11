///////////////////////////////////////////////////////////////////////////////
//初等函数扩展:////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define MATH_LIBRARY_SIMPLE_MACRO
#ifdef  MATH_LIBRARY_SIMPLE_MACRO

#define NULL 0

#define RETURN_VALUE(condition, value) \
        if (condition)                 \
            return (value);

#define SIMPLE_IF(condition, work) \
        if (condition)             \
            work;

#define ABSOLUTE_VALUE(value)       \
        (value > 0 ? value : -value)

#endif

#define MATH_LIBRARY_BASIC
#ifdef  MATH_LIBRARY_BASIC

///////////////////////////////////////////////////////////////////////////////
//计算阶乘/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int calculate_factorial(unsigned int x)
{   //0! == 1 && 1! == 1
    RETURN_VALUE(x < 2, 1);
    
    unsigned int i = 0, result = 1;
    for (i = 1; i <= x; result *= i++);
    return result;
}

///////////////////////////////////////////////////////////////////////////////
//计算2阶乘////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int calculate_double_factorial(unsigned int x)
{
    RETURN_VALUE(x < 2, 1);
    
    unsigned int i = (x % 2 != 0) ? 1 : ((x % 2 == 0) ? 2 : 0), result = 1;
    for (i = i; i <= x; result *= i, i += 2);
    return result;
}

///////////////////////////////////////////////////////////////////////////////
//计算组合C(k,n)(k为上标,n为下标)//////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int calculate_combination(unsigned int k, unsigned int n)
{
    SIMPLE_IF(k > n, return 0);
    
    return calculate_factorial(n) / 
          (calculate_factorial(k) * 
           calculate_factorial(n - k));
}

///////////////////////////////////////////////////////////////////////////////
//计算次方/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double calculate_x_n(double x, unsigned int n)
{   //x**0 == 1
    SIMPLE_IF(x == 0.0, return 0.0);
    SIMPLE_IF(n == 0,   return 1.0);
    
    unsigned int i = 0;
    double result = 1;
    for (i = 0; i < n; result *= x, i++);
    return result;
}

#endif

///////////////////////////////////////////////////////////////////////////////
//数学扩展:DEFINITE INTEGRATION////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define MATH_LIBRARY_EXTENSION_DEFINITE_INTEGRATION
#ifdef  MATH_LIBRARY_EXTENSION_DEFINITE_INTEGRATION

///////////////////////////////////////////////////////////////////////////////
//计算定积分///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double calculate_definite_integration(double (*f_x)(double x),
                                      double left,
                                      double right,
                                      unsigned int precision,
                                      unsigned int flag)
{
    if (f_x == NULL || left > right || precision == 0)
        return 0.0;
    
    unsigned int i = 0;
    double current = 0.0, result  = 0.0, result1 = 0.0, result2 = 0.0;
    double delta1  = (right - left) / (double)precision;
    double delta2  = (right - left) / (double)precision * 2.0;

    //梯形法公式:
    //(b-a)/n(y(1)+...+y(n-1)+y(0)/2+y(n)/2)
    //此时:--> precision = n
    if (flag == 1) {
        
        current = left;
        for (i = 1; i < precision; i++) {
            current += delta1;
            result  += f_x(current);
        }
        
        result += (f_x(left) + f_x(right)) / 2.0;
        result *= delta1;
    }
    
    //抛物线法公式(辛普森公式):
    //(b-a)/6n(y(0)+y(2n)+4(y1+...+y(2n-1))+2(y2+...+y(2n-2)))
    //此时:--> precision = 2n
    if (flag == 2) {
        if (precision % 2 != 0)
            precision += 1;

        //求奇数部分
        current = left + delta1;
        for (i = 1; i < precision; i += 2) {
            result1 += f_x(current);
            current += delta2;
        }
        //求偶数部分
        current = left + delta2;
        for (i = 2; i < precision; i += 2) {
            result2 += f_x(current);
            current += delta2;
        }
        
        result += (f_x(left) + f_x(right));
        result += result1 * 4.0 + result2 * 2.0;
        result *= delta1 / 3.0;
    }
    
    return result;
}

#endif

///////////////////////////////////////////////////////////////////////////////
//数学扩展:NEWTON TANGENT//////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define MATH_LIBRARY_EXTENSION_NEWTON_TANGENT
#ifdef  MATH_LIBRARY_EXTENSION_NEWTON_TANGENT

///////////////////////////////////////////////////////////////////////////////
//计算多项式SUM(k=0->n) ak * x**k (参数为a(0->n))//////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double calculate_multinomial(unsigned int exponent,
                             unsigned int level,
                             double *parameter,
                             double  x)
{
    SIMPLE_IF(level > exponent || parameter == NULL, return 0.0);

    unsigned int i = 0, j = 1;
    double result = 0.0, temp = 0;
    //以steps!为起点,每次迭代只需额外计算俩次,适于高阶导数节约时间
    unsigned int base_factorial = calculate_factorial(level);
    
    for (i = level; i <= exponent; i++, result += temp) {
        //1.小于steps次项,求steps次导后该项不存在
        //2.大于或等于该阶次,迭代求取
        temp  = parameter[i];
        temp *= calculate_x_n(x, i - level);
        temp *= level != 0 ? base_factorial : 1;
        if (level != 0) {
            base_factorial /= j++;
            base_factorial *= i + 1;
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
//牛顿切线法求取方程近似解/////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double calculate_newton_tangent(double (*f_0_x)(double x0),
                                double (*f_1_x)(double x1),
                                double start,
                                double precision)
{
    SIMPLE_IF(f_0_x == NULL || f_1_x == NULL || precision == 0.0, return 0);

    double x_next = 0, x = start, temp = 0;
    double x_zero_level = 0, x_one_level = 0;

    precision = ABSOLUTE_VALUE(precision);
    while (1) {
        //计算x0多项式的值与一阶导数的值
        x_zero_level = f_0_x(x);
        x_one_level  = f_1_x(x);
        //
        if (x_one_level == 0.0)
            break;
        //计算切点值
        temp = x_zero_level / x_one_level;
        //
        if (ABSOLUTE_VALUE(temp) <= precision)
            break;
        //迭代
        x_next = x - temp;
        x      = x_next;
    }
    return x;
}

#endif

///////////////////////////////////////////////////////////////////////////////
//数学扩展:TAYLOR FORMUL///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define MATH_LIBRARY_EXTENSION_TAYLOR_FORMULA
#ifdef  MATH_LIBRARY_EXTENSION_TAYLOR_FORMULA
//临时使用该方式替代PAI
const double math_pai = 3.141592653589793;//圆周率PIE(acos(-1))

///////////////////////////////////////////////////////////////////////////////
//计算e**x/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double calculate_e_x(double x, unsigned int precision)
{   //SUM(k=(0->n)) (x**k / k!)
    SIMPLE_IF(x == 0.0, return 1.0);

    unsigned int i = 0, factorial = 1;
    double power = 1.0, result = 0.0;
    for (i = 1; i <= precision; i++) {
        result += power / (double)factorial;
        power  *= x;
        factorial *= i;
    }
    return result;
}

///////////////////////////////////////////////////////////////////////////////
//计算ln(x)////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static double calculate_special_ln(double x, double precision)
{
    SIMPLE_IF(precision == 0.0, return 0.0);
    //用于优化原有泰勒公式不能很好收敛的情况
    //一个特殊公式的计算:
    //ln(x) = ln((1 + t)/(1 - t)) = ln(1 + t) - ln(1 - t)
    //      = 2 * t SUM(k=(0->n)) (t * t)**k / (2 * k + 1)
    // x = (1 + t)/(1 - t)  ==> t = (x - 1)/(x + 1)
    double t = (x - 1) / (x + 1);
    double t_square = t * t;
    double power = 1.0, temp = 1.0, result = 0.0;
    unsigned int i = 0;
    
    for (i = 0; temp >= precision; i++) {
        temp = power / (double)(2 * i + 1);
        power *= t_square;
        result += temp;
    }
    result *= 2 * t;
    return result;
}

double calculate_ln(double x, double precision)
{
    SIMPLE_IF(x <= 0.0 || x == 1.0, return 0.0);
    //优化1:ln(x) = ln(t*10**index) = ln(t) + index*ln10
    double index = 0.0, t = x, ln_t = 0.0, ln_10 = 0.0;
    for (t = x; t > 10.0; t /= 10.0, index += 1.0);
    //优化2:ln(t) = ln((1 + a) / (1 - a)) = ln(1 + a) - ln(1 - a)
    // ==> a = (t - 1) / (t + 1)
    ln_t  = calculate_special_ln(t, precision);
    if (index == 0.0)
        return ln_t;
    ln_10 = calculate_special_ln(10.0, precision);
    return ln_t + (double)index * ln_10;
}

///////////////////////////////////////////////////////////////////////////////
//计算sin(x)///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double calculate_sin(double x, double precision)
{
    SIMPLE_IF(precision == 0.0, return 0.0);
    //利用:sin(x) = sin(x +- 2PAI)对其简要化简 --- x=[-PAI,PAI]
    double flag = 1.0;
    double period1 = 1.0 * math_pai;
    double period2 = 2.0 * math_pai;
    while (x - period2 >= 0 && (x -= period2));
    while (x + period2 <= 0 && (x += period2));
    (x > 0 && x - period2 > -x && (x -= period2));
    (x < 0 && x + period2 < -x && (x += period2));
    //利用:-sin(x) = sin(x +- PAI)进一步化简 --- x=[-PAI/2,PAI/2]
    (x > 0 && x - period1 > -x && (x -= period1, flag = -flag));
    (x < 0 && x + period1 < -x && (x += period1, flag = -flag));
    
    //SUM(k=(0->n)) ((-1)**k x**(2*k+1) / (2*k+1)!)
    unsigned int i = 0, factorial = 1;
    double power = x, temp = x, result = 0.0;
    
    precision = ABSOLUTE_VALUE(precision);
    for (i = 0; ABSOLUTE_VALUE(temp) >= precision; i++) {
        if (i % 2 == 0)
            result += temp;
        if (i % 2 != 0)
            result -= temp;
        power *= x * x;
        factorial *= (2 * i + 2) * (2 * i + 3);
        temp = power / (double)factorial;
    }
    return flag * result;
}

///////////////////////////////////////////////////////////////////////////////
//计算cos(x)///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double calculate_cos(double x, double precision)
{
    SIMPLE_IF(precision == 0.0, return 0.0);
    //利用:cos(x) = cos(x +- 2PAI)对其简要化简 --- x=[-PAI,PAI]
    double flag = 1.0;
    double period1 = 1.0 * math_pai;
    double period2 = 2.0 * math_pai;
    while (x - period2 >= 0 && (x -= period2));
    while (x + period2 <= 0 && (x += period2));
    (x > 0 && x - period2 > -x && (x -= period2));
    (x < 0 && x + period2 < -x && (x += period2));
    //利用:-cos(x) = cos(x +- PAI)进一步化简 --- x=[-PAI/2,PAI/2]
    (x > 0 && x - period1 > -x && (x -= period1, flag = -flag));
    (x < 0 && x + period1 < -x && (x += period1, flag = -flag));
    
    //SUM(k=(0->n)) ((-1)**k x**(2*k) / (2*k)!)
    unsigned int i = 0, factorial = 1;
    double power = 1.0, temp = 1.0, result = 0.0;

    precision = ABSOLUTE_VALUE(precision);
    for (i = 0; ABSOLUTE_VALUE(temp) >= precision; i++) {
        if (i % 2 == 0)
            result += temp;
        if (i % 2 != 0)
            result -= temp;
        power *= x * x;
        factorial *= (2 * i + 1) * (2 * i + 2);
        temp = power / (double)factorial;
    }
    return flag * result;
}

///////////////////////////////////////////////////////////////////////////////
//计算arctan(x)////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double calculate_arctan(double x, double precision)
{
    SIMPLE_IF(x < -1.0 || x > 1.0, return 0);
    SIMPLE_IF(precision == 0.0, return 0.0);
    
    //SUM(k=(0->n)) ((-1)**k x**(2*k+1) / (2*k+1))
    unsigned int i = 0;
    double power = x, temp = x, result = 0.0;
    
    precision = ABSOLUTE_VALUE(precision);
    for (i = 0; ABSOLUTE_VALUE(temp) >= precision; i++) {
        temp = power / (double)(2 * i + 1);
        if (i % 2 == 0)
            result += temp;
        if (i % 2 != 0)
            result -= temp;
        power *= x * x;
    }
    return result;
}

#endif

///////////////////////////////////////////////////////////////////////////////
//数学扩展:CORDIX ALGORITHM////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define MATH_LIBRARY_EXTENSION_CORDIX_ALGORITHM
#ifdef  MATH_LIBRARY_EXTENSION_CORDIX_ALGORITHM

#define MATH_LIBRARY_EXTENSION_CORDIX_ALGORITHM_ARCTAN_TABLE
#ifdef  MATH_LIBRARY_EXTENSION_CORDIX_ALGORITHM_ARCTAN_TABLE
//问题:C不支持浮点数移位,如果使用整形
//当i达到某一个值时,2^-i是一个非常小的值以至于
//x_current和y_current左移时都为0,此时迭代陷入到困境中
//而且实际精度远不受precision影响而是初始化时其扩大的值影响
//但即使是这样子精度也很低,只能保证其前几位够用(三位后便开始漂移)
//Cordix算法(第一象限):
//x1 =  xcos(t) + ysin(t);
//y1 = -xsin(t) + ycos(t);
//矩阵表达式:                 简化为:
//[x1][ cos(t) sin(t)][x] ==> [x1]cos(t)[   1    tan(t)][x]
//[y1][-sin(t) cos(t)][y] ==> [y1]cos(t)[-tan(t)   1   ][y]
//弧度值表(计算机出现前,关于三角函数都是查表计算(倍角公式)):
//如果未使用硬件浮点数加速模块,这不失为一个好办法,额外空间开销极小
#define ARCTAN_TABLE_MAX 32
static double arctan_table[ARCTAN_TABLE_MAX] = {
    0.785398163397448280000000000000, //arctan(1/(2^0))
    0.463647609000806090000000000000, //arctan(1/(2^1))
    0.244978663126864140000000000000, //arctan(1/(2^2))
    0.124354994546761440000000000000, //arctan(1/(2^3))
    0.062418809995957350000000000000, //arctan(1/(2^4))
    0.031239833430268277000000000000, //arctan(1/(2^5))
    0.015623728620476831000000000000, //arctan(1/(2^6))
    0.007812341060101111100000000000, //arctan(1/(2^7))
    0.003906230131966971800000000000, //arctan(1/(2^8))
    0.001953122516478818800000000000, //arctan(1/(2^9))
    0.000976562189559319460000000000, //arctan(1/(2^10))
    0.000488281211194898290000000000, //arctan(1/(2^11))
    0.000244140620149361770000000000, //arctan(1/(2^12))
    0.000122070311893670210000000000, //arctan(1/(2^13))
    0.000061035156174208773000000000, //arctan(1/(2^14))
    0.000030517578115526096000000000, //arctan(1/(2^15))
    0.000015258789061315762000000000, //arctan(1/(2^16))
    0.000007629394531101970000000000, //arctan(1/(2^17))
    0.000003814697265606496100000000, //arctan(1/(2^18))
    0.000001907348632810187000000000, //arctan(1/(2^19))
    0.000000953674316405960840000000, //arctan(1/(2^20))
    0.000000476837158203088840000000, //arctan(1/(2^21))
    0.000000238418579101557970000000, //arctan(1/(2^22))
    0.000000119209289550780680000000, //arctan(1/(2^23))
    0.000000059604644775390552000000, //arctan(1/(2^24))
    0.000000029802322387695303000000, //arctan(1/(2^25))
    0.000000014901161193847655000000, //arctan(1/(2^26))
    0.000000007450580596923828100000, //arctan(1/(2^27))
    0.000000003725290298461914100000, //arctan(1/(2^28))
    0.000000001862645149230957000000, //arctan(1/(2^29))
    0.000000000931322574615478520000, //arctan(1/(2^30))
    0.000000000465661287307739260000, //arctan(1/(2^31))
};

#endif

///////////////////////////////////////////////////////////////////////////////
//计算sin(x)和cos(x)///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static double sin_and_cos_angle = 0.0;
static double sin_and_cos[2] = {0.0,0.0};
static void calculate_sin_and_cos(double precision)
{
    SIMPLE_IF(precision == 0.0, return);
    //利用:sin(a) = sin(a +- 2PAI)对其简要化简 --- a=[-PAI,PAI]
    //利用:cos(a) = cos(a +- 2PAI)对其简要化简 --- a=[-PAI,PAI]
    double a = sin_and_cos_angle, flag = 1.0;
    double period1 = 1.0 * math_pai;
    double period2 = 2.0 * math_pai;
    while (a - period2 >= 0 && (a -= period2));
    while (a + period2 <= 0 && (a += period2));
    (a > 0 && a - period2 > -a && (a -= period2));
    (a < 0 && a + period2 < -a && (a += period2));
    //利用:-sin(a) = sin(a +- PAI)进一步化简 --- a=[-PAI/2,PAI/2]
    //利用:-cos(a) = cos(a +- PAI)进一步化简 --- a=[-PAI/2,PAI/2]
    (a > 0 && a - period1 > -a && (a -= period1, flag = -flag));
    (a < 0 && a + period1 < -a && (a += period1, flag = -flag));
    //补偿因子:SUM(i=(0->n-1)) (1 + 2**(-2i))**(-1/2) = 0.60725935
    double compensation_factor = 0.60725935;
    double x_next = 0.0, x_current = 0.0;
    double y_next = 0.0, y_current = 0.0;
    double condition = 1.0, angle_flag = 0.0;
    unsigned int i = 0;

    x_current = compensation_factor;
    precision = ABSOLUTE_VALUE(precision);
    for (i = 0; i < ARCTAN_TABLE_MAX && condition >= precision; i++) {
        (a <= 0 && (angle_flag = -1.0));
        (a >  0 && (angle_flag =  1.0));
        //逆时针伪旋转公式:
        x_next = x_current - angle_flag * y_current / (double)(1 << i);
        y_next = y_current + angle_flag * x_current / (double)(1 << i);
        x_current = x_next;
        y_current = y_next;
        a -= angle_flag * arctan_table[i];
        condition /= 2.0;
    }
    sin_and_cos[0] = flag * y_current;
    sin_and_cos[1] = flag * x_current;
}

double calculate_sin_angle(double angle, double precision)
{
    if(angle == sin_and_cos_angle)
        return sin_and_cos[0];
    //计算sin和cos,但只选用sin
    sin_and_cos_angle = angle;
    calculate_sin_and_cos(precision);
    return sin_and_cos[0];
}

double calculate_cos_angle(double angle, double precision)
{
    if(angle == sin_and_cos_angle)
        return sin_and_cos[1];
    //计算sin和cos,但只选用cos
    sin_and_cos_angle = angle;
    calculate_sin_and_cos(precision);
    return sin_and_cos[1];
}

double calculate_tan_angle(double angle, double precision)
{
    if(angle == sin_and_cos_angle)
        return sin_and_cos[0]/sin_and_cos[1];
    //计算sin和cos
    sin_and_cos_angle = angle;
    calculate_sin_and_cos(precision);
    return sin_and_cos[0]/sin_and_cos[1];
    
}

///////////////////////////////////////////////////////////////////////////////
//计算arctan(y,x)//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
double calculate_arctan_yx(double y, double x, double precision)
{
    //目标:已知坐标(也就是正余弦值),求反正切值
    SIMPLE_IF(precision == 0.0, return 0.0);
    //坐标归化为第一四象限的值:
    double flag = 0.0;
    (x < 0 && y > 0 && (x = -x, y = -y, flag = 1.0));
    (x < 0 && y < 0 && (x = -x, y = -y, flag = 1.0));
    //
    double x_next = 0.0, x_current = x;
    double y_next = 0.0, y_current = y;
    double offset = math_pai, condition = 1.0, result = 0.0;
    unsigned int i = 0;
    //
    precision = ABSOLUTE_VALUE(precision);
    for (i = 0; i < ARCTAN_TABLE_MAX && condition >= precision; i++) {
        //伪旋转公式:
        if (y_current < 0) {
            x_next = x_current - y_current / (double)(1 << i);
            y_next = y_current + x_current / (double)(1 << i);
            result -= arctan_table[i];
        }
        if (y_current > 0) {
            x_next = x_current + y_current / (double)(1 << i);
            y_next = y_current - x_current / (double)(1 << i);
            result += arctan_table[i];
        }
        x_current = x_next;
        y_current = y_next;
        condition /= 2.0;
    }
    return result + flag * offset;
}

#endif

///////////////////////////////////////////////////////////////////////////////
//数学扩展:FFT/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define MATH_LIBRARY_EXTENSION_FFT
#ifdef  MATH_LIBRARY_EXTENSION_FFT

#define MATH_LIBRARY_EXTENSION_FFT_CORE
#ifdef  MATH_LIBRARY_EXTENSION_FFT_CORE
///////////////////////////////////////////////////////////////////////////////
//快速傅里叶(变换,逆变换)//////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static int    *rev_list  = NULL;   //递归的最后一层下标表(辅助数组)
static double *real_list = NULL;   //要处理的复数集实数部
static double *imag_list = NULL;   //要处理的复数集虚数部
static int     length    = 0;      //复数集的长度
const double pie = 3.14159265358;  //圆周率PIE(acos(-1))
static void fast_fourier_transform(int flag, double precision)
{
    //1.回溯前的准备,先进入到递归的最后一层,也即回溯的第一层
    int i = -1, j = 0, k = -1, h = 0;
    double temp1 = 0.0, temp2 = 0.0;
    for (j = 0; j < length; rev_list[j] = 0, j++);//初始化
    while ((1 << (++k)) < length);
    while (++i < length)
        rev_list[i]=(rev_list[i >> 1] >> 1) | ((i & 1) << (k - 1));
    //2.接下来交换元素到递归的最后一层(为回溯做好准备)
    for (i = 0; i < length; i++) {
        j = rev_list[i];
        if (i < j) {
            temp1 = real_list[i];
            temp2 = imag_list[i];
            real_list[i] = real_list[j];
            imag_list[i] = imag_list[j];
            real_list[j] = temp1;
            imag_list[j] = temp2;
        }
    }
    //3.从递归的最后一层开始进行回溯(h即表示当前层级单元元素量)
    //对应于分治树的最后一层,此时该层每一个单元只有一个元素,递归的底部
    for (h = 1; h < length; h *= 2) {
        //计算单位根,注意:e**(ix) = cos(x) + i(sin(x))
        double index = flag * pie / h;
        //注:此处仅仅只是为了脱离库依赖,自定义函数在此处并不是很可靠
        double real_w1 = calculate_cos_angle(index, precision);
        double imag_w1 = calculate_sin_angle(index, precision);
        //double real_w1 = calculate_cos(index, precision);
        //double imag_w1 = calculate_sin(index, precision);
        //计算每一个子单元,注意这里j的跨度是2*h,跳过了后面的h个单元
        for (j = 0; j < length; j += (2 * h)) {
            double real_wn = 1.0;//起始运动节点在wn(w0)时
            double imag_wn = 0.0;//每次进动后都偏移一个单位根
            //这里的k跨度是h,所以x为左半部分元素,y为右半部分元素
            for (k = j; k < j + h; k++) {
                double real_x = real_list[k];       //左半部分元素
                double imag_x = imag_list[k];
                double real_y = real_list[k + h];   //右半部分元素
                double imag_y = imag_list[k + h];
                //右半部分比左半部分多出w的偏移,为右半部分旋转wn(计算y*=wn)
                //复数*运算:z1*z2 = (x1x2 - y1y2) + i(x1y2 + x2y1)
                temp1 = real_wn * real_y - imag_wn * imag_y;
                temp2 = real_wn * imag_y + imag_wn * real_y;
                real_y = temp1;
                imag_y = temp2;
                //复数±运算:z1±z2 = (x1 ± x2) + i(y1 ± y2)
                real_list[k]     = real_x + real_y;
                imag_list[k]     = imag_x + imag_y;
                real_list[k + h] = real_x - real_y;
                imag_list[k + h] = imag_x - imag_y;
                //计算下一个单位根(几何意义是单位根旋转,运算是wn*=w1)
                temp1 = real_wn * real_w1 - imag_wn * imag_w1;
                temp2 = real_wn * imag_w1 + imag_wn * real_w1;
                real_wn = temp1;
                imag_wn = temp2;
            }
        }
    }
    //4.判断是否是DFT还是IDFT
    for(i = 0; flag == -1 && i < length; i++) {
        //复数/运算:z1/z2 = ((x1x2 + y1y2) / (x2**2 + y2**2)) + i
        //                  ((x2y1 - x1y2) / (x2**2 + y2**2))
        //当y2 == 0时,退化为:z1/x2 = x1 / x2 + i (y1 / x2)
        real_list[i] /= length;
        imag_list[i] /= length;
    }
}

#endif

void fft(double *real, double *imag, int len, int *rev, double precision)
{
    //容错处理
    if (real == NULL || imag == NULL || rev == NULL || precision <= 0.0)
        return;
    if (len <= 0 || len & (len - 1) != 0)
        return;
    //数据项交付本地
    rev_list  = rev;
    real_list = real;
    imag_list = imag;
    length    = len;
    //启动FFT
    fast_fourier_transform(1, precision);
    //本地复位
    rev_list  = NULL;
    real_list = NULL;
    imag_list = NULL;
    length    = 0;
}

void ifft(double *real, double *imag, int len, int *rev, double precision)
{
    //容错处理
    if (real == NULL || imag == NULL || rev == NULL || precision <= 0.0)
        return;
    if (len <= 0 || len & (len - 1) != 0)
        return;
    //数据项交付本地
    rev_list  = rev;
    real_list = real;
    imag_list = imag;
    length    = len;
    //启动IFFT
    fast_fourier_transform(-1, precision);
    //本地复位
    rev_list  = NULL;
    real_list = NULL;
    imag_list = NULL;
    length    = 0;
}

#endif


