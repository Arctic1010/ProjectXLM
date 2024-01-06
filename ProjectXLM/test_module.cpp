#include<bits/stdc++.h>
#include<windows.h>
#include<graphics.h>
#include"generate.h"
#include"objects.h"

using namespace std;

#define Vec point

const int PLAYER_SPEED = 5;
const int BULLET_SPEED = 7;
const int WIN_SCORE = 3;

const double ROTATE_SPEED = PI / 32;
const double eps = 1e-5;

const int SHOOT_CD = 100;

const int FPS = 30;
enum { FORWARD, BACK };
enum { LEFT, RIGHT };

double ratio = 0.4;
vector<Line_Node>Line_map;
deque<Bullet>Bulletque, T_que;
vector < pair<int, int> > tankpos;
Tank P[3]{ Tank(),Tank(L"material/tank1.png", 30, 30, ratio, 1, P1_opkey),
                  Tank(L"material/tank2.png", 520, 520, ratio, 2, P2_opkey) };

point B_collpos;

point BULLET_GENE = { ratio * 120, ratio * 62 };

IMAGE BULTYPE[2], Background;

#pragma comment(lib,"MSIMG32.LIB")

//打印带透明度的图片
inline void PutImage_Alpha(int x, int y, IMAGE* img)
{
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

void Draw_Map()
{
    setlinecolor(RGB(128, 128, 128));
    setfillcolor(RGB(128, 128, 128));
    for (Line_Node p : Line_map)
    {
        solidrectangle(p.sx, p.sy, p.ex, p.ey);
    }
}
// 计算点积
double dotProduct(Vec v1, Vec v2) 
{
    return v1.x * v2.x + v1.y * v2.y;
}
// 计算叉乘
double crossProduct(Vec v1, Vec v2) 
{
    return v1.x * v2.y - v1.y * v2.x;
}
double magnitude(Vec v) 
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}
// 计算模长
double angleBetween(Vec v1, Vec v2) 
{
    double dot = dotProduct(v1, v2);
    double mag1 = magnitude(v1);
    double mag2 = magnitude(v2);
    if (mag1 == 0 || mag2 == 0) 
    {
        return 0;
    }
    double cosTheta = dot / (mag1 * mag2);
    cosTheta = max(cosTheta, -1.0);
    cosTheta = min(cosTheta, 1.0);
    return acos(cosTheta);
}
point rotatePoint(point a, point o, double angle)
{
     
    angle = -angle;
    double newX = (a.x - o.x) * cos(angle) - (a.y - o.y) * sin(angle) + o.x;
    double newY = (a.x - o.x) * sin(angle) + (a.y - o.y) * cos(angle) + o.y;
    return point{ newX, newY };
}
// 绕某点旋转后的坐标
/*
   t1     t3
    +----+
    |    |
    +----+
   t2     t4
*/
int CheckPointToOBB(Bullet bullet, point t1, point t2, point t3, point t4) 
{
    point bcenter = { bullet.pos.x, bullet.pos.y };
    point tcenter = point{ 1.0 * (t1.x + t2.x) / 2.0, 1.0 * (t1.y + t2.y) / 2.0 };
    // 获取子弹和坦克的中心
    Vec DistanceVector = Vec{ bcenter.x - tcenter.x, bcenter.y - tcenter.y };
    // 获取 OBB 中心到球心的距离向量
    Vec tx = Vec{ 1, 0 };
    // x 轴方向的向量
    double Theta = angleBetween(DistanceVector, tx);
    t1 = rotatePoint(t1, tcenter, Theta);
    t2 = rotatePoint(t2, tcenter, Theta);
    t3 = rotatePoint(t3, tcenter, Theta);
    t4 = rotatePoint(t4, tcenter, Theta);
    bcenter = rotatePoint(bcenter, tcenter, Theta);

    // 全部进行旋转
    point nt[5] = { t1, t2, t3, t4 };
    for (int i = 0; i < 4; i++)  nt[i] = point{ nt[i].x - tcenter.x, nt[i].y - tcenter.y };
    bcenter = point{ bcenter.x - tcenter.x, bcenter.y - tcenter.y };

    if (bcenter.x < 0)  bcenter.x = -bcenter.x;
    if (bcenter.y < 0)  bcenter.y = -bcenter.y;
    Vec v = bcenter, h, u;
    for (int i = 0; i < 4; i++) 
    {
        if (nt[i].x > 0 && nt[i].y > 0)  h = nt[i];
    }
    u = Vec{ max(0.0, v.x - h.x), max(0.0, v.y - h.y) };
    return u.x * u.x + u.y * u.y <= ratio * ratio;
}

bool check_intersection(point a, point b, point c, point d)
{
    //快速排斥实验
    if (max(c.x, d.x) < min(a.x, b.x) || max(a.x, b.x) < min(c.x, d.x) || max(c.y, d.y) < min(a.y, b.y) || max(a.y, b.y) < min(c.y, d.y))
        return false;
    //跨立实验
    if (crossProduct(a - d, c - d) * crossProduct(b - d, c - d) > 0 || crossProduct(d - b, a - b) * crossProduct(c - b, a - b) > 0)
        return false;
    return true;
}

#define left sx
#define top sy
#define right ex
#define bottom ey

// 获取点到矩形的最小距离
double GetDistance_PointToRect(double x, double y, Line_Node rct)
{
    double x_rct, y_rct;	// 保存矩形内到目标点最近的点
    if (x >= rct.left && x <= rct.right)
        x_rct = x;
    else
        x_rct = (double)(fabs(x - rct.left) < fabs(x - rct.right) ? rct.left : rct.right);
    if (y >= rct.top && y <= rct.bottom)
        y_rct = y;
    else
        y_rct = (double)(fabs(y - rct.top) < fabs(y - rct.bottom) ? rct.top : rct.bottom);

    double dx = x - x_rct;
    double dy = y - y_rct;

    return sqrt(dx * dx + dy * dy);
}

// 根据圆的轨迹直线，获取圆与某点相切时的圆心坐标
// 返回是否存在相切
bool GetTangentCirclePoint(
    double x0,		// 切点坐标
    double y0,
    double x1,		// 圆心轨迹直线上的一点（更早运动到的点）
    double y1,
    double x2,		// 圆心轨迹直线上的另一点（其实运动不到的点）
    double y2,
    double r,		// 圆半径
    double* p_out_x,	// 输出圆心坐标
    double* p_out_y
)
{
    // 斜率不存在时
    if (fabs(x1 - x2) < eps)
    {
        // 计算相切时圆心与切点的竖直距离
        double d2 = r * r - (x0 - x1) * (x0 - x1);
        if (d2 < 0)
            return false;
        double d = sqrt(d2);

        // 求出两组解
        double _y1 = y0 + d;
        double _y2 = y0 - d;

        // 保留离 (x1, y1) 更近的解
        double _y_closer = fabs(y1 - _y1) < fabs(y1 - _y2) ? _y1 : _y2;

        *p_out_x = x1;
        *p_out_y = _y_closer;

        return true;
    }

    // 圆心轨迹直线方程：y - y1 = (y2 - y1) / (x2 - x1) * (x - x1)
    // 即：y = kx - kx1 + y1
    // 圆的方程：(x - x0) ^ 2 + (y - y0) ^ 2 = r ^ 2
    // 联立得二次函数，如下。

    double k = (y2 - y1) / (x2 - x1);			// 直线斜率
    double m = -k * x1 + y1 - y0;				// 部分常数
    double a = k * k + 1;						// 二次函数的 abc 系数
    double b = 2 * (k * m - x0);
    double c = x0 * x0 + m * m - r * r;
    double delta = b * b - 4 * a * c;			// 判别式
    if (delta < 0)								// 无解
        return false;
    double sqrt_delta = sqrt(delta);			// 判别式开根号
    double _x1 = (-b + sqrt_delta) / (2 * a);	// 两个根
    double _x2 = (-b - sqrt_delta) / (2 * a);

    // 保留离 (x1, y1) 更近的解
    double _x_closer = fabs(x1 - _x1) < fabs(x1 - _x2) ? _x1 : _x2;
    double _y = k * _x_closer - k * x1 + y1;

    *p_out_x = _x_closer;
    *p_out_y = _y;

    return true;
}

int Check_Tank_With_Bullet(Tank tank, Bullet bullet) 
{
    int Tank_body_collision = CheckPointToOBB(bullet, tank.critical_point[0] + tank.pos, tank.critical_point[6] + tank.pos, tank.critical_point[1] + tank.pos, tank.critical_point[7] + tank.pos);
    int Tank_tube_collision = CheckPointToOBB(bullet, tank.critical_point[2] + tank.pos, tank.critical_point[4] + tank.pos, tank.critical_point[3] + tank.pos, tank.critical_point[5] + tank.pos);
    return (Tank_body_collision | Tank_tube_collision);
}

bool Tank_Bullet_Test(Tank T)
{
    for (int i = 0; i < (int)Bulletque.size(); i++)
    {
        Bullet nowBullet = Bulletque[i];

        if (nowBullet.id == T.id && !nowBullet.is_new) continue;

        if (Check_Tank_With_Bullet(T, nowBullet))  return true;
    }
    return false;
}

// 小球碰撞处理
// rct 碰撞箱区域
// 
// 返回是否发生碰撞
bool BallCollisionProcess(Line_Node rct, Bullet &B)
{
    double B_vx = cos(B.angle) * BULLET_SPEED;
    double B_vy = -sin(B.angle) * BULLET_SPEED;
    //cout << "Vx Vy = " << B_vx << ' ' << B_vy << '\n';
    //cout << "aft afty = " << B.aftpos.x << ' ' << B.aftpos.y << '\n';

    /*
        B.pos.x += B_vx + rand() % 10 / 100;
        B.pos.y += B_vy + rand() % 10 / 100.f;
    */

    // 得到小球上一帧的坐标
    double last_x = B.pos.x;
    double last_y = B.pos.y;

    bool is_collision = false;	// 是否发生碰撞
    // 如果小球不慎进入砖块内部，此时应该回到上一帧，并使速度反向（同时加以扰动）
    /*if (B.aftpos.x >= rct.left && B.aftpos.x <= rct.right
        && B.aftpos.y >= rct.top && B.aftpos.y <= rct.bottom)
    {
        B.aftpos.x = last_x;
        B.aftpos.y = last_y;

        // 如果上一帧仍然陷在里面
        if (B.aftpos.x >= rct.left && B.aftpos.x <= rct.right
            && B.aftpos.y >= rct.top && B.aftpos.y <= rct.bottom)
        {
            // 强制弹出小球
            if (B_vx > 0)
            {
                B.aftpos.x = (double)(rct.left - B.radius);
            }
            else if (B_vx < 0)
            {
                B.aftpos.x = (double)(rct.right + B.radius);
            }

            if (B_vy > 0)
            {
                B.aftpos.y = (double)(rct.top - B.radius);
            }
            else if (B_vy < 0)
            {
                B.aftpos.y = (double)(rct.bottom + B.radius);
            }
            puts("Force pop");
        }

        B_vx *= -1;// +rand() % 10 / 100;
        B_vy *= -1;// +rand() % 10 / 100;

        B.pos.x = B.aftpos.x;// +rand() % 10 / 100;
        B.pos.y = B.aftpos.y;// +rand() % 10 / 100;

        B.angle = atan2(-B_vy, B_vx);

        puts("Collide inside");
        return true;
    }*/

    // 首先需要保证矩形和圆有重叠
    if (!(GetDistance_PointToRect(B.aftpos.x, B.aftpos.y, rct) <= B.radius))
    {
        B.pos.x = B.aftpos.x;// +rand() % 10 / 100;
        B.pos.y = B.aftpos.y;// +rand() % 10 / 100;
        //puts("No collision");
        return false;
    }// 没碰撞
    //else return true;
    //cout << "With rect" << rct.sx << ' ' << rct.sy << ' ' << rct.ex << ' ' << rct.ey << '\n';

    // 穿越碰撞箱边界标记
    /*
    bool cross_left = fabs(B.aftpos.x - rct.left) <= B.radius;
    bool cross_right = fabs(B.aftpos.x - rct.right) <= B.radius;
    bool cross_top = fabs(B.aftpos.y - rct.top) <= B.radius;
    bool cross_bottom = fabs(B.aftpos.y - rct.bottom) <= B.radius;
    */
    bool cross_left = fabs(B.aftpos.x - rct.left) <= B.radius+1;
    bool cross_right = fabs(B.aftpos.x - rct.right) <= B.radius+1;
    bool cross_top = fabs(B.aftpos.y - rct.top) <= B.radius+1;
    bool cross_bottom = fabs(B.aftpos.y - rct.bottom) <= B.radius+1;
    // 标记是否需要判断顶点碰撞
    bool vertex_judge_flag = true;
    double fVertex_X = 0;	// 判定顶点碰撞时使用的顶点
    double fVertex_Y = 0;
    
   /* if (cross_left) cout << "Cross left" << '\n';
    if (cross_right) cout << "Cross right" << '\n';
    if (cross_top) cout << "Cross top" << '\n';
    if (cross_bottom) cout << "Cross bottom" << '\n';
    */

    bool isVertexCollision = false;	// 标记是否发生顶点碰撞

    // 普通碰撞
    if (!isVertexCollision)
    {
        // 跨越碰撞箱左右边界，则水平速度反转
        if (cross_left || cross_right)
        {
            B_vx = -B_vx;
            is_collision = true;
            //cout << "VX reverse" << '\n';
        }
        // 跨越碰撞箱上下边界，则竖直速度反转
        if (cross_top || cross_bottom)
        {
            B_vy = -B_vy;
            is_collision = true;
            //cout << "VY reverse" << '\n';
        }
    }

    if (cross_left && cross_top)			// 左上角
    {
        //vertex_judge_flag = true;
        fVertex_X = (double)rct.left;
        fVertex_Y = (double)rct.top;
    }
    else if (cross_right && cross_top)		// 右上角
    {
        //vertex_judge_flag = true;
        fVertex_X = (double)rct.right;
        fVertex_Y = (double)rct.top;
    }
    else if (cross_left && cross_bottom)	// 左下角
    {
        //vertex_judge_flag = true;
        fVertex_X = (double)rct.left;
        fVertex_Y = (double)rct.bottom;
    }
    else if (cross_right && cross_bottom)	// 右下角
    {
        //vertex_judge_flag = true;
        fVertex_X = (double)rct.right;
        fVertex_Y = (double)rct.bottom;
    }

    // 如果没有同时穿越 xy 两个方向，就需要再评估用哪个顶点
    else
    {
        // 如果穿越上下边界，则就只需要决定使用左边还是右边的顶点
        if (cross_top || cross_bottom)
        {
            fVertex_Y = cross_top ? (double)rct.top : (double)rct.bottom;	// 首先就可以确定 Y

            fVertex_X =
                (fabs(B.aftpos.x - rct.left) < fabs(B.aftpos.x - rct.right)) ?
                (double)rct.left : (double)rct.right;
        }

        // 如果穿越左右边界
        else if (cross_left || cross_right)
        {
            fVertex_X = cross_left ? (double)rct.left : (double)rct.right;

            fVertex_Y =
                (fabs(B.aftpos.y - rct.top) < fabs(B.aftpos.y - rct.bottom)) ?
                (double)rct.top : (double)rct.bottom;
        }
        else
        {
            vertex_judge_flag = false;
        }
    }

    // 优先判断是不是顶点碰撞

    if (vertex_judge_flag)			// 处理顶点碰撞问题
    {
        // 获取碰撞时的小球圆心坐标（即与顶点相切时的坐标）
        double fCollisionX, fCollisionY;
        if (!GetTangentCirclePoint(
            fVertex_X,
            fVertex_Y,
            last_x,
            last_y,
            B.aftpos.x,
            B.aftpos.y,
            (double)B.radius,
            &fCollisionX,
            &fCollisionY
        ))
        {
            // 没有相切，说明顶点碰撞不成立
            goto tag_after_vertex_colision;
        }

        // 如果是真的相切，则相切时矩形到圆心的最近距离应该等于小球半径
        // 但如果此时小于半径，那么说明是假相切
        if (GetDistance_PointToRect(fCollisionX, fCollisionY, rct) < B.radius * 0.98)// 允许一点误差
        {
            goto tag_after_vertex_colision;
        }

        // 计算碰撞时，小球圆心到碰撞点的坐标差
        double f_dx = fCollisionX - fVertex_X;
        double f_dy = fCollisionY - fVertex_Y;

        // 求反射面弧度
        double f_radianReflectingSurface = atan2(f_dy, f_dx);

        // 求法线弧度
        double f_radianNormal = f_radianReflectingSurface + PI / 2;// 或 - PI / 2

        // 求小球入射弧度
        double f_radianIncidence = atan2(B_vy, B_vx);

        // 将小球速度沿法线对称，求得新的速度弧度
        double f_radianReflection = f_radianNormal * 2 - f_radianIncidence;

        // 求速度
        B_vx = cos(f_radianReflection) * BULLET_SPEED;
        B_vy = sin(f_radianReflection) * BULLET_SPEED;

        // 修正小球坐标到相切时的坐标
        B.aftpos.x = fCollisionX;
        B.aftpos.y = fCollisionY;

        isVertexCollision = true;	// 标记发生顶点碰撞

        is_collision = true;
    }
    //puts("Vertex collision");
tag_after_vertex_colision:

    // 回溯坐标，即发生碰撞后，把小球从墙里“拔”出来（回到上一帧的位置），避免穿墙效果
    // 如果发生的是顶点碰撞，那么在前面就已经进行了相切位置修正，就不需要回溯坐标了
    if (!isVertexCollision && is_collision)
    {
        B.aftpos.x = last_x;
        B.aftpos.y = last_y;
    }

    B.pos.x = B.aftpos.x;
    B.pos.y = B.aftpos.y;

    B.angle = atan2(-B_vy,B_vx);
   // puts("Collided");
   // cout << "ANGLE = " << B.angle << '\n';
    
    return is_collision;
    
}// BallCollisionProcess

#undef left
#undef right
#undef top
#undef bottom


//以下代码正在重构
//目标：实现子弹和墙的碰撞

void check_bullet(DWORD test_time)
{
    for (auto i = Bulletque.begin(); i != Bulletque.end(); i++)
    {
        Bullet B = (*i);
        if (test_time - B.exist_time >= BULLET_DURATION)
        {
            P[B.id].bullet_num--;
            continue;
        }
        if (!B.is_new)
        {
            B.is_new = 1;
            B.aftpos = B.pos;

            for (auto line : Line_map)
            {
                if (BallCollisionProcess(line, B)) break;
            }
        }
        else
        {
            B.aftpos.x = B.pos.x + cos(B.angle) * BULLET_SPEED;
            B.aftpos.y = B.pos.y - sin(B.angle) * BULLET_SPEED;
          //  cout << "lstx lsty = " << B.pos.x << ' ' << B.pos.y << '\n';
          //  cout << "aftx afty = " << B.aftpos.x << ' ' << B.aftpos.y << '\n';
            for (auto line : Line_map)
            {
                if(BallCollisionProcess(line, B)) break;
            }

            //B.pos = B.aftpos;
        }
        if (B.pos.x < 0 || B.pos.y < 0)
        {
            P[B.id].bullet_num--;
            continue;
        }
        T_que.push_back(B);
        //B.vx = cos(B.angle) * BULLET_SPEED;
        //B.vy =-sin(B.angle) * BULLET_SPEED;
    }

    Bulletque = T_que;
    T_que.clear();
}
//以上代码正在重构

void Tank::Tank_Peek(ExMessage msg)
{
    if (msg.message == WM_KEYDOWN)
    {
        if (msg.vkcode == op_key[U])
        {
            is_move[FORWARD] = 1;
        }
        else if (msg.vkcode == op_key[D])
        {
            is_move[BACK] = 1;
        }
        else if (msg.vkcode == op_key[L])
        {
            is_rotate[LEFT] = 1;
        }
        else if (msg.vkcode == op_key[R])
        {
            is_rotate[RIGHT] = 1;
        }
        else if (msg.vkcode == op_key[S])
        {
            is_shooting = 1;
        }
    }
    if (msg.message == WM_KEYUP)
    {
        if (msg.vkcode == op_key[U])
        {
            is_move[FORWARD] = 0;
        }
        else if (msg.vkcode == op_key[D])
        {
            is_move[BACK] = 0;
        }
        else if (msg.vkcode == op_key[L])
        {
            is_rotate[LEFT] = 0;
        }
        else if (msg.vkcode == op_key[R])
        {
            is_rotate[RIGHT] = 0;
        }
        else if (msg.vkcode == op_key[S])
        {
            is_shooting = 0;
            shoot_time = 0;
        }
    }
}
//向量叉积判断线条是否与墙壁重合
bool Tank::check_line(point start, point dest)
{
    bool flag = false;
    for (auto t : Line_map)
    {
        flag = (check_intersection(start, dest, { t.sx,t.sy }, { t.sx,t.ey }) ||
            check_intersection(start, dest, { t.sx,t.ey }, { t.ex,t.ey }) ||
            check_intersection(start, dest, { t.ex,t.ey }, { t.ex,t.sy }) ||
            check_intersection(start, dest, { t.ex,t.sy }, { t.sx,t.sy }));
        if (flag) return false;//重合
    }
    return true;
}

point T_critical_point[8];

void Tank::Tank_Move()
{
    aftpos.x = pos.x, aftpos.y = pos.y;
    aftangle = angle;
    if (is_move[FORWARD])
    {
        aftpos.x = pos.x + cos(angle) * PLAYER_SPEED; // 尝试移动玩家
        aftpos.y = pos.y - sin(angle) * PLAYER_SPEED;
    }
    if (is_move[BACK])
    {
        aftpos.x = pos.x - cos(angle) * PLAYER_SPEED;
        aftpos.y = pos.y + sin(angle) * PLAYER_SPEED;
    }
    if (is_rotate[LEFT])
    {
        aftangle += ROTATE_SPEED;
        if (aftangle > PI) aftangle -= PI * 2; //保持 aftangle 一直在 -PI ~ PI 范围内
    }
    if (is_rotate[RIGHT])
    {
        aftangle -= ROTATE_SPEED;
        if (aftangle < -PI) aftangle += PI * 2;
    }
    if (is_shooting) // 发射子弹判断
    {
        if (bullet_num < BULLET_MAXNUM)
        {
            if (!shoot_time || GetTickCount64() - shoot_time >= SHOOT_CD)
            {
                bullet_num++;
                shoot_time = GetTickCount64();
                Bulletque.push_back(Bullet(rotatePoint(bullet_gene_pos_D + pos,centerpos + pos, angle), ratio, id, 0, GetTickCount64(), angle));
            }
        }
    }
    // ----- 整点方向调整，防止精度问题 -----

    if (fabs(aftangle) <= eps) aftangle = 0;
    if (fabs(aftangle - PI) <= eps) aftangle = PI;
    if (fabs(aftangle - PI / 2) <= eps) aftangle = PI / 2;
    if (fabs(aftangle + PI / 2) <= eps) aftangle = -PI / 2;

    // ----- 已重构，玩家和墙的碰撞检测 ----- 

    for (int i = 0; i < 8; i++)
    {//注意先尝试移动
        T_critical_point[i] = rotatePoint(TANK_CPOINT[i] * ratio, centerpos, aftangle);
    }

    bool can_move = check_line(aftpos + T_critical_point[7], aftpos + T_critical_point[0]);

    for (int i = 0; i < 7 && can_move; i++)
    {
        can_move &= check_line(aftpos + T_critical_point[i], aftpos + T_critical_point[i+1]);
    }
    
    if (can_move) // 移动玩家坦克
    {
        pos.x = aftpos.x;
        pos.y = aftpos.y;
        angle = aftangle;

        for (int i = 0; i < 8; i++)
        {
            critical_point[i] = T_critical_point[i];
        }
    }

    // TODO: 碰撞移动补正
}

void Tank::Draw()
{
    //cout << pos.x << " " << pos.y << "ASDADADSASDASSDA\n";
    rotateimage(&img_rotate, &img, angle, TRANSPARENT, false, false);
    PutImage_Alpha(round(pos.x), round(pos.y), &img_rotate);
}

enum { MAIN, GAME_PLAY, HELP, WIN };
// 定义Widget类，表示一个简单的图形用户界面
class Widget
{
private:
    int width; // 宽度
    int height; // 高度
    int currentIndex; // 当前页面索引
    vector<IMAGE*> pages; // 存储所有页面的图片指针
    vector<vector<Button*>> buttons; // 存储每个页面上的按钮

    // 添加一个页面
    void addPage(IMAGE* page)
    {
        pages.push_back(page);
        buttons.push_back({});
    }

    void modifyPage(IMAGE* page, int idx)
    {
        assert(idx < pages.size());
        pages[idx] = page;
    }

    // 在指定页面上添加一个按钮
    void addButton(int index, Button* button)
    {
        if (index >= 0 && index < buttons.size())
        {
            buttons[index].push_back(button);
        }
    }

    // 设置当前显示的页面索引
    void setCurrentIndex(int index)
    {
        if (index >= 0 && index < pages.size())
        {
            currentIndex = index;
        }
    }

    int getCurrentIndex()
    {
        return currentIndex;
    }

    // 处理鼠标点击事件
    void mouseClick(int mouseX, int mouseY)
    {
        if (currentIndex >= 0 && currentIndex < buttons.size())
        {
            for (Button* button : buttons[currentIndex])
            {
                if (button->checkClick(mouseX, mouseY))
                {
                    break;
                }
            }
        }
    }

    // 处理鼠标移动事件
    void mouseMove(int mouseX, int mouseY)
    {
        if (currentIndex >= 0 && currentIndex < buttons.size())
        {
            for (Button* button : buttons[currentIndex])
            {
                button->checkMouseOver(mouseX, mouseY);
            }
        }
    }

    // 绘制当前页面的内容
    void draw_background_and_button(bool have_background)
    {
        if (currentIndex >= 0 && currentIndex < pages.size())
        {
            if(have_background)
                putimage(0, 0, pages[currentIndex]); // 在窗口中绘制当前页面的图片

            if (currentIndex >= 0 && currentIndex < buttons.size())
            {
                for (Button* button : buttons[currentIndex])
                {
                    button->draw(); // 绘制当前页面上的所有按钮
                }
            }
        }
    }

public:

    void run_main();
    void run_game_play();
    void run_help();
    void run_win(int win_id,int score_1,int score_2);

    Widget(int width, int height):width(width), height(height), currentIndex(-1)
    {
    }
    ~Widget() {}

    void game_round_init()
    {
        Line_map = Pre_Draw_Graph(25, 25, 1000, 715);

        IMAGE* page_game_play = new IMAGE(width, height);
        //setbkcolor();
        setfillcolor(RGB(0xff, 0xff, 0xff));
        solidrectangle(0, 0, width, height);
        Draw_Map();
        getimage(page_game_play, 0, 0, width, height); // 将页面2的内容保存到图片中

        modifyPage(page_game_play, GAME_PLAY);

        //P[1].pos = { 30,30 };
        //P[2].pos = { 940,665 };

        srand((unsigned)time(0));
        int xid = rand() % tankpos.size();
        int yid = rand() % tankpos.size();
        while (xid == yid) {
            yid = rand() % tankpos.size();
        }
        point p1tank[8];
        P[1].pos = { tankpos[xid].first, tankpos[xid].second };
        p1tank[0] = { tankpos[xid].first, tankpos[xid].second };
        p1tank[1] = { p1tank[0].x + 89, p1tank[0].y };
        p1tank[2] = { p1tank[1].x, p1tank[1].y + 27 };
        p1tank[3] = { p1tank[2].x + 15,p1tank[2].y };
        p1tank[4] = { p1tank[3].x,p1tank[3].y + 14 };
        p1tank[5] = { p1tank[4].x - 15, p1tank[4].y };
        p1tank[6] = { p1tank[5].x, p1tank[5].y + 27 };
        p1tank[7] = { p1tank[6].x - 89, p1tank[6].y };
        for (int i = 0; i < 8; i++) {
            P[1].critical_point[i] = p1tank[i];
        }
        P[2].pos = { tankpos[yid].first, tankpos[yid].second };
        p1tank[0] = { tankpos[yid].first, tankpos[yid].second };
        p1tank[1] = { p1tank[0].x + 89, p1tank[0].y };
        p1tank[2] = { p1tank[1].x, p1tank[1].y + 27 };
        p1tank[3] = { p1tank[2].x + 15,p1tank[2].y };
        p1tank[4] = { p1tank[3].x,p1tank[3].y + 14 };
        p1tank[5] = { p1tank[4].x - 15, p1tank[4].y };
        p1tank[6] = { p1tank[5].x, p1tank[5].y + 27 };
        p1tank[7] = { p1tank[6].x - 89, p1tank[6].y };
        for (int i = 0; i < 8; i++) {
            P[2].critical_point[i] = p1tank[i];
        }

        P[1].angle = P[1].aftangle = PI / 32 * (rand() % 64) - PI;
        P[2].angle = P[2].aftangle = PI / 32 * (rand() % 64) - PI;
    }

    // 初始化控件，创建图形环境，设置页面和按钮，设置变量初始化
    void init()
    {
        loadimage(&BULTYPE[0], L"material/bullet1.png", BULLET_SIZE, BULLET_SIZE);
        loadimage(&Background, L"material/background.jpg", 1280, 720);

        initgraph(width, height);

        // 创建页面1
        IMAGE* page_main = new IMAGE(width, height);
        loadimage(page_main, L"material/background.jpg");

        addPage(page_main); // 添加页面1

        Button* button0_start_game = new Button(100, 450, 200, 50, L"GAME START", [&]() {
            setCurrentIndex(GAME_PLAY);
            });
        addButton(MAIN, button0_start_game);

        Button* button0_help = new Button(100, 550, 200, 50, L"HELP", [&]() {
            setCurrentIndex(HELP);
            });
        addButton(MAIN, button0_help); 

        Button* button0_exit = new Button(100, 650, 200, 50, L"EXIT", [&]() {
            exit(0);
            });
        addButton(MAIN, button0_exit); 

        // 创建页面2
        IMAGE* page_game_play = new IMAGE(width, height);
        setbkcolor(RGB(0xff, 0xff, 0xff));
        getimage(page_game_play, 0, 0, width, height); // 将页面2的内容保存到图片中

        addPage(page_game_play); // 添加页面2

        Button* button1_return = new Button(1120, 620, 140, 50, L"BACK TO MENU", [&]() {
            setCurrentIndex(MAIN); // 点击按钮1时，切换回页面1
            });
        addButton(GAME_PLAY, button1_return); // 将按钮1添加到页面2

        // 创建页面3
        IMAGE* page_help = new IMAGE(width, height);
        putimage(0, 0, &Background);
        setlinecolor(RGB(192, 192, 192));
        setfillcolor(RGB(192, 192, 192));
        solidrectangle(300, 169, 980, 551);//临时绘制页面内容在屏幕
        getimage(page_help, 0, 0, width, height); //保存页面内容

        addPage(page_help); 

        Button* button2_return = new Button(840, 471, 140, 50, L"BACK TO MENU", [&]() {
            setCurrentIndex(MAIN); 
            });
        addButton(HELP, button2_return); 

        //创建page4
        IMAGE* page_win = new IMAGE(width, height);
        loadimage(page_win, L"material/win.jpg");

        addPage(page_win); // 添加页面4

        Button* button3_return = new Button(1120, 620, 140, 50, L"BACK TO MENU", [&]() {
            setCurrentIndex(MAIN); 
            });
        addButton(WIN, button3_return); 


        setCurrentIndex(MAIN); // 设置初始显示页面为页面1
    }

    // 关闭
    void close()
    {
        closegraph(); // 关闭图形环境
    }
};

//const point TANK_CPOINT[8] = { {15,33},{104,33},{104,60},{119,60},
//                               {119,74},{104,74},{104,101},{15,101} };//真实坦克八个顶点的位置
void Widget::run_main()//主界面 图形化操作
{
    ExMessage msg;
    BeginBatchDraw(); // 开始批量绘制
    while (getCurrentIndex() == MAIN)
    {
        DWORD start_time = GetTickCount64();
        while (peekmessage(&msg)) // 检查是否有消息
        {
            int mouseX = msg.x; // 获取鼠标x坐标
            int mouseY = msg.y; // 获取鼠标y坐标

            switch (msg.message)
            {
            case WM_LBUTTONDOWN: // 鼠标左键按下事件
                mouseClick(mouseX, mouseY); // 处理鼠标点击事件
                break;
            case WM_MOUSEMOVE: // 鼠标移动事件
                mouseMove(mouseX, mouseY); // 处理鼠标移动事件
                break;
            }
        }

        cleardevice();
        draw_background_and_button(1); // 绘制当前页面内容
        FlushBatchDraw(); // 将缓冲区内容显示在屏幕上

        DWORD end_time = GetTickCount64();
        DWORD delta_time = end_time - start_time;
        if (delta_time < 1000 / FPS)
        {
            Sleep(1000 / FPS - delta_time);
        }
    }

    EndBatchDraw(); // 结束批量绘制
    
    int now_index = getCurrentIndex();
    switch(now_index)
    {
    case GAME_PLAY:
        run_game_play();
        break;
    case HELP:
        run_help();
        break;
    }
}

void Widget::run_game_play()//游戏界面 图形化操作
{
    ExMessage msg;

    int frame_num = FPS;
    int score_1 = 0, score_2 = 0, win_id = 0;
    wchar_t str_fps[15], str_ready[14], str_go[10], str_score1[3], str_score2[3];
    wsprintf(str_fps, L"FPS = %d", frame_num);
    wsprintf(str_ready, L"Get Ready...");
    wsprintf(str_go, L"Fire!");
    wsprintf(str_score1, L"0");
    wsprintf(str_score2, L"0");

here:
    bool is_dead_1 = 0, is_dead_2 = 0;
    P[1] = Tank(L"material/tank1.png", -1, -1, ratio, 1, P1_opkey);
    P[2] = Tank(L"material/tank2.png", -1, -1, ratio, 2, P2_opkey);
    while (Bulletque.size()) Bulletque.pop_back();
    
    game_round_init();
    
    BeginBatchDraw();
    DWORD func_begin_time = GetTickCount64();
    DWORD nowtime = GetTickCount64();

    bool should_move = 1, is_in_ready = 1;

    while (getCurrentIndex() == GAME_PLAY)
    {
        DWORD start_time = GetTickCount64();

        while (peekmessage(&msg))
        {
            if (!is_in_ready && should_move)
            {
                P[1].Tank_Peek(msg);
                P[2].Tank_Peek(msg);
            }
            

            int mouseX = msg.x; // 获取鼠标x坐标
            int mouseY = msg.y; // 获取鼠标y坐标

            switch (msg.message)
            {
            case WM_LBUTTONDOWN: // 鼠标左键按下事件
                mouseClick(mouseX, mouseY); // 处理鼠标点击事件
                break;
            case WM_MOUSEMOVE: // 鼠标移动事件
                mouseMove(mouseX, mouseY); // 处理鼠标移动事件
                break;
            }
        }
        DWORD test_time = GetTickCount64();
        if (test_time - nowtime >= 1000)
        {
            // ----- TEST AREA -----
            //cout << "FPS: " << frame_num << '\n';
            wsprintf(str_fps, L"FPS = %d", frame_num);
            frame_num = 0;
            //cout << "Now Bullet Queue\n";
            for (Bullet B : Bulletque)
            {
                cout << "X = " << B.pos.x << " Y = " << B.pos.y << " Angle = " << B.angle << '\n';
            }
            nowtime = test_time;
        }
        

        if (!is_in_ready && should_move)
        {
            P[1].Tank_Move();
            P[2].Tank_Move();

            check_bullet(test_time);
        }
        
        if (Tank_Bullet_Test(P[2])) {
            is_dead_2 = 1;
            score_1++;
            wsprintf(str_score1, L"%d", score_1);
            goto here;
        }
        if (Tank_Bullet_Test(P[1])) {
            is_dead_1 = 1;
            score_2++;
            wsprintf(str_score2, L"%d", score_2);
            goto here;
        }
        if (score_1 >= WIN_SCORE) {
            win_id = 1;
            cout << "1 win" << '\n';
            break;
        }
        else if (score_2 >= WIN_SCORE) {
            win_id = 2;
            cout << "1 win" << '\n';
            break;
        }

        cleardevice();        

        draw_background_and_button(1);

        settextcolor(BLACK);
        settextstyle(18, 0, L"Consolas");
        outtextxy(1200, 10, str_fps);

        settextstyle(75, 0, L"Agency FB");
        settextcolor(BLACK);
        outtextxy(1210, 250, str_score1);
        settextcolor(BLACK);
        outtextxy(1210, 450, str_score2);

        settextcolor(RGB(235, 51, 36));
        outtextxy(1050, 250, L"RED");
        settextcolor(RGB(22, 65, 124));
        outtextxy(1050, 450, L"BLUE");

        P[1].Draw();
        P[2].Draw();

        if (test_time - func_begin_time < 2000)
        {
            settextcolor(RGB(255, 50, 35));
            settextstyle(150, 0, L"Agency FB");
            outtextxy(300, 300, str_ready);
        }
        else if (test_time - func_begin_time < 2500)
        {
            is_in_ready = 0;
            settextcolor(RGB(0, 155, 150));
            settextstyle(150, 0, L"Agency FB");
            outtextxy(450, 300, str_go);
        }

        for (Bullet B : Bulletque)
        {
            PutImage_Alpha(round(B.pos.x - BULLET_D), round(B.pos.y - BULLET_D), &BULTYPE[B.type]);
        }
        
        FlushBatchDraw(); // 将缓冲区内容显示在屏幕上

        DWORD end_time = GetTickCount64();
        DWORD delta_time = end_time - start_time;
        if (delta_time < 1000 / FPS)
        {
            Sleep(1000 / FPS - delta_time);
        }
        frame_num++;
    }
    EndBatchDraw(); // 结束批量绘制

    if (getCurrentIndex() == MAIN)
    {
        run_main();
    }
    if (win_id != 0)
    {
        setCurrentIndex(WIN);
        run_win(win_id,score_1,score_2);
    }
}

void Widget::run_win(int win_id, int score_1, int score_2)
{
    ExMessage msg;
    wchar_t str_win_id[9], str_win_str[20], str_score1[3], str_score2[3];
    if (win_id == 1) wsprintf(str_win_id, L"RED");
    else if (win_id == 2) wsprintf(str_win_id, L"BLUE");
    else assert(0);

    wsprintf(str_win_str, L" won the game!");
    wsprintf(str_score1, L"%d", score_1);
    wsprintf(str_score2, L"%d", score_2);


    BeginBatchDraw(); // 开始批量绘制

    while (getCurrentIndex() == WIN)
    {
        DWORD start_time = GetTickCount64();
        while (peekmessage(&msg)) // 检查是否有消息
        {
            int mouseX = msg.x; // 获取鼠标x坐标
            int mouseY = msg.y; // 获取鼠标y坐标

            switch (msg.message)
            {
            case WM_LBUTTONDOWN: // 鼠标左键按下事件
                mouseClick(mouseX, mouseY); // 处理鼠标点击事件
                break;
            case WM_MOUSEMOVE: // 鼠标移动事件
                mouseMove(mouseX, mouseY); // 处理鼠标移动事件
                break;
            }
        }

        cleardevice();

        draw_background_and_button(1); // 绘制当前页面内容
        /*
        settextcolor(RGB(235, 51, 36));
        outtextxy(1050, 250, L"RED");
        settextcolor(RGB(22, 65, 124));
        outtextxy(1050, 450, L"BLUE");
        */
        if(win_id==1) settextcolor(RGB(235, 51, 36));
        if(win_id==2) settextcolor(RGB(22, 65, 124));
        settextstyle(100, 0, L"Agency FB");
        outtextxy(680, 80, str_win_id);
        settextcolor(BLACK);
        outtextxy(680 + textwidth(str_win_id), 80, str_win_str);

        settextstyle(70, 0, L"Agency FB");
        outtextxy(720, 230, L"Final score:");

        settextstyle(100, 0, L"Agency FB");
        settextcolor(RGB(235, 51, 36));
        outtextxy(780, 350, str_score1);
        settextcolor(BLACK);
        outtextxy(880, 350, L":");
        settextcolor(RGB(22, 65, 124));
        outtextxy(980, 350, str_score2);

        FlushBatchDraw(); // 将缓冲区内容显示在屏幕上

        DWORD end_time = GetTickCount64();
        DWORD delta_time = end_time - start_time;
        if (delta_time < 1000 / FPS)
        {
            Sleep(1000 / FPS - delta_time);
        }
    }

    EndBatchDraw(); // 结束批量绘制

    if (getCurrentIndex() == MAIN)
    {
        run_main();
    }
}

void Widget::run_help()//帮助界面 图形化操作
{
    ExMessage msg;

    BeginBatchDraw(); // 开始批量绘制

    while (getCurrentIndex() == HELP)
    {
        DWORD start_time = GetTickCount64();
        while (peekmessage(&msg)) // 检查是否有消息
        {
            int mouseX = msg.x; // 获取鼠标x坐标
            int mouseY = msg.y; // 获取鼠标y坐标

            switch (msg.message)
            {
            case WM_LBUTTONDOWN: // 鼠标左键按下事件
                mouseClick(mouseX, mouseY); // 处理鼠标点击事件
                break;
            case WM_MOUSEMOVE: // 鼠标移动事件
                mouseMove(mouseX, mouseY); // 处理鼠标移动事件
                break;
            }
        }

        cleardevice();

        draw_background_and_button(1); // 绘制当前页面内容
        FlushBatchDraw(); // 将缓冲区内容显示在屏幕上

        DWORD end_time = GetTickCount64();
        DWORD delta_time = end_time - start_time;
        if (delta_time < 1000 / FPS)
        {
            Sleep(1000 / FPS - delta_time);
        }
    }

    EndBatchDraw(); // 结束批量绘制

    if (getCurrentIndex() == MAIN)
    {
        run_main();
    }
}
void TankInit() {
    int lx = 25, ly = 25;
    int rx = 1000, ry = 715;
    for (int i = 1; i <= 14; i++) {
        for (int j = 1; j <= 10; j++) {
            int X1 = lx + (i - 1) * 70;
            int Y1 = ly + (j - 1) * 70;
            X1 = min(X1, 1000);
            Y1 = min(Y1, 715);
            int X2 = X1 + 70 - 1;
            int Y2 = Y1 + 70 - 1;
            X2 = min(X2, 1000);
            Y2 = min(Y2, 715);
            if (min(Y2 - Y1 + 1, X2 - X1 + 1) < 70)  continue;
            tankpos.push_back({X1 + 10, Y1 + 10});
        }
    }
    return;
}
int main()
{
    //game_init();
    TankInit();
    Widget widget(1280, 720);
    widget.init();
    widget.run_main();
    //printf("%d %d\n", Point1, Point2);
    widget.close();
    return 0;
}