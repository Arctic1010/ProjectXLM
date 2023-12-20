#pragma once
#include<bits/stdc++.h>
// ----- STRUCT PART -----

const int VK_M = 0x4D;
const int VK_E = 0x45;
const int VK_S = 0x53;
const int VK_D = 0x44;
const int VK_F = 0x46;
const int VK_Q = 0x51; //�������
const int TANK_SIZE = 135;//��ʵ̹��ͼƬ��С��������Χ͸������
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
							   {119,74},{104,74},{104,101},{15,101} };//��ʵ̹�˰˸������λ��

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
//�����ӵ���
class Bullet
{
public:
	int id, type;//�ӵ��ɱ��Ϊid��̹�˷���������Ϊtype
	point pos, aftpos; //�ӵ���������
	int exist_time;//�����ӵ�ʱ��ʱ���
	bool is_new;//�Ƿ�ոշ������ж���ײ
	double angle;//�ӵ��ٶ�ƫ�ǣ�������
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

//����̹����
class Tank
{
public:
	IMAGE img;//��תǰ��ͼƬ���浵��
	IMAGE img_rotate;//��ת���ͼƬ������ã���ֹ�����ת����ͼƬʧ��
	point pos, aftpos, collpos;//collpos��ʱû�ã�posΪ̹��ͼƬ���ϽǾ������꣬aftposΪ�ƶ������꣬������ײ��
	point critical_point[8];//̹�˰˸�����
	double angle, aftangle;//angleΪ̹����ת�Ƕȣ�������
	bool is_move[2], is_rotate[2];//̹���Ƿ����ƶ�����ת
	int shoot_time, id;//idΪ̹�˱�ţ�shoot_timeΪ��ʼ�������ʱ��ʱ���
	bool is_shooting;//�Ƿ������
	double tank_size;//̹�������ź�Ĵ�С
	int bullet_num;//̹�˵�ǰ�������е��ӵ���Ŀ
	int op_key[5];//̹�˲����������
	enum { U, D, L, R, S };
	point centerpos, bullet_gene_pos_D;//����������Ϊ���̹��ͼƬ���Ͻǵ����꣬���Ǿ�������

	Tank();
	//Tank(wstring img_loc, int t_x, int t_y);
	Tank(wstring img_loc, int t_x, int t_y, double ratio, int t_id, const int* t_opk);
	void Tank_Peek(ExMessage msg);//̹��̽���Ƿ��ƶ�
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

// ����Button�࣬��ʾһ����ť
class Button
{
private:
	int x; // ��ť���Ͻ�x����
	int y; // ��ť���Ͻ�y����
	int width; // ��ť���
	int height; // ��ť�߶�
	float scale; // ���ű���������ʵ�������ͣЧ��
	bool isMouseOver; // ��ʾ����Ƿ��ڰ�ť�Ϸ�
	wstring text; // ��ť�ı� 
	function<void()> onClick; // �����ť�����ĺ���

public:

	Button(int x, int y, int width, int height, const wstring& text, const function<void()>& onClick)
		: x(x), y(y), width(width), height(height), text(text), onClick(onClick), scale(1.0f), isMouseOver(false)
	{
	}

	// �������Ƿ��ڰ�ť�Ϸ�
	void checkMouseOver(int mouseX, int mouseY)
	{
		isMouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

		if (isMouseOver) {
			scale = 0.9f; // �����ͣʱ���Ű�ť
		}
		else {
			scale = 1.0f; // �ָ���ťԭʼ��С
		}
	}

	// ���������Ƿ��ڰ�ť�ڣ���ִ�к���
	bool checkClick(int mouseX, int mouseY)
	{
		if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
		{
			onClick(); // ִ�а�ť���ʱ�ĺ���
			isMouseOver = false;
			scale = 1.0f;
			return true;
		}
		return false;
	}

	// ���ư�ť
	void draw()
	{
		int scaledWidth = width * scale; // ���ź�İ�ť���
		int scaledHeight = height * scale; // ���ź�İ�ť�߶�
		int scaledX = x + (width - scaledWidth) / 2; // ���ź�İ�ťx����
		int scaledY = y + (height - scaledHeight) / 2; // ���ź�İ�ťy����

		if (isMouseOver)
		{
			setlinecolor(RGB(0, 120, 215)); // �����ͣʱ��ť�߿���ɫ
			setfillcolor(RGB(229, 241, 251)); // �����ͣʱ��ť�����ɫ

		}
		else
		{
			setlinecolor(RGB(173, 173, 173)); // ��ť�߿���ɫ
			setfillcolor(RGB(225, 225, 225)); // ��ť�����ɫ
		}

		fillrectangle(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight); // ���ư�ť
		settextcolor(BLACK); // �����ı���ɫΪ��ɫ
		setbkmode(TRANSPARENT); // �����ı�����͸��
		settextstyle(20 * scale, 0, _T("΢���ź�")); // �����ı���С������
		//������ʾ��ť�ı�
		int textX = scaledX + (scaledWidth - textwidth(text.c_str())) / 2; // �����ı��ڰ�ť�����x����
		int textY = scaledY + (scaledHeight - textheight(_T("΢���ź�"))) / 2; // �����ı��ڰ�ť�����y����
		outtextxy(textX, textY, text.c_str()); // �ڰ�ť�ϻ����ı�
	}
};