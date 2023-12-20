#include<bits/stdc++.h>
#include<graphics.h>
#include"generate.h"
#include"objects.h"



using namespace std;

// ----- CONST VARIABLES -----














void Tank::TEST()
{

}








void game_main()
{
	loadimage(&BULTYPE[0], L"material/bullet1.png", BULLET_SIZE, BULLET_SIZE);
	loadimage(&Background, L"material/background.jpg", 1280, 720);

	bool running = true;
	ExMessage msg;

	//loadimage(&temp, L"material/bullet2.png");
	setbkcolor(RGB(0xff, 0xff, 0xff));
	Line_map = Pre_Draw_Graph(25, 25, 1000, 715);

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
			
		}
		
		// ----- MOVE PLAYER -----

		

		

		// ----- BULLET PROCESS -----

		if (test_time - nowtime >= 1000)
		{
			// ----- TEST AREA -----
			P[1].TEST();
			P[2].TEST();

			nowtime = test_time;
		}

		// ----- PROCESS OF RENDER -----
		

		

		

		//for (int i = 0; i <= 1280; i += 50)
		//	for (int j = 0; j <= 720; j += 50)
		//		PutImage_Alpha(i-5, j-5, &temp);

		

		

		FlushBatchDraw();

		

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;
		if (delta_time < 1000 / FPS)
		{
			Sleep(1000 / FPS - delta_time);
		}
	}
	EndBatchDraw();

	return ;
}