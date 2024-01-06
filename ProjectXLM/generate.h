#pragma once
#include <graphics.h>
#include <iostream>
#include <map> 
#include <vector> 
#include <set>
#include <cstring> 
#include <string.h> 
#include <cstdlib> 
#include <stdlib.h> 
#include <cstdio> 
#include <algorithm>
#include <cmath>
#include <math.h>
#include <deque>
#include <easyx.h>
#include <conio.h>
using namespace std;

//存储每一条墙壁对应的线条的左上角和右下角
struct Line_Node
{
	int sx, sy, ex, ey;
};

vector<Line_Node> Pre_Draw_Graph(int lx, int ly, int rx, int ry);
/*
 
*/
/*  
     */