/*
		bool can_move = check_line(new_pos_x + ratio * 14, new_pos_y + ratio * 32, new_pos_x + ratio * 103, new_pos_y + ratio * 32, 1, 0, test_angle) &&
			check_line(new_pos_x + ratio * 14, new_pos_y + ratio * 100, new_pos_x + ratio * 103, new_pos_y + ratio * 100, 1, 0, test_angle) &&
			check_line(new_pos_x + ratio * 103, new_pos_y + ratio * 59, new_pos_x + ratio * 118, new_pos_y + ratio * 59, 1, 0, test_angle) &&
			check_line(new_pos_x + ratio * 103, new_pos_y + ratio * 73, new_pos_x + ratio * 118, new_pos_y + ratio * 73, 1, 0, test_angle) &&
			check_line(new_pos_x + ratio * 14, new_pos_y + ratio * 32, new_pos_x + ratio * 14, new_pos_y + ratio * 100, 0, 1, test_angle) &&
			check_line(new_pos_x + ratio * 103, new_pos_y + ratio * 32, new_pos_x + ratio * 103, new_pos_y + ratio * 59, 0, 1, test_angle) &&
			check_line(new_pos_x + ratio * 103, new_pos_y + ratio * 73, new_pos_x + ratio * 103, new_pos_y + ratio * 100, 0, 1, test_angle) &&
			check_line(new_pos_x + ratio * 118, new_pos_y + ratio * 59, new_pos_x + ratio * 118, new_pos_y + ratio * 73, 0, 1, test_angle);

		*/

		/*
				check_line_t(new_pos_x + ratio * 15, new_pos_y + ratio * 33, new_pos_x + ratio * 104, new_pos_y + ratio * 33, 1, 0, test_angle);
				check_line_t(new_pos_x + ratio * 15, new_pos_y + ratio * 101, new_pos_x + ratio * 104, new_pos_y + ratio * 101, 1, 0, test_angle);
				check_line_t(new_pos_x + ratio * 104, new_pos_y + ratio * 60, new_pos_x + ratio * 119, new_pos_y + ratio * 60, 1, 0, test_angle);
				check_line_t(new_pos_x + ratio * 104, new_pos_y + ratio * 74, new_pos_x + ratio * 119, new_pos_y + ratio * 74, 1, 0, test_angle);
				check_line_t(new_pos_x + ratio * 15, new_pos_y + ratio * 33, new_pos_x + ratio * 15, new_pos_y + ratio * 101, 0, 1, test_angle);
				check_line_t(new_pos_x + ratio * 104, new_pos_y + ratio * 33, new_pos_x + ratio * 104, new_pos_y + ratio * 60, 0, 1, test_angle);
				check_line_t(new_pos_x + ratio * 104, new_pos_y + ratio * 74, new_pos_x + ratio * 104, new_pos_y + ratio * 101, 0, 1, test_angle);
				check_line_t(new_pos_x + ratio * 119, new_pos_y + ratio * 60, new_pos_x + ratio * 119, new_pos_y + ratio * 74, 0, 1, test_angle);
				*/
				//IMAGE temp;
				/*
				void check_line_t(int start_x, int start_y, int dest_x, int dest_y, int delta_x, int delta_y, double radian)
				{
					radian = -radian;
					double d_x = (double)Player.tank_size / 2 + new_pos_x;
					double d_y = (double)Player.tank_size / 2 + new_pos_y;
					for (int x = start_x, y = start_y;
						x <= dest_x && y <= dest_y; x += delta_x, y += delta_y)
					{
						double t_x = cos(radian) * ((double)x - d_x) - sin(radian) * ((double)y - d_y) + d_x;
						double t_y = sin(radian) * ((double)x - d_x) + cos(radian) * ((double)y - d_y) + d_y;

						PutImage_Alpha(t_x - 5, t_y - 5, &temp);

						if (getpixel(floor(t_x), floor(t_y)) == RGB(128, 128, 128))||
							getpixel(floor(t_x), ceil(t_y)) == RGB(128, 128, 128) ||
							getpixel(ceil(t_x), floor(t_y)) == RGB(128, 128, 128) ||
							getpixel(ceil(t_x), ceil(t_y)) == RGB(128, 128, 128))
						{
							coll_t_x = t_x;
							coll_t_y = t_y;

							//cout << "DEBUG: At pixel " << t_x << ' ' << t_y << " has a collision. Tank at" << ' ' << "\n";
							//return false;
						}
					}
					//return true;
				}
				*/
/*
Bullet::Bullet()
{
	pos_x = pos_y = 0;
}

Bullet::Bullet(wstring img_loc, int t_x, int t_y, int t_id)
{
	loadimage(&img, img_loc.c_str());
	pos_x = t_x;
	pos_y = t_y;
	id = t_id;
}

Bullet::Bullet(wstring img_loc, int t_x, int t_y, double ratio, int t_id)
{
	loadimage(&img, img_loc.c_str(), TANK_SIZE * ratio, TANK_SIZE * ratio);
	pos_x = t_x;
	pos_y = t_y;
	id = t_id;
}
*/
/*||
			getpixel(floor(t_x), ceil(t_y)) == RGB(128, 128, 128) ||
			getpixel(ceil(t_x), floor(t_y)) == RGB(128, 128, 128) ||
			getpixel(ceil(t_x), ceil(t_y)) == RGB(128, 128, 128))*/
/*
Bullet::Bullet(int t_x, int t_y, int t_id)
{
	pos.x = t_x;
	pos.y = t_y;
	id = t_id;
	exist_time = 0;
	is_new = 0;
}

Bullet::Bullet(int t_x, int t_y, double ratio, int t_id, int t_type, int t_exist_time)
{
	pos.x = t_x;
	pos.y = t_y;
	id = t_id;
	type = t_type;
	exist_time = t_exist_time;
	is_new = 0;
}
*/
/*
Tank::Tank(wstring img_loc, int t_x, int t_y)
{
	loadimage(&img, img_loc.c_str());
	img_rotate = img;
	angle = 0;
	pos.x = t_x;
	pos.y = t_y;
	is_move[0] = is_move[1] = 0;
	is_rotate[0] = is_rotate[1] = 0;
	tank_size = TANK_SIZE;
	bullet_num = 0;
	shoot_time = 0;
	for (int i = 0; i < 8; i++)
	{
		critical_point[i] = TANK_CPOINT[i];
	}
}
*/
/*
#include<bits/stdc++.h>
#include<graphics.h>
#include"generate.h"

using namespace std;

#pragma once


// ----- CONST VARIABLES -----

const int PLAYER_SPEED = 5;
const int BULLET_SPEED = 7;
const double PI = 3.1415926;
const double ROTATE_SPEED = PI / 36;
const double eps = 1e-5;
const int TANK_SIZE = 135;
const int BULLET_SIZE = 10;
const int SHOOT_CD = 100;
const int BULLET_D = 5; // For center-oriented coordinations
const int VK_M = 0x4D;
const int BULLET_MAXNUM = 500;
const int BULLET_DURATION = 5000;
enum { FORWARD, BACK };
enum { LEFT, RIGHT };

// ----- STRUCT PART -----

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
};

#define Vec point

point point_add(const point& a, point& b)
{
	return point(a.x + b.x, a.y + b.y);
}


const point TANK_CPOINT[8] = { {15,33},{104,33},{104,60},{119,60},
							   {119,74},{104,74},{104,101},{15,101} };

class Tank
{
public:
	IMAGE img;
	IMAGE img_rotate;
	point pos;
	point critical_point[8];
	double angle;
	bool is_move[2], is_rotate[2];
	int shoot_time;
	bool is_shooting;
	double tank_size;
	int bullet_num;

	Tank();
	//Tank(wstring img_loc, int t_x, int t_y);
	Tank(wstring img_loc, int t_x, int t_y, double ratio);

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
}

Tank::Tank(wstring img_loc, int t_x, int t_y, double ratio)
{
	loadimage(&img, img_loc.c_str(), TANK_SIZE * ratio, TANK_SIZE * ratio);
	img_rotate = img;
	angle = 0;
	pos.x = t_x;
	pos.y = t_y;
	is_move[0] = is_move[1] = 0;
	is_rotate[0] = is_rotate[1] = 0;
	tank_size = TANK_SIZE * ratio;
	bullet_num = 0;
	shoot_time = 0;
	is_shooting = 0;
	for (int i = 0; i < 8; i++)
	{
		critical_point[i] = point((int)(TANK_CPOINT[i].x * ratio),
			(int)(TANK_CPOINT[i].y * ratio));
	}
}

IMAGE BULTYPE[2];

class Bullet
{
public:
	int id, type;
	point pos; // Based on center
	int exist_time;
	bool is_new;
	double angle;
	Bullet();
	Bullet(point p, double ratio, int t_id, int t_type, int t_exist_time,double t_angle);
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

using namespace std;
const int FPS = 30;

#pragma comment(lib,"MSIMG32.LIB")

inline void PutImage_Alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

deque<Bullet>Bulletque,T_que;
vector<Line_Node>Line_map;

double ratio = 0.4;

Tank Player(L"material/tank1.png", 500, 500, ratio);
point aftpos, collpos, B_aftpos, B_collpos;

point TANK_CENTER = { ratio * TANK_SIZE / 2, ratio * TANK_SIZE / 2 };
point BULLET_GENE = { ratio * 120, ratio * 62 };
point BULLET_GENE_D = { ratio * 125, ratio * 67 };

bool check_line(int start_x,int start_y,int dest_x,int dest_y,int delta_x,int delta_y,double radian)
{
	radian = -radian;
	point d = { (double)Player.tank_size / 2 + aftpos.x,
			  (double)Player.tank_size / 2 + aftpos.y };

	for (int x = start_x, y = start_y; x <= dest_x && y <= dest_y; x += delta_x, y += delta_y)
	{
		point t = { cos(radian) * ((double)x - d.x) - sin(radian) * ((double)y - d.y) + d.x,
				   sin(radian) * ((double)x - d.x) + cos(radian) * ((double)y - d.y) + d.y };

		if (getpixel(floor(t.x), floor(t.y)) == RGB(128, 128, 128))
		{
			collpos.x = t.x;
			collpos.y = t.y;
			//cout << "DEBUG: At pixel " << t_x << ' ' << t_y << " has a collision. Tank at"<<' '<< "\n";
			return false;
		}
	}
	return true;
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

IMAGE temp;

bool check_bullet_wall(Bullet& B)
{
	double radian = -B.angle;

	Vec v = { B_aftpos.x - B.pos.x,B_aftpos.y - B.pos.y };
	double t = sqrt(v.x * v.x + v.y * v.y);



	for (double x = B.pos.x, y = B.pos.y; fabs(x-B_aftpos.x)>=0.1 || fabs(y-B_aftpos.y)>=0.1; x += v.x / t, y += v.y / t)
	{
		//cout << x << ' ' << y << '\n';
		if (getpixel(floor(x), floor(y)) == RGB(128, 128, 128))
		{
			bool hvflag = check_hor_ver(point(x, y));
			v = { B_aftpos.x - x,B_aftpos.y - y };
			if (hvflag)
			{
				cout << "collision horizontally.\n";
				v.y = - v.y;
				B.angle = - B.angle;
			}
			else
			{
				cout << "collision vertically.\n";
				v.x = - v.x;
				B.angle = PI - B.angle;
			}
			B_aftpos = { x + v.x,y + v.y };
			return 1;
		}
	}
	return 0;
}


point cal_rev_point(point p, point O, double radian)
{
	radian = -radian;
	return point(cos(radian) * ((double)p.x - O.x) - sin(radian) * ((double)p.y - O.y) + O.x,
				 sin(radian) * ((double)p.x - O.x) + cos(radian) * ((double)p.y - O.y) + O.y);
}


void Draw2()
{
	setlinecolor(RGB(128, 128, 128));
	setfillcolor(RGB(128, 128, 128));
	for (Line_Node p : Line_map)
	{
		solidrectangle(p.sx, p.sy, p.ex, p.ey);
	}
}

signed main()
{
	initgraph(1280, 720);

	loadimage(&BULTYPE[0], L"material/bullet1.png", BULLET_SIZE, BULLET_SIZE);

	bool running = true;
	ExMessage msg;

	loadimage(&temp, L"material/bullet2.png");
	setbkcolor(RGB(0xff, 0xff, 0xff));
	Line_map = Pre_Draw_Graph(25, 25, 1000, 715);
	for (auto i : Line_map)
	{
		cout << i.sx << ' ' << i.sy << ' ' << i.ex << ' ' << i.ey << '\n';
	}

	BeginBatchDraw();

	// ----- TEST VARIABLE -----

	DWORD nowtime = GetTickCount();

	while (running)
	{
		DWORD start_time = GetTickCount();

		// ----- PEEK MESSAGE -----

		while (peekmessage(&msg))
		{
			// ----- PEEK KEYBOARD -----

			if (msg.message == WM_KEYDOWN)
			{
				switch (msg.vkcode)
				{
				case VK_UP:
					Player.is_move[FORWARD] = 1;
					break;
				case VK_DOWN:
					Player.is_move[BACK] = 1;
					break;
				case VK_LEFT:
					Player.is_rotate[LEFT] = 1;
					break;
				case VK_RIGHT:
					Player.is_rotate[RIGHT] = 1;
					break;
				case VK_M:
					Player.is_shooting = 1;
					break;
				}
			}
			if (msg.message == WM_KEYUP)
			{
				switch (msg.vkcode)
				{
				case VK_UP:
					Player.is_move[FORWARD] = 0;
					break;
				case VK_DOWN:
					Player.is_move[BACK] = 0;
					break;
				case VK_LEFT:
					Player.is_rotate[LEFT] = 0;
					break;
				case VK_RIGHT:
					Player.is_rotate[RIGHT] = 0;
					break;
				case VK_M:
					Player.is_shooting = 0;
					Player.shoot_time = 0;
					break;
				}
			}
		}

		// ----- MOVE PLAYER -----

		aftpos.x = Player.pos.x, aftpos.y = Player.pos.y;
		double test_angle = Player.angle;
		if (Player.is_move[FORWARD])
		{
			aftpos.x = Player.pos.x + cos(Player.angle) * PLAYER_SPEED;
			aftpos.y = Player.pos.y - sin(Player.angle) * PLAYER_SPEED;
		}
		if (Player.is_move[BACK])
		{
			aftpos.x = Player.pos.x - cos(Player.angle) * PLAYER_SPEED;
			aftpos.y = Player.pos.y + sin(Player.angle) * PLAYER_SPEED;
		}
		if (Player.is_rotate[LEFT])
		{
			test_angle += ROTATE_SPEED;
			if (test_angle > PI) test_angle -= PI * 2;
		}
		if (Player.is_rotate[RIGHT])
		{
			test_angle -= ROTATE_SPEED;
			if (test_angle < -PI) test_angle += PI * 2;
		}
		if (Player.is_shooting)
		{
			if (Player.bullet_num < BULLET_MAXNUM)
			{
				if (!Player.shoot_time || GetTickCount() - Player.shoot_time >= SHOOT_CD)
				{
					Player.bullet_num++;
					Player.shoot_time = GetTickCount();
					Bulletque.push_back(Bullet(cal_rev_point(point_add(BULLET_GENE_D, Player.pos),
						point_add(TANK_CENTER, Player.pos), Player.angle),
						1, 1, 0, GetTickCount(), Player.angle));
					//cout << "cood: " << TANK_CENTER.x << ' ' << TANK_CENTER.y << ' ' << Player.pos.x << ' ' << Player.pos.y << '\n';
					//cout << "cood: " << Bulletque.back().pos.x << ' ' << Bulletque.back().pos.y << '\n';
				}
			}
		}

		// ----- O'CLOCK ADJUST -----

		if (fabs(test_angle) <= eps) test_angle = 0;
		if (fabs(test_angle - PI) <= eps) test_angle = PI;
		if (fabs(test_angle - PI / 2) <= eps) test_angle = PI / 2;
		if (fabs(test_angle + PI / 2) <= eps) test_angle = -PI / 2;

		DWORD test_time = GetTickCount();

		// ----- BULLET PROCESS -----



		// ----- PLAYER-WALL COLLISION CHECK -----

		bool can_move = check_line(aftpos.x + ratio * 15, aftpos.y + ratio * 33, aftpos.x + ratio * 104, aftpos.y + ratio * 33, 1, 0, test_angle) &&
			check_line(aftpos.x + ratio * 15, aftpos.y + ratio * 101, aftpos.x + ratio * 104, aftpos.y + ratio * 101, 1, 0, test_angle) &&
			check_line(aftpos.x + ratio * 104, aftpos.y + ratio * 60, aftpos.x + ratio * 119, aftpos.y + ratio * 60, 1, 0, test_angle) &&
			check_line(aftpos.x + ratio * 104, aftpos.y + ratio * 74, aftpos.x + ratio * 119, aftpos.y + ratio * 74, 1, 0, test_angle) &&
			check_line(aftpos.x + ratio * 15, aftpos.y + ratio * 33, aftpos.x + ratio * 15, aftpos.y + ratio * 101, 0, 1, test_angle) &&
			check_line(aftpos.x + ratio * 104, aftpos.y + ratio * 33, aftpos.x + ratio * 104, aftpos.y + ratio * 60, 0, 1, test_angle) &&
			check_line(aftpos.x + ratio * 104, aftpos.y + ratio * 74, aftpos.x + ratio * 104, aftpos.y + ratio * 101, 0, 1, test_angle) &&
			check_line(aftpos.x + ratio * 119, aftpos.y + ratio * 60, aftpos.x + ratio * 119, aftpos.y + ratio * 74, 0, 1, test_angle);

		if (can_move)
		{
			Player.pos.x = aftpos.x;
			Player.pos.y = aftpos.y;
			Player.angle = test_angle;
		}


		if (test_time - nowtime >= 1000)
		{
			// ----- TEST AREA -----
			cout << "Now Bullet Queue\n";
			for (Bullet B : Bulletque)
			{
				cout << B.pos.x << ' ' << B.pos.y << '\n';
			}
			//cout << "now angle = " << Player.angle << '\n';
			nowtime = test_time;
		}

		// ----- PROCESS OF RENDER -----
		cleardevice();

		setbkcolor(RGB(0xff, 0xff, 0xff));

		Draw2();

		//for (int i = 0; i <= 1280; i += 50)
		//	for (int j = 0; j <= 720; j += 50)
		//		PutImage_Alpha(i-5, j-5, &temp);

		rotateimage(&Player.img_rotate, &Player.img, Player.angle, TRANSPARENT, false, false);
		PutImage_Alpha(round(Player.pos.x), round(Player.pos.y), &Player.img_rotate);

		for (auto i = Bulletque.begin(); i != Bulletque.end(); i++)
		{
			Bullet B = (*i);
			if (test_time - B.exist_time >= BULLET_DURATION)
			{
				Player.bullet_num--;
				continue;
			}
			B_aftpos = B.pos;
			if (!B.is_new) B.is_new = 1;
			else
			{
				B_aftpos.x = B.pos.x + cos(B.angle) * BULLET_SPEED;
				B_aftpos.y = B.pos.y - sin(B.angle) * BULLET_SPEED;
			}

			check_bullet_wall(B);

			B.pos.x = B_aftpos.x;
			B.pos.y = B_aftpos.y;
			T_que.push_back(B);
		}

		Bulletque = T_que;
		T_que.clear();

		for (Bullet B : Bulletque)
		{
			PutImage_Alpha(round(B.pos.x-BULLET_D), round(B.pos.y-BULLET_D), &BULTYPE[B.type]);
			//T_check_bullet_wall(B);
		}

		FlushBatchDraw();

		// ----- ADJUST RENDER FPS -----

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;
		if (delta_time < 1000 / FPS)
		{
			Sleep(1000 / FPS - delta_time);
		}
	}
	EndBatchDraw();

	return 0;
}
*/
/*


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
*/
/*
	bool cross_left =
		rct.left > B.aftpos.x
		&& fabs(B.aftpos.x - rct.left) <= B.radius;
	bool cross_right =
		B.aftpos.x > rct.right
		&& fabs(B.aftpos.x - rct.right) <= B.radius;
	bool cross_top =
		rct.top > B.aftpos.y
		&& fabs(B.aftpos.y - rct.top) <= B.radius;
	bool cross_bottom =
		B.aftpos.y > rct.bottom
		&& fabs(B.aftpos.y - rct.bottom) <= B.radius;
		if (cross_left) cout << "Cross left" << '\n';
	if (cross_right) cout << "Cross right" << '\n';
	if (cross_top) cout << "Cross top" << '\n';
	if (cross_bottom) cout << "Cross bottom" << '\n';

*/