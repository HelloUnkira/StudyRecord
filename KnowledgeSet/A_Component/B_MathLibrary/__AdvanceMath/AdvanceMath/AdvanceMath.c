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
