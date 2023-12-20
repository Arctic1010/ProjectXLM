#pragma once
#include<bits/stdc++.h>
// ----- STRUCT PART -----

const int VK_M = 0x4D;
const int VK_E = 0x45;
const int VK_S = 0x53;
const int VK_D = 0x44;
const int VK_F = 0x46;
const int VK_Q = 0x51; //虚拟键码
const int TANK_SIZE = 135;//真实坦克图片大小，包含周围透明部分
const int BULLET_SIZE = 10;
const int BULLET_D = 5;
const int BULLET_MAXNUM = 5;
const int BULLET_DURATION = 6000;
const double PI = 3.1415926535897932384;

const int P1_opkey[5] = { VK_UP,VK_DOWN,VK_LEFT,VK_RIGHT,VK_M };
const int P2_opkey[5] = { VK_E,VK_D,VK_S,VK_F,VK_Q };

struct point
{
	double x, y;
	point()
	{

	}
	point(double _x, double _y)
	{
		x = _x; y = _y;
	}
	point(int _x, int _y)
	{
		x = _x; y = _y;
	}
	void print(char End)
	{
		cout << x << ' ' << y << End;
	}
};

const point TANK_CPOINT[8] = { {15,33},{104,33},{104,60},{119,60},
							   {119,74},{104,74},{104,101},{15,101} };//真实坦克八个顶点的位置

point operator+(const point& a, const point& b)
{
	return point(a.x + b.x, a.y + b.y);
}
point operator-(const point& a, const point& b)
{
	return point(a.x - b.x, a.y - b.y);
}
point operator*(const point& a, const double& b)
{
	return point(a.x * b, a.y * b);
}
//声明子弹类
class Bullet
{
public:
	int id, type;//子弹由标号为id的坦克发出，类型为type
	point pos, aftpos; //子弹中心坐标
	int exist_time;//发出子弹时的时间戳
	bool is_new;//是否刚刚发出，判断碰撞
	double angle;//子弹速度偏角，弧度制
	Bullet();
	Bullet(point p, double ratio, int t_id, int t_type, int t_exist_time, double t_angle);
};

Bullet::Bullet()
{
}

Bullet::Bullet(point p, double ratio, int t_id, int t_type, int t_exist_time, double t_angle)
{
	pos.x = p.x;
	pos.y = p.y;
	id = t_id;
	type = t_type;
	exist_time = t_exist_time;
	is_new = 0;
	angle = t_angle;
}

//声明坦克类
class Tank
{
public:
	IMAGE img;//旋转前的图片，存档用
	IMAGE img_rotate;//旋转后的图片，输出用，防止多次旋转导致图片失真
	point pos, aftpos, collpos;//collpos暂时没用，pos为坦克图片左上角绝对坐标，aftpos为移动后坐标，测试碰撞用
	point critical_point[8];//坦克八个顶点
	double angle, aftangle;//angle为坦克旋转角度，弧度制
	bool is_move[2], is_rotate[2];//坦克是否在移动或旋转
	int shoot_time, id;//id为坦克标号，shoot_time为开始连续射击时的时间戳
	bool is_shooting;//是否在射击
	double tank_size;//坦克在缩放后的大小
	int bullet_num;//坦克当前版面上有的子弹数目
	int op_key[5];//坦克操作虚拟键码
	enum { U, D, L, R, S };
	point centerpos, bullet_gene_pos_D;//这两个变量为相对坦克图片左上角的坐标，并非绝对坐标

	Tank();
	//Tank(wstring img_loc, int t_x, int t_y);
	Tank(wstring img_loc, int t_x, int t_y, double ratio, int t_id, const int* t_opk);
	void Tank_Peek(ExMessage msg);//坦克探测是否移动
	void Tank_Move();
	bool check_line(point start, point dest);
	void Draw();
	void TEST();
};

Tank::Tank()
{
	pos.x = pos.y = 0;
	angle = 0;
	is_move[0] = is_move[1] = 0;
	is_rotate[0] = is_rotate[1] = 0;
	tank_size = TANK_SIZE;
	bullet_num = 0;
	shoot_time = 0;
	for (int i = 0; i < 8; i++)
	{
		critical_point[i] = TANK_CPOINT[i];
	}
	op_key[0] = VK_UP;
	op_key[1] = VK_DOWN;
	op_key[2] = VK_LEFT;
	op_key[3] = VK_RIGHT;
	op_key[4] = VK_M;
}

Tank::Tank(wstring img_loc, int t_x, int t_y, double ratio, int t_id, const int* t_opk)
{
	loadimage(&img, img_loc.c_str(), TANK_SIZE * ratio, TANK_SIZE * ratio);
	img_rotate = img;
	angle = 0;
	pos.x = t_x;
	pos.y = t_y;
	id = t_id;
	is_move[0] = is_move[1] = 0;
	is_rotate[0] = is_rotate[1] = 0;
	tank_size = TANK_SIZE * ratio;
	bullet_num = 0;
	shoot_time = 0;
	is_shooting = 0;
	bullet_gene_pos_D = { ratio * 125, ratio * 67 };
	centerpos = { tank_size / 2, tank_size / 2 };
	for (int i = 0; i < 8; i++)
	{
		critical_point[i] = TANK_CPOINT[i] * ratio;
	}
	for (int i = 0; i <= 4; i++)
	{
		op_key[i] = t_opk[i];
	}
}

// 定义Button类，表示一个按钮
class Button
{
private:
	int x; // 按钮左上角x坐标
	int y; // 按钮左上角y坐标
	int width; // 按钮宽度
	int height; // 按钮高度
	float scale; // 缩放比例，用于实现鼠标悬停效果
	bool isMouseOver; // 表示鼠标是否在按钮上方
	wstring text; // 按钮文本 
	function<void()> onClick; // 点击按钮触发的函数

public:

	Button(int x, int y, int width, int height, const wstring& text, const function<void()>& onClick)
		: x(x), y(y), width(width), height(height), text(text), onClick(onClick), scale(1.0f), isMouseOver(false)
	{
	}

	// 检查鼠标是否在按钮上方
	void checkMouseOver(int mouseX, int mouseY)
	{
		isMouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

		if (isMouseOver) {
			scale = 0.9f; // 鼠标悬停时缩放按钮
		}
		else {
			scale = 1.0f; // 恢复按钮原始大小
		}
	}

	// 检查鼠标点击是否在按钮内，并执行函数
	bool checkClick(int mouseX, int mouseY)
	{
		if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
		{
			onClick(); // 执行按钮点击时的函数
			isMouseOver = false;
			scale = 1.0f;
			return true;
		}
		return false;
	}

	// 绘制按钮
	void draw()
	{
		int scaledWidth = width * scale; // 缩放后的按钮宽度
		int scaledHeight = height * scale; // 缩放后的按钮高度
		int scaledX = x + (width - scaledWidth) / 2; // 缩放后的按钮x坐标
		int scaledY = y + (height - scaledHeight) / 2; // 缩放后的按钮y坐标

		if (isMouseOver)
		{
			setlinecolor(RGB(0, 120, 215)); // 鼠标悬停时按钮边框颜色
			setfillcolor(RGB(229, 241, 251)); // 鼠标悬停时按钮填充颜色

		}
		else
		{
			setlinecolor(RGB(173, 173, 173)); // 按钮边框颜色
			setfillcolor(RGB(225, 225, 225)); // 按钮填充颜色
		}

		fillrectangle(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight); // 绘制按钮
		settextcolor(BLACK); // 设置文本颜色为黑色
		setbkmode(TRANSPARENT); // 设置文本背景透明
		settextstyle(20 * scale, 0, _T("微软雅黑")); // 设置文本大小和字体
		//居中显示按钮文本
		int textX = scaledX + (scaledWidth - textwidth(text.c_str())) / 2; // 计算文本在按钮中央的x坐标
		int textY = scaledY + (scaledHeight - textheight(_T("微软雅黑"))) / 2; // 计算文本在按钮中央的y坐标
		outtextxy(textX, textY, text.c_str()); // 在按钮上绘制文本
	}
};