typedef struct PID_Control {
    double T;       /* 设定值 */
    double Kp;      /* 增量系数 */
    double Ki;      /* 积分系数 */
    double Kd;      /* 微分系数 */
    double Et1;      /* 上次偏差 */
    double Sum;     /* 之前的偏差和 */
} PID_Ctrl;

double PID_Process(PID_Ctrl *Ctrl, double Input)
{
/*
    位置PID:
    U[t] = P;
    P = Kp * (E[t]);
    I = Ki * (E[t] + Sum(E[0,t-1]));
    D = Kd * (E[t] - E[t-1]);
*/
    double Etc = Ctrl->T - Input;
    
    double P = Ctrl->Kp * (Etc);
    double I = Ctrl->Ki * (Etc + Ctrl->Sum);
    double D = Ctrl->Kd * (Etc - Ctrl->Et1);
    
    Ctrl->Et1  = Etc;    
    Ctrl->Sum += Etc;
    
    return P + I + D;
}

typedef struct PID_Control {
    double T;       /* 设定值 */
    double Kp;      /* 增量系数 */
    double Ki;      /* 积分系数 */
    double Kd;      /* 微分系数 */
    double Et1;     /* 上次偏差 */
    double Et2;     /* 上上次偏差 */
    double DZ;      /* 死区 */
} PID_Ctrl;

double PID_Process(PID_Ctrl *Ctrl, double Input)
{
/*
    增量PID:
    U[t] = P;
    P = Kp * (E[t] - E[t-1]);
    I = Ki * (E[t]);
    D = Kd * (E[t] - 2 * E[t-1] + E[t - 2])
*/
    double Etc = Ctrl->T - Input;
    
    double P = Ctrl->Kp * (Etc - Ctrl->Et1);
    double I = Ctrl->Ki * (Etc);
    double D = Ctrl->Kd * (Etc + Ctrl->Et2 - Ctrl->Et1 * 2);
    
    Ctrl->Et2  = Ctrl->Et1;
    Ctrl->Et1  = Etc;
    
    return P + I + D;
}