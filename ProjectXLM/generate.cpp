#include "generate.h"
const int N = 2e6;
using namespace std;
void chkmin(int& x, int y) {
	if (x > y)  x = y;
}
void chkmax(int& x, int y) {
	if (x < y)  x = y;
}
struct node {
	int lx, ly, rx, ry;
} a[15][11];
int fa[N];
int Get_id(int x, int y) {
	return (x - 1) * 10 + y;
}
int getfa(int x) {
	if (fa[x] != x)  return fa[x] = getfa(fa[x]);
	return fa[x];
}
void merge(int x, int y) {
	int A = getfa(x);
	int B = getfa(y);
	if (A == B)  return;
	fa[A] = B;
}
bool check_fa(int x, int y) {
	int A = getfa(x);
	int B = getfa(y);
	if (A == B)  return true;
	return false;
}
int dx[5] = { 0, 0, 1, -1 };
int dy[5] = { 1, -1, 0, 0 };
map < pair<pair<int, int>, pair<int, int> >, int> mp;
map <pair<int, int>, int> mp2;
int vib[15][11][5];

vector<Line_Node>map_line;

void Draw(int lx, int ly, int rx, int ry) {
	map_line.push_back({ lx,ly,rx,ry });
}
vector<Line_Node> Pre_Draw_Graph(int lx, int ly, int rx, int ry) {
	map_line.clear();
	Draw(20, 20, 1005, 25);
	Draw(20, 20, 25, 720);
	Draw(20, 715, 1005, 720);
	Draw(1000, 20, 1005, 720);
	memset(vib, 0, sizeof(vib));
	int num = 140;
	mp.clear();
	mp2.clear();
	for (int i = 1; i <= 14; i++) {
		for (int j = 1; j <= 10; j++) {
			int X1 = lx + (i - 1) * 70;
			int Y1 = ly + (j - 1) * 70;
			chkmin(X1, 1000);
			chkmin(Y1, 715);
			a[i][j].lx = X1, a[i][j].ly = Y1;
			int X2 = X1 + 70 - 1;
			int Y2 = Y1 + 70 - 1;
			chkmin(X2, 1000);
			chkmin(Y2, 715);
			a[i][j].rx = X2, a[i][j].ry = Y2;
			fa[Get_id(i, j)] = Get_id(i, j);
		}
	}
	vector < pair<int, int> > node;
	node.push_back({ 1, 1 });
	int cnt = 0;
	mp2[{1, 1}] = 1;
	srand(unsigned(time(0)));
	while (true) {
		if (cnt == num - 1)  break;
		vector <  pair< pair<int, int>, pair<int, int> > > Ex;
		for (auto u : node) {
			int x = u.first;
			int y = u.second;
			for (int i = 0; i < 4; i++) {
				int xx = x + dx[i];
				int yy = y + dy[i];
				if (xx < 1 || yy < 1 || xx > 14 || yy > 10)  continue;
				if (mp2[{xx, yy}])  continue;
				if (check_fa(Get_id(x, y), Get_id(xx, yy) == true))  continue;
				Ex.push_back({ {x, y}, {xx, yy} });
			}
		}
		int selection = rand() % Ex.size();
		mp[{Ex[selection].first, Ex[selection].second}] = true;
		mp[{Ex[selection].second, Ex[selection].first}] = true;
		++cnt;
		merge(Get_id(Ex[selection].first.first, Ex[selection].first.second), Get_id(Ex[selection].second.first, Ex[selection].second.second));
		node.push_back(Ex[selection].second);
		mp2[Ex[selection].second] = true;
	}
	for (int i = 1; i <= 14; i++) {
		for (int j = 1; j <= 10; j++) {
			int x = i, y = j;
			for (int k = 0; k < 4; k++) {
				int xx = i + dx[k];
				int yy = j + dy[k];
				if (xx < 1 || yy < 1 || xx > 14 || yy > 10)  continue;
				if (mp[{{x, y}, { xx, yy }}] == false) {
					if (rand() % 50 <= 20) {
						vib[i][j][k] = 1;
					}
				}
			}
		}
	}
	for (int i = 1; i <= 14; i++) {
		for (int j = 1; j <= 10; j++) {
			int x = i, y = j;
			for (int k = 0; k < 4; k++) {
				if (vib[i][j][k])  continue;
				// ÏÂ£¬ÉÏ£¬×ó£¬ÓÒ
				int xx = i + dx[k];
				int yy = j + dy[k];
				if (xx < 1 || yy < 1 || xx > 14 || yy > 10)  continue;
				if (mp[{{x, y}, { xx, yy }}] == false) {
					if (k == 2) {
						int X1 = a[x][y].rx - 4;
						int Y1 = a[x][y].ly;
						int X2 = a[xx][yy].lx + 4;
						int Y2 = a[xx][yy].ry;
						chkmin(X2, 1000);
						chkmin(Y2, 715);
						Draw(X1, Y1, X2, Y2);
					}
					if (k == 0) {
						int X1 = a[x][y].lx;
						int Y1 = a[x][y].ry - 4;
						int X2 = a[xx][yy].rx;
						int Y2 = a[xx][yy].ly + 4;
						chkmin(X2, 1000);
						chkmin(Y2, 715);
						Draw(X1, Y1, X2, Y2);
					}
					if (k == 1) {
						int X1 = a[xx][yy].lx;
						int Y1 = a[xx][yy].ry - 4;
						int X2 = a[x][y].rx;
						int Y2 = a[x][y].ly + 4;
						chkmin(X2, 1000);
						chkmin(Y2, 715);
						Draw(X1, Y1, X2, Y2);
					}
					if (k == 3) {
						int X1 = a[xx][yy].rx - 4;
						int Y1 = a[xx][yy].ly;
						int X2 = a[x][y].lx + 4;
						int Y2 = a[x][y].ry;
						chkmin(X2, 1000);
						chkmin(Y2, 715);
						Draw(X1, Y1, X2, Y2);
					}
				}
			}
		}
	}
	return map_line;
}
