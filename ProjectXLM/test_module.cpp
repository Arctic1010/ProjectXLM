#include<bits/stdc++.h>
#include<graphics.h>
#include"generate.h"
#include"objects.h"

using namespace std;

#define Vec point

const int PLAYER_SPEED = 5;
const int BULLET_SPEED = 7;

const double ROTATE_SPEED = PI / 32;
const double eps = 1e-5;

const int SHOOT_CD = 100;

const int FPS = 30;
enum { FORWARD, BACK };
enum { LEFT, RIGHT };

double ratio = 0.4;
vector<Line_Node>Line_map;
deque<Bullet>Bulletque, T_que;

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
double dotProduct(Vec v1, Vec v2) {
    return v1.x * v2.x + v1.y * v2.y;
}
// 计算叉乘
double crossProduct(Vec v1, Vec v2) 
{
    return v1.x * v2.y - v1.y * v2.x;
}
double magnitude(Vec v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}
// 计算模长
double angleBetween(Vec v1, Vec v2) {
    double dot = dotProduct(v1, v2);
    double mag1 = magnitude(v1);
    double mag2 = magnitude(v2);
    if (mag1 == 0 || mag2 == 0) {
        return 0;
    }
    double cosTheta = dot / (mag1 * mag2);
    cosTheta = max(cosTheta, -1.0);
    cosTheta = min(cosTheta, 1.0);
    return acos(cosTheta);
}
point rotatePoint(point a, point o, double angle) {
     
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

int Check_Tank_With_Bullet(Tank tank, Bullet bullet) 
{
    int Tank_body_collision = CheckPointToOBB(bullet, tank.critical_point[0] + tank.pos, tank.critical_point[6] + tank.pos, tank.critical_point[1] + tank.pos, tank.critical_point[7] + tank.pos);
    int Tank_tube_collision = CheckPointToOBB(bullet, tank.critical_point[2] + tank.pos, tank.critical_point[4] + tank.pos, tank.critical_point[3] + tank.pos, tank.critical_point[5] + tank.pos);
    return (Tank_body_collision | Tank_tube_collision);
}

//以下代码待重构
//目标：计算几何实现子弹和墙的碰撞检测
bool Tank_Bullet_Test(Tank T) 
{
    for (int i = 0; i < (int)Bulletque.size(); i++) 
    {
        Bullet nowBullet = Bulletque[i];
        if (Check_Tank_With_Bullet(T, nowBullet))  return true;
    }
    return false;
}

bool check_horizontal(double radian)
{
    if (radian >= -PI / 4 && radian <= PI / 4) return 1;
    if (radian >= PI * 3 / 4 && radian <= PI) return 1;
    if (radian <= -PI * 3 / 4 && radian >= -PI) return 1;
    return 0;
}

bool check_vertical(double radian)
{
    if (radian >= PI / 4 && radian <= PI * 3 / 4) return 1;
    if (radian <= -PI / 4 && radian >= -PI * 3 / 4) return 1;
    return 0;
}

bool check_hor_ver(point p)
{
    int hor = 0, ver = 0;
    for (double i = p.x; i <= p.x + 10; i++)
    {
        if (getpixel(floor(i), floor(p.y)) == RGB(128, 128, 128)) hor++;
    }
    for (double i = p.x; i >= p.x - 10; i--)
    {
        if (getpixel(floor(i), floor(p.y)) == RGB(128, 128, 128)) hor++;
    }
    for (double i = p.y; i <= p.y + 10; i++)
    {
        if (getpixel(floor(p.x), floor(i)) == RGB(128, 128, 128)) ver++;
    }
    for (double i = p.y; i >= p.y - 10; i--)
    {
        if (getpixel(floor(p.x), floor(i)) == RGB(128, 128, 128)) ver++;
    }

    if (hor > ver) return 1;
    return 0;

}

bool check_bullet_wall(Bullet& B)
{
    double radian = -B.angle;

    Vec v = { B.aftpos.x - B.pos.x,B.aftpos.y - B.pos.y };
    double t = sqrt(v.x * v.x + v.y * v.y);



    for (double x = B.pos.x, y = B.pos.y; fabs(x - B.aftpos.x) >= 0.1 || fabs(y - B.aftpos.y) >= 0.1; x += v.x / t, y += v.y / t)
    {
        //cout << x << ' ' << y << '\n';
        if (getpixel(floor(x), floor(y)) == RGB(128, 128, 128))
        {
            bool hvflag = check_hor_ver(point(x, y));
            v = { B.aftpos.x - x,B.aftpos.y - y };
            if (hvflag)
            {
                //cout << "collision horizontally.\n";
                v.y = -v.y;
                B.angle = -B.angle;
            }
            else
            {
                //cout << "collision vertically.\n";
                v.x = -v.x;
                B.angle = PI - B.angle;
            }
            B.aftpos = { x + v.x,y + v.y };
            return 1;
        }
    }
    return 0;
}

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
        B.aftpos = B.pos;
        if (!B.is_new) B.is_new = 1;
        else
        {
            B.aftpos.x = B.pos.x + cos(B.angle) * BULLET_SPEED;
            B.aftpos.y = B.pos.y - sin(B.angle) * BULLET_SPEED;
        }

        check_bullet_wall(B);

        B.pos.x = B.aftpos.x;
        B.pos.y = B.aftpos.y;
        T_que.push_back(B);
    }

    Bulletque = T_que;
    T_que.clear();
}
//以上代码待重构

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
            if (!shoot_time || GetTickCount() - shoot_time >= SHOOT_CD)
            {
                bullet_num++;
                shoot_time = GetTickCount();
                Bulletque.push_back(Bullet(rotatePoint(bullet_gene_pos_D + pos,centerpos + pos, angle), ratio, id, 0, GetTickCount(), angle));
            }
        }
    }
    // ----- 整点方向调整，防止精度问题 -----

    if (fabs(aftangle) <= eps) aftangle = 0;
    if (fabs(aftangle - PI) <= eps) aftangle = PI;
    if (fabs(aftangle - PI / 2) <= eps) aftangle = PI / 2;
    if (fabs(aftangle + PI / 2) <= eps) aftangle = -PI / 2;

    // ----- 已重构，玩家和墙的碰撞检测 -----
    bool can_move = check_line(aftpos + critical_point[7], aftpos + critical_point[0]);

    for (int i = 0; i < 7 && can_move; i++)
    {
        can_move &= check_line(aftpos + critical_point[i], aftpos + critical_point[i+1]);
    }
    
    if (can_move) // 移动玩家坦克
    {
        pos.x = aftpos.x;
        pos.y = aftpos.y;
        angle = aftangle;

        for (int i = 0; i < 8; i++)
        {
            critical_point[i] = rotatePoint(TANK_CPOINT[i] * ratio, centerpos, angle);
        }
    }

    // TODO: 碰撞移动补正
}

void Tank::Draw()
{
    rotateimage(&img_rotate, &img, angle, TRANSPARENT, false, false);
    PutImage_Alpha(round(pos.x), round(pos.y), &img_rotate);
}

enum { MAIN, GAME_PLAY, HELP };
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

    Widget(int width, int height):width(width), height(height), currentIndex(-1)
    {
    }
    ~Widget() {}

    void game_round_init()
    {
        Line_map = Pre_Draw_Graph(25, 25, 1000, 715);

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

        Button* button0_start_game = new Button(100, 450, 200, 50, L"开始游戏", [&]() {
            setCurrentIndex(GAME_PLAY);
            });
        addButton(MAIN, button0_start_game);

        Button* button0_help = new Button(100, 550, 200, 50, L"帮助", [&]() {
            setCurrentIndex(HELP);
            });
        addButton(MAIN, button0_help); 

        Button* button0_exit = new Button(100, 650, 200, 50, L"退出游戏", [&]() {
            exit(0);
            });
        addButton(MAIN, button0_exit); 

        // 创建页面2
        IMAGE* page_game_play = new IMAGE(width, height);
        setbkcolor(RGB(0xff, 0xff, 0xff));
        getimage(page_game_play, 0, 0, width, height); // 将页面2的内容保存到图片中

        addPage(page_game_play); // 添加页面2

        Button* button1_return = new Button(1120, 620, 100, 50, L"返回到主界面", [&]() {
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

        Button* button2_return = new Button(850, 471, 100, 50, L"返回到主界面", [&]() {
            setCurrentIndex(MAIN); 
            });
        addButton(HELP, button2_return); 


        setCurrentIndex(MAIN); // 设置初始显示页面为页面1
    }

    // 关闭
    void close()
    {
        closegraph(); // 关闭图形环境
    }
};

void Widget::run_main()//主界面 图形化操作
{
    ExMessage msg;

    BeginBatchDraw(); // 开始批量绘制

    while (getCurrentIndex() == MAIN)
    {
        DWORD start_time = GetTickCount();
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

        DWORD end_time = GetTickCount();
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

    int frame_num = 0;

    wchar_t str[15];
    wsprintf(str, L"FPS = %d", frame_num);

    Line_map = Pre_Draw_Graph(25, 25, 1000, 715);

    BeginBatchDraw();
    DWORD nowtime = GetTickCount();
    while (getCurrentIndex() == GAME_PLAY)
    {
        DWORD start_time = GetTickCount();

        while (peekmessage(&msg))
        {
            P[1].Tank_Peek(msg);
            P[2].Tank_Peek(msg);

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
        DWORD test_time = GetTickCount();
        if (test_time - nowtime >= 1000)
        {
            // ----- TEST AREA -----
            //cout << "FPS: " << frame_num << '\n';
            wsprintf(str, L"FPS = %d", frame_num);
            frame_num = 0;
            nowtime = test_time;
        }
        P[1].Tank_Move();
        P[2].Tank_Move();

        

        cleardevice();

        setbkcolor(RGB(0xff, 0xff, 0xff));

        
        outtextxy(1200, 10, str);

        Draw_Map();

        P[1].Draw();
        P[2].Draw();
        check_bullet(test_time);

        if (Tank_Bullet_Test(P[2])) {
            printf("Red Won");
            return ;
        }
        if (Tank_Bullet_Test(P[1])) {
            printf("Blue Won");//TODO: 胜利界面
            return ;
        }

        for (Bullet B : Bulletque)
        {
            PutImage_Alpha(round(B.pos.x - BULLET_D), round(B.pos.y - BULLET_D), &BULTYPE[B.type]);
        }
        draw_background_and_button(0);
        FlushBatchDraw(); // 将缓冲区内容显示在屏幕上

        DWORD end_time = GetTickCount();
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
}

void Widget::run_help()//帮助界面 图形化操作
{
    ExMessage msg;

    BeginBatchDraw(); // 开始批量绘制

    while (getCurrentIndex() == 2)
    {
        DWORD start_time = GetTickCount();
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

        DWORD end_time = GetTickCount();
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

int main()
{
    Widget widget(1280, 720);
    widget.init();
    widget.run_main();
    widget.close();
    return 0;
}