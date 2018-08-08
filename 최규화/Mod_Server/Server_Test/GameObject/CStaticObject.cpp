#include "../stdafx.h"

CStaticObject::CStaticObject()
{
}

//큐브 랜덤 색상 텍스쳐
//ObjData.CustomData1.w = rand() % 400 + 100; 

void CStaticObject::MakeStaticObjectBasicData()
{
	//여기다 물체 배치에 대한 상수들 넣는다.
	float BigWall_X1 = 300 * sinf(0.4f * MMPE_PI);
	float BigWall_Z1 = (600 * cosf(0.4f * MMPE_PI) + 600) / 2;

	float BigWall_X2 = ((600 * sinf(0.4f * MMPE_PI)) + (600 * sinf(0.8f * MMPE_PI))) / 2;
	float BigWall_Z2 = ((600 * cosf(0.4f * MMPE_PI)) + (-600 * cosf(0.2f * MMPE_PI))) / 2;

	float BigWall_Z3 = -600 * cosf(0.2f * MMPE_PI);

	float BigWall_Rad1 = MMPE_PI / 5; //36degree

	g_objectData =
	{
		//============2층=============
		//1 - 8 - 4 = 13
		{ 0,{ {   0.f,  91.f,   0.f, 0.f },				{ 0 },{ INSTALLED_OBJECT_TYPE::SecondFloor} } },

		{ 1,{ {  30.f, 100.f,  90.f, 0.f },{ MMPE_PI * 0.25 },{ INSTALLED_OBJECT_TYPE::SmallWall  } } },
		{ 2,{ {  90.f, 100.f,  30.f, 0.f },{ MMPE_PI * 0.25 },{ INSTALLED_OBJECT_TYPE::SmallWall  } } },
		{ 3,{ { -30.f, 100.f,  90.f, 0.f },{ MMPE_PI * 0.25 },{ INSTALLED_OBJECT_TYPE::SmallWall  } } },
		{ 4,{ { -90.f, 100.f,  30.f, 0.f },{ MMPE_PI * 0.25 },{ INSTALLED_OBJECT_TYPE::SmallWall  } } },
		{ 5,{ {  30.f, 100.f, -90.f, 0.f },{ MMPE_PI * 0.25 },{ INSTALLED_OBJECT_TYPE::SmallWall  } } },
		{ 6,{ {  90.f, 100.f, -30.f, 0.f },{ MMPE_PI * 0.25 },{ INSTALLED_OBJECT_TYPE::SmallWall  } } },
		{ 7,{ { -30.f, 100.f, -90.f, 0.f },{ MMPE_PI * 0.25 },{ INSTALLED_OBJECT_TYPE::SmallWall  } } },
		{ 8,{ { -90.f, 100.f, -30.f, 0.f },{ MMPE_PI * 0.25 },{ INSTALLED_OBJECT_TYPE::SmallWall  } } },

		{ 9, { { 50.f, 101.f, 50.f, 0.f   },{ 0 },{ INSTALLED_OBJECT_TYPE::Box } } },
		{ 10,{ { 50.f, 101.f, -50.f, 0.f  },{ 0 },{ INSTALLED_OBJECT_TYPE::Box } } },
		{ 11,{ { -50.f, 111.f, 50.f, 0.f  },{ 0 },{ INSTALLED_OBJECT_TYPE::Box } } },
		{ 12,{ { -50.f, 111.f, -50.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box } } },

		//============1층=============
		//MoveCube - 4 = 4
		{ 13,{ { 0.f, 25.f, 145.f, 0.f   },{ 50.f },{ INSTALLED_OBJECT_TYPE::MoveCube } } },
		{ 14,{ { 0.f, 52.f, -150.f, 0.f  },{ 50.f },{ INSTALLED_OBJECT_TYPE::MoveCube } } },
		{ 15,{ { 145.f, 34.f, 0.f, 0.f   },{ 50.f },{ INSTALLED_OBJECT_TYPE::MoveCube } } },
		{ 16,{ { -145.f, 96.f, 0.f, 0.f  },{ 50.f },{ INSTALLED_OBJECT_TYPE::MoveCube } } },

		//Cube - 17 - 4 - 3 - 11 = 35
		{ 17,{ { 40.f, 18.f, -70.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 18,{ { 68.f, 70.f, -93.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 19,{ { 35.f, 50.f, -70.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 20,{ { 128.f, 38.f, 27.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 21,{ { 102.f, 69.f, 81.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 22,{ { 58.f, 39.f,  78.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 23,{ { 65.f, 35.f,-110.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 24,{ { 92.f, 61.f,  60.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 25,{ { 103.f, 26.f, 35.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 26,{ { 65.f, 17.f, -65.f ,0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } }, //10
		{ 27,{ { 82.f, 11.f, 34.f, 0.f  },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 28,{ { 72.f, 44.f, 41.f, 0.f  },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 29,{ {-32.f, 39.f, 51.f, 0.f  },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },

		{ 30,{ {  0.f, 24.f,  34.f, 0.f   },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 31,{ {  0.f, 38.f, -71.f ,0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 32,{ {  0.f, 15.f,  12.f, 0.f  },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 33,{ { 13.f, 33.f, -42.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },  //17

		{ 34,{ { -76.f, 53.f, 8.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 35,{ { -40.f, 10.f, -15.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 36,{ { 38.f, 48.f, -126.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },   //20
		{ 37,{ { -41.f, 50.f, -94.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 38,{ { -45.f, 30.f, -60.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 39,{ { -65.f, 40.f, -102,0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 40,{ { -69.f, 54.f, 61.f ,0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 41,{ { -85.f, 42.f, -45.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 42,{ { -106.f, 40.f, 42.f ,0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 43,{ { -104.f, 64.f, -62.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 44,{ { -87.f, 70.f,  82.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },    //28

		//door
		//10 - 10 - 10 - 10 - 10 = 50
		//좌상 ~반시계방향 ~우상
		{ 45,{ { -170.f,  5.f, 200.f, 0.f },{ -BigWall_Rad1 },{ INSTALLED_OBJECT_TYPE::Box  } } }, //29
		{ 46,{ { -170.f, 15.f, 200.f, 0.f },{ -BigWall_Rad1 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 47,{ { -170.f, 25.f, 200.f, 0.f },{ -BigWall_Rad1 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 48,{ { -170.f, 35.f, 200.f, 0.f },{ -BigWall_Rad1 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 49,{ { -167.f, 35.f, 212.f, 0.f },{  -BigWall_Rad1},{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 50,{ { -165.f, 35.f, 223.f, 0.f },{  -BigWall_Rad1},{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 51,{ { -163.f, 35.f, 234.f, 0.f },{  -BigWall_Rad1},{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 52,{ { -163.f, 25.f, 234.f, 0.f },{  -BigWall_Rad1},{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 53,{ { -163.f, 15.f, 234.f, 0.f },{ -BigWall_Rad1 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 54,{ { -163.f,  5.f, 234.f, 0.f },{ -BigWall_Rad1 },{ INSTALLED_OBJECT_TYPE::Box  } } }, //38

		{ 55,{ { -270.f,   5.f,  -70.f, 0.f },{ BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } }, //39
		{ 56,{ { -270.f,  15.f,  -70.f, 0.f },{ BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } }, //40
		{ 57,{ { -270.f,  25.f,  -70.f, 0.f },{ BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 58,{ { -270.f,  35.f,  -70.f, 0.f },{ BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 59,{ { -265.f,  35.f,  -80.f, 0.f },{ BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 60,{ { -260.f,  35.f,  -90.f, 0.f },{ BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 61,{ { -255.f,  35.f, -100.f, 0.f },{ BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 62,{ { -255.f,  25.f, -100.f, 0.f },{ BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 63,{ { -255.f,  15.f, -100.f, 0.f },{ BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 64,{ { -255.f,   5.f, -100.f, 0.f },{ BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } }, //48

		{ 65,{ { -20.f,   5.f, -280.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 66,{ { -20.f,  15.f, -280.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 67,{ { -20.f,  25.f, -280.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 68,{ { -20.f,  35.f, -280.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 69,{ { -10.f,  35.f, -280.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 70,{ {   0.f,  35.f, -280.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 71,{ {  10.f,  35.f, -280.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 72,{ {  10.f,  25.f, -280.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 73,{ {  10.f,  15.f, -280.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 74,{ {  10.f,   5.f, -280.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Box  } } },					 //58

		{ 75,{ { 270.f,  5.f,  -70.f,  0.f  },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 76,{ { 270.f, 15.f,  -70.f,  0.f  },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 77,{ { 270.f, 25.f,  -70.f,  0.f  },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 78,{ { 270.f, 35.f,  -70.f,  0.f  },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 79,{ { 265.f, 35.f,  -80.f,  0.f  },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 80,{ { 260.f, 35.f,  -90.f,  0.f  },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 81,{ { 255.f, 35.f, -100.f,  0.f  },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 82,{ { 255.f, 25.f, -100.f,  0.f  },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 83,{ { 255.f, 15.f, -100.f,  0.f  },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 84,{ { 255.f,  5.f, -100.f,  0.f  },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::Box  } } },  //68

		{ 85, { { 170.f,  5.f, 200.f,  0.f },{ BigWall_Rad1  },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 86, { { 170.f, 15.f, 200.f,  0.f },{ BigWall_Rad1  },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 87, { { 170.f, 25.f, 200.f,  0.f },{ BigWall_Rad1  },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 88, { { 170.f, 35.f, 200.f,  0.f },{ BigWall_Rad1  },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 89, { { 165.f, 35.f, 210.f,  0.f },{ BigWall_Rad1  },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 90, { { 160.f, 35.f, 220.f,  0.f },{ BigWall_Rad1  },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 91, { { 155.f, 35.f, 230.f, 0.f  },{ BigWall_Rad1  },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 92, { { 155.f, 25.f, 230.f, 0.f  },{ BigWall_Rad1  },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 93, { { 155.f, 15.f, 230.f, 0.f  },{ BigWall_Rad1  },{ INSTALLED_OBJECT_TYPE::Box  } } },
		{ 94, { { 155.f,  5.f, 230.f, 0.f  },{ BigWall_Rad1  },{ INSTALLED_OBJECT_TYPE::Box  } } },		   //78

		//Column
		//4 = 4
		{ 95,{ {  110.f,  0.f,   0.f,  0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Column } } },			       //79
		{ 96,{ { -110.f,  0.f,   0.f,  0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Column } } },
		{ 97,{ {    0.f,  0.f, -110.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Column } } },
		{ 98,{ {    0.f,  0.f,  110.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::Column } } },				   //82

		//BrokenCart
		//1 = 1
		{ 99,{ {  -60.f,  0.f, -100.f, 0.f },{ 0 },{ INSTALLED_OBJECT_TYPE::BrokenCart } } },		       //83

		//BigWall = 5
		{ 100,{ { -BigWall_X1, 0.f, BigWall_Z1, 0.f },{ -BigWall_Rad1 },	{ INSTALLED_OBJECT_TYPE::BigWall } } },
		{ 101,{ {  BigWall_X1, 0.f, BigWall_Z1, 0.f },{  BigWall_Rad1 },	{ INSTALLED_OBJECT_TYPE::BigWall } } },
		{ 102,{ {  BigWall_X2, 0.f, BigWall_Z2, 0.f },{ -BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::BigWall } } },
		{ 103,{ {  0.f,        0.f, BigWall_Z3, 0.f },{  0 },				{ INSTALLED_OBJECT_TYPE::BigWall } } },
		{ 104,{ { -BigWall_X2, 0.f, BigWall_Z2, 0.f },{  BigWall_Rad1 * 2 },{ INSTALLED_OBJECT_TYPE::BigWall } } },

		//SmallWall
		//좌상
		{ 105,{ { -BigWall_X1 - 200,   0.f, 480.f  * cosf(0.4f * MMPE_PI), 0.f },{ MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 106,{ { -BigWall_X1 - 140,   0.f, 400.f  * cosf(0.4f * MMPE_PI), 0.f },{ MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 107,{ { -BigWall_X1 - 80,	   0.f, 320.f  * cosf(0.4f * MMPE_PI), 0.f },{ MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		
		//중상
		{ 108,{ { 0.f,  0.f, 530.f,  0.f },{ MMPE_PI / 2 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 109,{ { 0.f,  0.f, 460.f,  0.f },{ MMPE_PI / 2 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 110,{ { 0.f,  0.f, 390.f,  0.f },{ MMPE_PI / 2 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },

		//우상
		{ 111,{ { BigWall_X1 + 200, 0.f, 480.f * cosf(0.4f * MMPE_PI) },{ -MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 112,{ { BigWall_X1 + 140, 0.f, 400.f * cosf(0.4f * MMPE_PI) },{ -MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 113,{ { BigWall_X1 + 80,  0.f, 320.f * cosf(0.4f * MMPE_PI) },{ -MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },

		//우하
		{ 114,{ { 520.f * sinf(0.8f * MMPE_PI), 0.f, -500.f * cosf(0.2f * MMPE_PI) },{ MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 115,{ { 470.f * sinf(0.8f * MMPE_PI), 0.f, -440.f * cosf(0.2f * MMPE_PI) },{ MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 116,{ { 420.f * sinf(0.8f * MMPE_PI), 0.f, -380.f * cosf(0.2f * MMPE_PI) },{ MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },

		//좌하
		{ 117,{ { -520.f * sinf(0.8f * MMPE_PI), 0.f, -500.f * cosf(0.2f * MMPE_PI) },{ -MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 118,{ { -470.f * sinf(0.8f * MMPE_PI), 0.f, -440.f * cosf(0.2f * MMPE_PI) },{ -MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 119,{ { -420.f * sinf(0.8f * MMPE_PI), 0.f, -380.f * cosf(0.2f * MMPE_PI) },{ -MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },

		//좌상 Building
		{ 120,{ { -BigWall_X1 - 20,    0.f, 240.f  * cosf(0.4f * MMPE_PI), 0.f		},{ MMPE_PI / 8 },		{ INSTALLED_OBJECT_TYPE::NormalBuilding } } },
		//중상 Building
		{ 121,{ { 0.f,  0.f, 320.f,  0.f											},{ MMPE_PI / 2 },		{ INSTALLED_OBJECT_TYPE::NormalBuilding } } },
		//우상 Building
		{ 122,{ { BigWall_X1 + 20,  0.f, 240.f * cosf(0.4f * MMPE_PI)               },{ -MMPE_PI / 8 },		{ INSTALLED_OBJECT_TYPE::NormalBuilding } } },
		//우하 Building
		{ 123,{ { 370.f * sinf(0.8f * MMPE_PI), 0.f, -320.f * cosf(0.2f * MMPE_PI)  },{ MMPE_PI / 3 },      { INSTALLED_OBJECT_TYPE::NormalBuilding } } },
		//좌하 Building
		{ 124,{ { -370.f * sinf(0.8f * MMPE_PI), 0.f, -320.f * cosf(0.2f * MMPE_PI) },{ -MMPE_PI / 3 },     { INSTALLED_OBJECT_TYPE::NormalBuilding } } },

		//RigidBody
		{ 125,{ {    0.f, 200.f,  290.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 126,{ {    0.f, 100.f,  290.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 127,{ { -270.f,  50.f,   60.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 128,{ {  270.f, 100.f,   60.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 129,{ { -210.f, 390.f, -200.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 130,{ {  200.f, 440.f, -180.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 131,{ {  180.f, 310.f,  -30.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 132,{ { -231.f,  50.f, -160.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 133,{ { -231.f, 100.f, -160.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 134,{ { -190.f, 370.f, -340.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 135,{ {  -70.f,  40.f, -155.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
		{ 136,{ {  -70.f,  90.f, -155.f, 0.f } , { 0 } , { INSTALLED_OBJECT_TYPE::Rigidbodybox } } },
	};
}

void CStaticObject::CreateGameObject()
{
	for (unsigned int i = 0; i < 1; ++i)
		g_staticobjs.insert(move(new CSecondFloorObject(i)));

	for (unsigned int i = 1; i < 9; ++i)
		g_staticobjs.insert(move(new CSmallWallObject(i)));

	for (unsigned int i = 9; i < 13; ++i)
		g_staticobjs.insert(move(new CNormalBoxObject(i)));

	for (unsigned int i = 13; i < 17; ++i)
		g_moveobjs.insert(move(new CMoveCubeObject(i, 50.f)));

	for (unsigned int i = 17; i < 95; ++i)
		g_staticobjs.insert(move(new CNormalBoxObject(i)));

	for (unsigned int i = 95; i < 99; ++i)
		g_staticobjs.insert(move(new CColumnObject(i)));
	
	for (unsigned int i = 99; i < 100; ++i)
		g_staticobjs.insert(move(new CBrokenCartObject(i)));

	for (unsigned int i = 100; i < 105; ++i)
		g_staticobjs.insert(move(new CBigWallObject(i)));

	for (unsigned int i = 105; i < 120; ++i)
		g_staticobjs.insert(move(new CSmallWallObject(i)));

	for (unsigned int i = 120; i < 125; ++i)
		g_staticobjs.insert(move(new CBuildingObject(i)));

	for (unsigned int i = 125; i < 137; ++i)
		g_rigidobjs.insert(move(new CRigidCubeObject(i)));
		
	for (auto sobj : g_staticobjs)
		sobj->AfterGravitySystem(0);

	for (const auto& sobj : g_staticobjs)
		sobj->UpdateDataForPacket();
}

void CStaticObject::StaticObjectLoop()
{
	MakeStaticObjectBasicData();
	CreateGameObject();
}

void CStaticObject::UpdateDataForPacket()
{
	m_stc_sobjdata.ID = m_id;
	m_stc_sobjdata.degree = m_degree;
	m_stc_sobjdata.Fixed = m_fixed;
	m_stc_sobjdata.Pos = m_pos4f;
	m_stc_sobjdata.Rotate_status = m_rot4f;
	m_stc_sobjdata.type = m_type;
	m_stc_sobjdata.TextureColor = m_textureNumber;
}


CStaticObject::~CStaticObject()
{
	if (pp != nullptr)
		delete pp;

	if (rb != nullptr)
		delete rb;
}

CNormalBoxObject::CNormalBoxObject(unsigned int id)
{
	//스테틱오브젝트 충돌 시, 회전된 스테틱오브젝트들은 Lookvector 와 Rightvector를 회전한 값에 따라 바꿔줘야한다
	//그래야지 충돌처리할 때 Lookvector, Rightvector를 이용하는데, 바뀐 값이 적용되서 충돌처리를 한다 

	pp = new PhysicsPoint();

	//---------------------오브젝트에 대한 기본 데이터값 업데이트---------------------//

	m_id = id;
	m_fixed = true;
	//m_alive = true;
	//m_dir = 0;
	//m_ai = true;
	m_godmode = true;
	//m_airbone = false;
	m_degree = g_objectData[m_id].degree;

	m_pos4f = g_objectData[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };

	m_type = INSTALLED_OBJECT_TYPE::Box;

	m_textureNumber = rand() % 400 + 100;

	//----------rot, degree값을 이용한 Lookvector, Rightvector, Upvector 값 업데이트----------//
	
	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookvector();
	UpdateUpvector();

	//---------기타 물리효과 데이터 초기화(충돌박스, 댐핑계수, 바운스유무, 질량)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(5, 5, 5);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------패킷 통신을 위한 StaticObject 패킷송신용데이터 업데이트---------------//
	UpdateDataForPacket();

}

CSmallWallObject::CSmallWallObject(unsigned int id)
{
	//스테틱오브젝트 충돌 시, 회전된 스테틱오브젝트들은 Lookvector 와 Rightvector를 회전한 값에 따라 바꿔줘야한다
	//그래야지 충돌처리할 때 Lookvector, Rightvector를 이용하는데, 바뀐 값이 적용되서 충돌처리를 한다 

	pp = new PhysicsPoint();

	//---------------------오브젝트에 대한 기본 데이터값 업데이트---------------------//

	m_id = id;
	m_fixed = true;
	//m_alive = true;
	//m_dir = 0;
	//m_ai = true;
	m_godmode = true;
	//m_airbone = false;
	m_degree = g_objectData[m_id].degree;

	m_pos4f = g_objectData[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };

	m_type = INSTALLED_OBJECT_TYPE::SmallWall;

	//----------rot, degree값을 이용한 Lookvector, Rightvector, Upvector 값 업데이트----------//

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };
	
	UpdateLookvector();
	UpdateUpvector();

	//---------기타 물리효과 데이터 초기화(충돌박스, 댐핑계수, 바운스유무, 질량)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(20, 10, 5);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------패킷 통신을 위한 StaticObject 패킷송신용데이터 업데이트---------------//
	UpdateDataForPacket();
}

CBigWallObject::CBigWallObject(unsigned int id)
{
	//스테틱오브젝트 충돌 시, 회전된 스테틱오브젝트들은 Lookvector 와 Rightvector를 회전한 값에 따라 바꿔줘야한다
	//그래야지 충돌처리할 때 Lookvector, Rightvector를 이용하는데, 바뀐 값이 적용되서 충돌처리를 한다 

	pp = new PhysicsPoint();

	//---------------------오브젝트에 대한 기본 데이터값 업데이트---------------------//

	m_id = id;
	m_fixed = true;
	//m_alive = true;
	//m_dir = 0;
	//m_ai = true;
	m_godmode = true;
	//m_airbone = false;
	m_degree = g_objectData[m_id].degree;

	m_pos4f = g_objectData[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };

	m_type = INSTALLED_OBJECT_TYPE::BigWall;


	//----------rot, degree값을 이용한 Lookvector, Rightvector, Upvector 값 업데이트----------//

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	UpdateLookvector();
	UpdateUpvector();

	//---------기타 물리효과 데이터 초기화(충돌박스, 댐핑계수, 바운스유무, 질량)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(350, 50, 5);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------패킷 통신을 위한 StaticObject 패킷송신용데이터 업데이트---------------//
	UpdateDataForPacket();
}

CBuildingObject::CBuildingObject(unsigned int id)
{
	//스테틱오브젝트 충돌 시, 회전된 스테틱오브젝트들은 Lookvector 와 Rightvector를 회전한 값에 따라 바꿔줘야한다
	//그래야지 충돌처리할 때 Lookvector, Rightvector를 이용하는데, 바뀐 값이 적용되서 충돌처리를 한다 

	pp = new PhysicsPoint();

	//---------------------오브젝트에 대한 기본 데이터값 업데이트---------------------//

	m_id = id;
	m_fixed = true;
	//m_alive = true;
	//m_dir = 0;
	//m_ai = true;
	m_godmode = true;
	//m_airbone = false;
	m_degree = g_objectData[m_id].degree;

	m_pos4f = g_objectData[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };

	m_type = INSTALLED_OBJECT_TYPE::NormalBuilding;

	m_textureNumber = rand() % 400 + 100;

	//----------rot, degree값을 이용한 Lookvector, Rightvector, Upvector 값 업데이트----------//

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };			
	
	UpdateLookvector();
	UpdateUpvector();

	//---------기타 물리효과 데이터 초기화(충돌박스, 댐핑계수, 바운스유무, 질량)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(15, 35, 15);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------패킷 통신을 위한 StaticObject 패킷송신용데이터 업데이트---------------//
	UpdateDataForPacket();
}

CSecondFloorObject::CSecondFloorObject(unsigned int id)
{
	//스테틱오브젝트 충돌 시, 회전된 스테틱오브젝트들은 Lookvector 와 Rightvector를 회전한 값에 따라 바꿔줘야한다
	//그래야지 충돌처리할 때 Lookvector, Rightvector를 이용하는데, 바뀐 값이 적용되서 충돌처리를 한다 

	pp = new PhysicsPoint();
	
	//---------------------오브젝트에 대한 기본 데이터값 업데이트---------------------//

	m_id = id;
	m_fixed = true;
	//m_alive = true;
	//m_dir = 0;
	//m_ai = true;
	m_godmode = true;
	//m_airbone = false;
	m_degree = g_objectData[m_id].degree;

	m_pos4f = g_objectData[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };

	m_type = INSTALLED_OBJECT_TYPE::SecondFloor;

	//----------rot, degree값을 이용한 Lookvector, Rightvector, Upvector 값 업데이트----------//

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, MMPE_PI * 0.25);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	UpdateLookvector();
	UpdateUpvector();

	//---------기타 물리효과 데이터 초기화(충돌박스, 댐핑계수, 바운스유무, 질량)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(90, 0.5, 90);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------패킷 통신을 위한 StaticObject 패킷송신용데이터 업데이트---------------//
	UpdateDataForPacket();
}

CColumnObject::CColumnObject(unsigned int id)
{
	//스테틱오브젝트 충돌 시, 회전된 스테틱오브젝트들은 Lookvector 와 Rightvector를 회전한 값에 따라 바꿔줘야한다
	//그래야지 충돌처리할 때 Lookvector, Rightvector를 이용하는데, 바뀐 값이 적용되서 충돌처리를 한다 

	pp = new PhysicsPoint();

	//---------------------오브젝트에 대한 기본 데이터값 업데이트---------------------//

	m_id = id;
	m_fixed = true;
	//m_alive = true;
	//m_dir = 0;
	//m_ai = true;
	m_godmode = true;
	//m_airbone = false;
	m_degree = g_objectData[m_id].degree;

	m_pos4f = g_objectData[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };
	
	m_type = INSTALLED_OBJECT_TYPE::Column;

	m_textureNumber = rand() % 400 + 100;

	//----------rot, degree값을 이용한 Lookvector, Rightvector, Upvector 값 업데이트----------//

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	UpdateLookvector();
	UpdateUpvector();

	//---------기타 물리효과 데이터 초기화(충돌박스, 댐핑계수, 바운스유무, 질량)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(15, 45, 15);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------패킷 통신을 위한 StaticObject 패킷송신용데이터 업데이트---------------//
	UpdateDataForPacket();
}

CBrokenCartObject::CBrokenCartObject(unsigned int id)
{
	//스테틱오브젝트 충돌 시, 회전된 스테틱오브젝트들은 Lookvector 와 Rightvector를 회전한 값에 따라 바꿔줘야한다
	//그래야지 충돌처리할 때 Lookvector, Rightvector를 이용하는데, 바뀐 값이 적용되서 충돌처리를 한다 

	pp = new PhysicsPoint();

	//---------------------오브젝트에 대한 기본 데이터값 업데이트---------------------//

	m_id = id;
	m_fixed = true;
	//m_alive = true;
	//m_dir = 0;
	//m_ai = true;
	m_godmode = true;
	//m_airbone = false;
	m_degree = g_objectData[m_id].degree;

	m_pos4f = g_objectData[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };

	m_type = INSTALLED_OBJECT_TYPE::BrokenCart;

	//----------rot값을 이용한 Lookvector, Rightvector, Upvector 값 업데이트----------//

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	xmf4_rot = { m_rot4f.x, m_rot4f.y, m_rot4f.z, m_rot4f.w };
	auto q = XMLoadFloat4(&xmf4_rot);
	XMFLOAT3 axis{ 0,1,0 };
	auto q2 = QuaternionRotation(axis, m_degree);
	xmf4_rot = QuaternionMultiply(xmf4_rot, q2);
	m_rot4f = { xmf4_rot.x, xmf4_rot.y, xmf4_rot.z, xmf4_rot.w };

	UpdateLookvector();
	UpdateUpvector();

	//---------기타 물리효과 데이터 초기화(충돌박스, 댐핑계수, 바운스유무, 질량)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(15, 8, 15);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------패킷 통신을 위한 StaticObject 패킷송신용데이터 업데이트---------------//
	UpdateDataForPacket();
}

CRigidCubeObject::CRigidCubeObject(unsigned int id)
{
	rb = new RigidBody();
	pp = new PhysicsPoint();

	m_id = id;
	m_fixed = false;
	m_alive = true;

	m_dir = 0;
	m_ai = true;
	m_godmode = true;
	m_airbone = false;
	m_type = Rigidbodybox;

	m_ability.curHP = 100;
	m_ability.orignHP = 100;
	m_ability.attack = 0;
	m_ability.speed = 0;
	m_ability.level = 1;
	m_ability.exp = 0;

	m_degree = 0;

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookvector();
	UpdateUpvector();

	//XMFLOAT4 xmf4 = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	xmf4_pos = XMFLOAT4(g_objectData[id].pos.x, g_objectData[id].pos.y  , g_objectData[id].pos.z, g_objectData[id].pos.w);
	rb->SetPosition(&xmf4_pos);
	rb->SetHalfBox(10, 10, 10);
	rb->SetDamping(0.5f, 0.38f);
	rb->SetBounce(false);
	rb->SetMass(1.5);
	rb->SetIMoment(10, 10, 10);

	xmf4_rot = { 0,0,0,1 };
	rb->SetOrient(&xmf4_rot);

	//물리엔진 값을 이용한 pos 와 rot을 업데이트
	UpdateRPosCenterPos();
	UpdateRRotatePos();

	XMFLOAT3 testForce{ -5,-3,2 };
	XMFLOAT3 testPoint{ -15, 5,-5 };

 	rb->AddForcePoint(testForce, testPoint);
	rb->integrate(0.1);


}

void CRigidCubeObject::AmendObject(XMFLOAT3 axis, float radian, CMonoObject * obj)
{
	XMFLOAT4 q = QuaternionRotation(axis, radian);
	
	obj->SetRotatePos4f(QuaternionMultiply(obj->GetRotatePos4f(), q));
	obj->UpdateLookvector();
	obj->UpdateUpvector();
}

void CRigidCubeObject::RigidBodyCollisionPlane(XMFLOAT3 & Normal, float distance, double deltime, CMonoObject * obj)
{
	
	if (obj->GetRigidBody() != nullptr)
	{
		if (obj->GetRigidBody()->AmendTime > 0)
		{
			obj->GetRigidBody()->AmendTime -= deltime;

			if (obj->GetRigidBody()->AmendTime <= 0)
			{
				obj->GetRigidBody()->AmendTime = 0;
				obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);
				obj->GetRigidBody()->SetVelocity(0, 0, 0);
			}
		}
		else
		{
			obj->GetRigidBody()->AmendTime = 0;
		}

		XMFLOAT4 arr[8];
		//cout << "Main Pos: " << obj->GetRigidBody()->GetPosition().x << " , " << obj->GetRigidBody()->GetPosition().y << " , " << obj->GetRigidBody()->GetPosition().z << " , " << obj->GetRigidBody()->GetPosition().w << "\n";
 		//cout << "Mainframework: " << obj->GetLookVector().x << " , " << obj->GetLookVector().y << " , " << obj->GetLookVector().z << "\n";
		//cout << "Mainframework: " << obj->GetRightVector().x << " , " << obj->GetRightVector().y << " , " << obj->GetRightVector().z << "\n";


		//먼저 8 개의 점을 가져온다.
		obj->GetRigidBody()->GetEightPoint(arr, obj->GetUpVector(), obj->GetLookVector(), obj->GetRightVector());
		vector<CollisionPoint> allpoint;
		vector<CollisionPoint> tempcollisionpoint;
		vector<CollisionPoint> contactpoint;

		for (int i = 0; i < 8; ++i)
		{
			float temppenetration = arr[i].x * Normal.x + arr[i].y * Normal.y + arr[i].z * Normal.z;
			//충돌 점을 생성한다음 저장한다.
			CollisionPoint cp;
			cp.Pos = arr[i];
			cp.penetration = temppenetration;
			cp.pAxis = Normal;
			allpoint.push_back(cp);
		}

		// 5) 번을 처리함
		//penetration이 작은 순으로 정렬
		sort(allpoint.begin(), allpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
		{
			return a.penetration < b.penetration;
		});

		// 6) 번을 처리함.
		//실제로 정렬된 녀석중 1차적으로 충돌이 된 녀석들을 tempcollisionpoint에 저장한다.
		for (auto i : allpoint)
		{
			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0이하면 실제로 충돌함
				tempcollisionpoint.push_back(i);

		}
		//이제 가장 깊은 녀석을 기준으로 충돌리스트를 돌면서 penetration을 뺐을때 결과가 충분히 작으면
		//이녀석도 밀어내거나 힘을가할때 사용될 수 있는 충돌점이다. 이 것들이 최종적인 충돌점이다.

		// 7) 번을 처리함
		for (auto i : tempcollisionpoint)
		{
			//실제충돌의 첫번째 깊이 - penetration들을 비교해서 Epsilon이면 이제 실제 접촉점이 저장되는 곳에 넣는다.
			if (fabsf(tempcollisionpoint.front().penetration - i.penetration) <= 0.12)//차가 0.1정도면 실제로 충돌 가능성이 있다고 판단한다.
			{
				contactpoint.push_back(i);
			}

		}

		//------------- 여기가 가장 중요한 분기 -----------------//

		//------------- size 가 1/2/4/그외 일때 로 나눠서 처리 ------------------//
		int size = contactpoint.size();
		if (size == 1)
		{
			//size가 1이면 뭘해야할까? 바로 정점 2개가 맞부딪힐수있도록 회전시켜야한다.
			//그러기 위해서는 적당한 위치에 충격량을 가한다.
			//문제는 이게 과회전이 일어날 수 있다.
			//따라서 적당한 회전각도가 되면 그 각도로 보정한다. 다만 해당 충격량이 너무 크지 않을때만 보정한다.

			//2인자점 - 1인자점을 해서 2인자점을 향하는 벡터를 만든다.
			//이때 2인자 점을 향하는 각도가 특정각도 이하면 그 각도로 보정한다.

			//문제는 2인자점을 어느것으로 할것인가 이다. 경우의수는 2가지다. 왼쪽이나 오른쪽점 / 또는 대각선 점.

			//가장가까운 2인자점.
			auto V1 = Float4Add(allpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			//현재점의 대각선에 위치한 2인자점
			auto sV1 = Float4Add(allpoint[3].Pos, contactpoint[0].Pos, false);
			sV1 = Float4Normalize(sV1);


			//가장가까운 2인자점의 각도를 구하기
			float NdotV1 = Normal.x*V1.x + Normal.y*V1.y + Normal.z*V1.z;

			XMFLOAT3 ProjAB = Normal;
			ProjAB.x *= NdotV1;
			ProjAB.y *= NdotV1;
			ProjAB.z *= NdotV1;

			auto V2 = Float4Add(V1, XMFloat3to4(ProjAB), false);
			V2 = Float4Normalize(V2);


			auto tempdot = V1.x*V2.x + V1.y*V2.y + V1.z*V2.z;
			if (abs(tempdot) > 1)
			{
				if (tempdot > 0)
					tempdot = 1;
				else
					tempdot = -1;
			}
			//교정할 각도.  
			double theta = acos(tempdot);

			//대각선 2인자점의 각도를 구하기
			float sNdotV1 = Normal.x*sV1.x + Normal.y*sV1.y + Normal.z*sV1.z;

			XMFLOAT3 sProjAB = Normal;
			sProjAB.x *= sNdotV1;
			sProjAB.y *= sNdotV1;
			sProjAB.z *= sNdotV1;

			auto sV2 = Float4Add(sV1, XMFloat3to4(sProjAB), false);
			sV2 = Float4Normalize(sV2);


			auto stempdot = sV1.x*sV2.x + sV1.y*sV2.y + sV1.z*sV2.z;
			if (abs(stempdot) > 1)
			{
				if (stempdot > 0)
					stempdot = 1;
				else
					stempdot = -1;
			}
			//교정할 각도.  
			double stheta = acos(stempdot);

			//충격량을 구함. 충격량이 특정 값 이하일때만 보정가능.

			CollisionPoint fp;//충격량을 가할 지점
			fp.Pos = XMFLOAT4(0, 0, 0, 0);
			fp.pAxis = Normal;

			fp.Pos = contactpoint[0].Pos;
			fp.penetration = contactpoint[0].penetration;
			float impurse = obj->GetRigidBody()->CalculateImpulse(fp, NULL, 1);

			//최대임펄스를 구한다.
			if (impurse > obj->GetRigidBody()->GetMaxImpurse())
				impurse = obj->GetRigidBody()->GetMaxImpurse();


			//최소 임펄스를 구한다.
			if (impurse < obj->GetRigidBody()->GetMinImpurse())
				impurse = obj->GetRigidBody()->GetMinImpurse();




			//그후 사잇각이 특정각도 이하면 보정시킨다. 
			//단 이게 double로 해도 0이아닌데 0이나오는경우가 생긴다.
			//따라서 0일경우 그냥 충격량을 가해서 각도를 변경시킨다.

			if (abs(stheta) <= MMPE_PI / 20 && abs(stheta) != 0 && obj->GetRigidBody()->AmendTime <= 0)
			{
				//회전축을 구하고..
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(sV1, sV2));
				mAxis = Float3Normalize(mAxis);
				//보정을 시킨다.
				AmendObject(mAxis, stheta, obj);

				//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
				//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
				obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

			}
			else if (abs(theta) <= MMPE_PI / 25 && abs(theta) != 0 && obj->GetRigidBody()->AmendTime <= 0)
			{
				//대략 5도 이하면 보정시킴.

				//회전축을 구함
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V2));
				mAxis = Float3Normalize(mAxis);

				//보정을 시킨다
				AmendObject(mAxis, theta, obj);

				//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
				//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
				obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

			}
			else //아니라면 이제 충격량을 적당한 지점에 가한다!
			{
				//여기에 왔다는것은 더이상 보정을 하지 않거나 보정을 아직 할필요가 없는 경우다.

				//Jm = J/M
				//임펄스의 비율을 나눈다.  즉. 일반적인 1:1 관계에서 1.3: 0.3 정도로 둔다.
				//나머지 0.7은 소실된 에너지라 치자.
				if (obj->GetRigidBody()->AmendTime <= 0)
				{
					auto ratioImpurse = impurse * 0.3;

					auto Jm = Normal;
					Jm.x *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);
					Jm.y *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);
					Jm.z *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);


					//각속도 계산
					//W = 기존 각속도 + ((Q-P)Ximpurse)*InverseI
					auto W = obj->GetRigidBody()->GetAngularVelocity();
					XMVECTOR rxi = XMLoadFloat3(&XMFloat4to3(Float4Add(fp.Pos, obj->GetCenterPos4f(), false)));
					rxi = XMVector3Cross(rxi, XMLoadFloat3(&Normal));
					rxi *= (ratioImpurse);
					rxi = XMVector3Transform(rxi, XMLoadFloat4x4(&obj->GetRigidBody()->GetIMoment()));

					XMFLOAT3 ia;
					XMStoreFloat3(&ia, rxi);

					W = Float3Add(W, ia);
					W = Float3Float(W, obj->GetRigidBody()->GetE() / 2);
					XMFLOAT3 lastvel = obj->GetRigidBody()->GetVelocity();
					obj->GetRigidBody()->SetVelocity(Float3Float(Float3Add(lastvel, Jm), obj->GetRigidBody()->GetE()));
					obj->GetRigidBody()->SetAngularVelocity(W);
				}

				//이제 속도와 각속도는 변경 했으니, 겹쳐진 부분 해소
				//가장 작은값의 penetration(가장 깊은)만큼 올리면 된다.
				auto px = fabsf(contactpoint[0].penetration)*Normal.x;
				auto py = fabsf(contactpoint[0].penetration)*Normal.y;
				auto pz = fabsf(contactpoint[0].penetration)*Normal.z;

				obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
				obj->UpdateRigidCenterPos();
		
			}
		}
		else if (size == 2)
		{
			//삐뚫어진 점 2개면 보정을 해야한다.
			//보정을 해야하는지 검사를 하기 위해 평면의 노멀과 접촉점 두개를 이은 벡터가 내적시 0이 나오는지 검사한다.
			auto V1 = Float4Add(contactpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			float Dot = V1.x * Normal.x + V1.y * Normal.y + V1.z * Normal.z;

			if (fabsf(Dot) <= MMPE_EPSILON)
				Dot = 0;

			//결과가 0이면 보정이 필요없음.
			if (Dot == 0)
			{
				//이제 2개의 충돌을 4개의 점충돌로 바꿀 수 있는 보정을 할 수 있는지 검사한다.
				//그러기 위해서 3번째 점- 첫번째 점 을 해서 3번째 점을 향하는 벡터를 만든다.
				//그 후 이게 V1과 내적시 0 이 나오면 첫번째 점과 세번째 점이 대각선이 아니란것이고,
				//아니라면 두번째 점과 세번째점을 이용해 벡터를 만들어야한다.
				auto V2 = Float4Add(allpoint[2].Pos, contactpoint[0].Pos, false);
				V2 = Float4Normalize(V2);
				float Dot2 = V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;

				if (fabsf(Dot2) <= MMPE_EPSILON)
					Dot2 = 0;

				//첫번째 점과 세번째점이 맞지 않는경우 두번째 점과 세번째점으로 맞춘다.
				if (Dot2 != 0)
				{

					V2 = Float4Add(allpoint[2].Pos, contactpoint[1].Pos, false);
					V2 = Float4Normalize(V2);
				}
				//이제 여기 왔다는것은, 4개의 점을 만들 수 있는 녀석들을 찾은 셈이다.
				//다만 바로 보정하면 안되고, 이게 어느정도 각도 차가 덜 나야 된다.
				//그럼 그 각도는 무엇인가??
				/*
				ㅅ  1)(V2)
				ㅣ /
				ㅣ/
				ㅡㅡㅡ2)(V3)

				현재 V2가 바로 1) 이다.
				이게 평면의 노멀과 수직인 2) 의 벡터로 만들건데
				1)과 2) 각도를 재다가 적당한 각도 한 5~10 도 정도 되면 그만큼 회전 시키면 끝이다!
				그러기 위해선 2)를 구해야한다. 2)는 평면의 노멀을 A 현재 V2를 B라 하고 ProjAB 한결과를 V2에 뺀다음 V2를 노멀화 하면 된다.
				*/

				float NdotV2 = Normal.x*V2.x + Normal.y*V2.y + Normal.z*V2.z;
				XMFLOAT3 ProjAB = Normal;
				ProjAB.x *= NdotV2;
				ProjAB.y *= NdotV2;
				ProjAB.z *= NdotV2;

				auto V3 = Float4Add(V2, XMFloat3to4(ProjAB), false);
				V3 = Float4Normalize(V3);
				//이제 V2와 V3를 아니까 V2와 V3의 사잇각을 구한다음 V2에서 V3로 외적해서 Axis를 구한다.

				//먼저 사잇각도를 구한다.

				//기존에는 float으로 했는데, 0이아니어야 하는데 0이나오는경우가 생김..


				auto tempdot = V2.x*V3.x + V2.y*V3.y + V2.z*V3.z;
				if (abs(tempdot) > 1)
				{
					if (tempdot > 0)
						tempdot = 1;
					else
						tempdot = -1;
				}
				//교정할 각도.  
				double theta = acos(tempdot);


				CollisionPoint fp;//충격량을 가할 지점
				fp.Pos = XMFLOAT4(0, 0, 0, 0);
				fp.pAxis = Normal;

				for (auto i : contactpoint)
				{
					fp.Pos = Float4Add(fp.Pos, i.Pos);
					fp.penetration += i.penetration;
				}

				fp.Pos.x /= contactpoint.size();
				fp.Pos.y /= contactpoint.size();
				fp.Pos.z /= contactpoint.size();

				fp.penetration /= contactpoint.size();

				float impurse = obj->GetRigidBody()->CalculateImpulse(fp, NULL, 1);

				//최대임펄스를 구한다.
				if (impurse > obj->GetRigidBody()->GetMaxImpurse())
					impurse = obj->GetRigidBody()->GetMaxImpurse();


				//최소 임펄스를 구한다.
				if (impurse < obj->GetRigidBody()->GetMinImpurse())
					impurse = obj->GetRigidBody()->GetMinImpurse();


				//그후 사잇각이 특정각도 이하면 보정시킨다. 
				if (abs(theta) <= MMPE_PI / 25 && abs(theta) != 0 && abs(impurse) < obj->GetRigidBody()->GetMaxImpurse() && obj->GetRigidBody()->AmendTime <= 0)//대략 5도 이하면 보정시킴.
				{
					//회전축을 구하고
					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
					mAxis = Float3Normalize(mAxis);
					//보정을 시킨다.
					AmendObject(mAxis, theta, obj);

					//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
					//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
					obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);
					obj->GetRigidBody()->SetAccel(0, 0, 0);


				}
				else//아니라면 이제 충격량을 적당한 지점에 가한다!
				{
					//여기에 왔다는것은 더이상 보정을 하지 않거나 보정을 아직 할필요가 없는 경우다.

					//먼저 현재 충돌점은 2개이니 2개의 충돌점을 더한 후 2로 나눠주면 충돌지점 완성이다.
					//충격량은 차후에 다시 구해야겠지만, 현재는 0,50,0 정도로 가하겠다.

					//근데 여기서 경우의 수가 생긴다. 알다시피 대각선형태로 정확하게 45도 기준으로 서있는 경우의수가 생긴다.
					//이것을 없애려면 먼저 해당경우의 수에 속하는지를 검사해야한다.

					//그후 그 경우의수를 해결해야겠다. 검사를 하려면 좌우 대칭인지를 보면될것같다.
					//그리고 대칭이면 랜덤이든 아니면 오른쪽으로 충격량을 약간 가하던 하면 될것같다.

					//먼저 2/3/4/5 인덱스의 점을 비교하면서 이 길이가 0.0001정도 차이면 통과.
					float ComparePenetration = allpoint[2].penetration;
					bool pass = true;
					for (int i = 2; i < 6; i++)
					{
						if (fabsf(ComparePenetration - allpoint[i].penetration) > 0.25)
						{
							pass = false;
							break;
						}
					}

					//4개가 모두 같은 깊이면 균형을 이루는것이므로 균형을 부셔버린다.
					if (pass)
					{
						theta = -MMPE_PI / 18;
						XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
						mAxis = Float3Normalize(mAxis);
						//보정을 시킨다.
						AmendObject(mAxis, theta, obj);

						//그리고 재귀 시킨다. 왜냐하면 보정이되었으면 allpoint,tempcollisionpoint,contactpoint , penetration 모두 다 바뀌어야 하기 때문이다.
						//재귀 후 아마 2가지 경우의수가 있다. 충돌이 일어나거나, 아니면 살짝 떠있거나.. 어쨌든 잘 해결 된다.
						obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);


						return;
					}

					//Jm = J/M

					//임펄스의 비율을 나눈다.  즉. 일반적인 1:1 관계에서 1.3: 0.3 정도로 둔다.

					if (obj->GetRigidBody()->AmendTime <= 0)
					{
						auto ratioImpurse = impurse * 0.3;

						auto Jm = Normal;



						Jm.x *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);
						Jm.y *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);
						Jm.z *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);



						//각속도 계산
						//W = 기존 각속도 + ((Q-P)Ximpurse)*InverseI
						auto W = obj->GetRigidBody()->GetAngularVelocity();
						XMVECTOR rxi = XMLoadFloat3(&XMFloat4to3(Float4Add(fp.Pos, obj->GetCenterPos4f(), false)));
						rxi = XMVector3Cross(rxi, XMLoadFloat3(&Normal));
						rxi *= (ratioImpurse);
						rxi = XMVector3Transform(rxi, XMLoadFloat4x4(&obj->GetRigidBody()->GetIMoment()));

						XMFLOAT3 ia;
						XMStoreFloat3(&ia, rxi);

						W = Float3Add(W, ia);
						W = Float3Float(W, obj->GetRigidBody()->GetE() / 2);
						XMFLOAT3 lastvel = obj->GetRigidBody()->GetVelocity();
						obj->GetRigidBody()->SetVelocity(Float3Float(Float3Add(lastvel, Jm), obj->GetRigidBody()->GetE()));
						obj->GetRigidBody()->SetAngularVelocity(W);

					}
					//이제 속도와 각속도는 변경 했으니, 겹쳐진 부분 해소
					//가장 작은값의 penetration(가장 깊은)만큼 올리면 된다.
					auto px = fabsf(contactpoint[0].penetration)*Normal.x;
					auto py = fabsf(contactpoint[0].penetration)*Normal.y;
					auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
					obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
					obj->UpdateRigidCenterPos();

				}

			}
			else//Dot의 결과 삐뚫어진 점 일경우 즉 노멀과 (두번째점-첫번째점) 벡터가 나온경우 
			{
				XMFLOAT3 ProjAB = Normal;
				ProjAB.x *= Dot;
				ProjAB.y *= Dot;
				ProjAB.z *= Dot;

				//교정되었을때 V1
				auto V4 = Float4Add(V1, XMFloat3to4(ProjAB), false);
				V4 = Float4Normalize(V4);

				//교정할 각도.


				//기존에는 float으로 했는데, 0이아니어야 하는데 0이나오는경우가 생김..


				auto tempdot = V1.x*V4.x + V1.y*V4.y + V1.z*V4.z;
				if (abs(tempdot) > 1)
				{
					if (tempdot > 0)
						tempdot = 1;
					else
						tempdot = -1;
				}
				//교정할 각도.  
				double theta = acos(tempdot);

				if (abs(theta) != 0 && obj->GetRigidBody()->AmendTime <= 0)
				{

					//회전축을 구하고
					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V4));
					mAxis = Float3Normalize(mAxis);


					//보정을 시킨다.
					//여기 거치면 size = 4 인곳으로 이동됨 //velocity = 0, 0, 0
					AmendObject(mAxis, theta, obj);
					obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

				}

			}
		}
		else if (size == 4)
		{
			//임시적이지만 그냥 일단 penetration과 속도만 변경하자.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
			obj->UpdateRigidCenterPos();

			//땅에 닿았으니 현재 속도의 y는 반감되어야 한다. 원래는 탄성계수가 있지만.. 그냥 절반 감소시킨후 부호를 -로 하자.
			auto d = obj->GetRigidBody()->GetVelocity();
			d.y = -0.01 * d.y;
			obj->GetRigidBody()->SetVelocity(d);
			obj->GetRigidBody()->SetAccel(0, 0, 0);
		}
		else if (size != 0) //그외
		{
			//임시적이지만 그냥 일단 penetration과 속도만 변경하자.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
			obj->UpdateRigidCenterPos();

			//땅에 닿았으니 현재 속도의 y는 반감되어야 한다. 원래는 탄성계수가 있지만.. 그냥 절반 감소시킨후 부호를 -로 하자.
			auto d = obj->GetRigidBody()->GetVelocity();
			d.y = -0.01 * d.y;
			obj->GetRigidBody()->SetVelocity(d);
			obj->GetRigidBody()->SetAccel(0, 0, 0);
		}

		//==================================== 충돌 후 예외의 상황을 방지하기 위한 처리들 ============================================//

		//너무 낮은위치에 있을때 속도를 0으로
		if (obj->GetCenterPos4f().y < -200)
			obj->GetRigidBody()->SetVelocity(0, 0, 0);


		// 뭔가 보정을 하거나 한다음 겹쳐진 부분을 재해결.

		XMFLOAT4 tarr[8];
		obj->GetRigidBody()->GetEightPoint(tarr, obj->GetUpVector(), obj->GetLookVector(), obj->GetRightVector());//먼저 8 개의 점을 가져온다.
		std::vector<CollisionPoint> tallpoint;
		std::vector<CollisionPoint> ttempcollisionpoint;
		std::vector<CollisionPoint> tcontactpoint;

		for (int i = 0; i < 8; i++)
		{
			float temppenetration = tarr[i].x*Normal.x + tarr[i].y*Normal.y + tarr[i].z*Normal.z;
			//충돌 점을 생성한다음 저장한다.
			CollisionPoint cp;
			cp.Pos = tarr[i];
			cp.penetration = temppenetration;
			cp.pAxis = Normal;
			tallpoint.push_back(cp);
		}

		//penetration이 작은 순으로 정렬
		sort(tallpoint.begin(), tallpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
		{
			return a.penetration < b.penetration;
		});

		//실제로 정렬된 녀석중 1차적으로 충돌이 된 녀석들을 tempcollisionpoint에 저장한다.
		for (auto i : tallpoint)
		{
			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0이하면 실제로 충돌함
				ttempcollisionpoint.push_back(i);

		}

		if (ttempcollisionpoint.size() > 0)
		{
			auto px = fabsf(ttempcollisionpoint[0].penetration)*Normal.x;
			auto py = fabsf(ttempcollisionpoint[0].penetration)*Normal.y;
			auto pz = fabsf(ttempcollisionpoint[0].penetration)*Normal.z;
			obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
			obj->UpdateRigidCenterPos();
		}

		//UpdateRPosCenterPos();

		//cout << "Inner PosX: " << m_pos4f.x << " , " << "PosY: " << m_pos4f.y << " , " << "PosZ: " << m_pos4f.z << " , " << "PosW: " << m_pos4f.w << "\n";

		allpoint.clear();
		tempcollisionpoint.clear();
		contactpoint.clear();
	}
}

void CRigidCubeObject::GravitySystem(double deltime)
{
	GeneratorGravity gg;
	gg.SetGravityAccel(XMFLOAT3(0, -100, 0));

	gg.Update(deltime, *rb);
}

void CRigidCubeObject::AfterGravitySystem(double deltime)
{
	RigidBodyCollisionPlane(XMFLOAT3(0, 1, 0), 0, deltime,  this);
}

void CRigidCubeObject::Tick(double deltime)
{
	if (rb != nullptr)
		rb->integrate(deltime);

	UpdateRPosCenterPos();
	UpdateRRotatePos();
	
}

void CRigidCubeObject::UpdateDataForPacket()
{
	m_stc_robjdata.id = m_id;
	m_stc_robjdata.pos4f = m_pos4f;
	m_stc_robjdata.rot4f = m_rot4f;
	m_stc_robjdata.type = Rigidbodybox;
}

void CRigidCubeObject::Collision(unordered_set<CRigidCubeObject*>* rbobjs, double deltime)
{
	for (auto iter = rbobjs->begin(); iter != rbobjs->end(); ++iter)
	{
		if ((*iter) != this)
		{
			if ((*iter)->rb != nullptr)
			{
				bool test = rb->CollisionTest(*(*iter)->rb, m_Lookvector, m_Rightvector, m_Upvector, (*iter)->m_Lookvector, (*iter)->m_Rightvector, (*iter)->m_Upvector);

				if (test)
				{
					if (rb->CollisionPointVector[0].pAxis.y > 0)
					{
						rb->SetVelocity(rb->GetVelocity().x, 0, rb->GetVelocity().z);
						m_airbone = false;
					}

					if (rb->CollisionPointVector[0].pAxis.y < 0)
					{
						(*iter)->rb->SetVelocity((*iter)->rb->GetVelocity().x, 0, (*iter)->rb->GetVelocity().z);
						(*iter)->m_airbone = false;
					}

					rb->ResolvePenetration(*(*iter)->rb, deltime);
				}
			}
			else
			{
				if ((*iter)->pp != nullptr)
				{
					RigidBody ppConvertrb;
					ppConvertrb.SetVelocity((*iter)->pp->GetVelocity());
					ppConvertrb.SetPosition(&(*iter)->GetCenterPos4f());
					ppConvertrb.SetMass((*iter)->pp->GetMass(false));
					ppConvertrb.SetHalfBox((*iter)->pp->GetHalfBox().x, (*iter)->pp->GetHalfBox().y, (*iter)->pp->GetHalfBox().z);
					ppConvertrb.SetE(1);
					ppConvertrb.SetDamping((*iter)->pp->GetDamping(), 0);
					ppConvertrb.SetBounce((*iter)->pp->GetBounce());
					ppConvertrb.SetAngularVelocity(0, 0, 0);
					ppConvertrb.SetAccel((*iter)->pp->GetAccel());


					bool test = rb->CollisionTest(ppConvertrb, m_Lookvector, m_Rightvector, m_Upvector,
						                         (*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());
					if (test)
					{
						if (rb->CollisionPointVector[0].pAxis.y < 0)
						{
							ppConvertrb.SetVelocity(ppConvertrb.GetVelocity().x, 0, ppConvertrb.GetVelocity().z);
							(*iter)->m_airbone = false;
						}

						rb->AmendTime = 0;
						rb->ResolvePenetration(ppConvertrb, deltime);
						(*iter)->pp->SetVelocity(ppConvertrb.GetVelocity());
						*(*iter)->pp->CenterPos = ppConvertrb.GetPosition();
						(*iter)->pp->SetAccel(ppConvertrb.GetAccel());
						(*iter)->UpdatePPosCenterPos();
					}
					
				}
			}
		}
	}
}

void CRigidCubeObject::Collision(unordered_set<CStaticObject*>* sobjs, double deltime)
{
	for (auto iter = sobjs->begin(); iter != sobjs->end(); ++iter)
	{	
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			RigidBody ppConvertrb;
			ppConvertrb.SetVelocity((*iter)->GetPhysicsPoint()->GetVelocity());
			ppConvertrb.SetPosition(&(*iter)->GetCenterPos4f());
			ppConvertrb.SetMass((*iter)->GetPhysicsPoint()->GetMass(false));
			ppConvertrb.SetHalfBox((*iter)->GetPhysicsPoint()->GetHalfBox().x, (*iter)->GetPhysicsPoint()->GetHalfBox().y, (*iter)->GetPhysicsPoint()->GetHalfBox().z);
			ppConvertrb.SetE(1);
			ppConvertrb.SetDamping((*iter)->GetPhysicsPoint()->GetDamping(), 0);
			ppConvertrb.SetBounce((*iter)->GetPhysicsPoint()->GetBounce());
			ppConvertrb.SetAngularVelocity(0, 0, 0);
			ppConvertrb.SetAccel((*iter)->GetPhysicsPoint()->GetAccel());


			bool test = rb->CollisionTest(ppConvertrb, m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				if (rb->CollisionPointVector[0].pAxis.y < 0)
				{
					ppConvertrb.SetVelocity(ppConvertrb.GetVelocity().x, 0, ppConvertrb.GetVelocity().z);
					(*iter)->SetAirbone(false);
				}

				rb->AmendTime = 0;
				rb->ResolvePenetration(ppConvertrb, deltime);
				(*iter)->GetPhysicsPoint()->SetVelocity(ppConvertrb.GetVelocity());
				*(*iter)->GetPhysicsPoint()->CenterPos = ppConvertrb.GetPosition();
				(*iter)->GetPhysicsPoint()->SetAccel(ppConvertrb.GetAccel());

				(*iter)->UpdatePPosCenterPos();
			}
		}		


	}
}

void CRigidCubeObject::Collision(vector<CPlayerObject*>* clients, double deltime)
{
	for (auto iter = clients->begin(); iter != clients->end(); ++iter)
	{
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			RigidBody ppConvertrb;
			ppConvertrb.SetVelocity((*iter)->GetPhysicsPoint()->GetVelocity());
			ppConvertrb.SetPosition(&(*iter)->GetCenterPos4f());
			ppConvertrb.SetMass((*iter)->GetPhysicsPoint()->GetMass(false));
			ppConvertrb.SetHalfBox((*iter)->GetPhysicsPoint()->GetHalfBox().x, (*iter)->GetPhysicsPoint()->GetHalfBox().y, (*iter)->GetPhysicsPoint()->GetHalfBox().z);
			ppConvertrb.SetE(1);
			ppConvertrb.SetDamping((*iter)->GetPhysicsPoint()->GetDamping(), 0);
			ppConvertrb.SetBounce((*iter)->GetPhysicsPoint()->GetBounce());
			ppConvertrb.SetAngularVelocity(0, 0, 0);
			ppConvertrb.SetAccel((*iter)->GetPhysicsPoint()->GetAccel());


			bool test = rb->CollisionTest(ppConvertrb, m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				if (rb->CollisionPointVector[0].pAxis.y < 0)
				{
					ppConvertrb.SetVelocity(ppConvertrb.GetVelocity().x, 0, ppConvertrb.GetVelocity().z);
					(*iter)->SetAirbone(false);
				}

				rb->AmendTime = 0;
				rb->ResolvePenetration(ppConvertrb, deltime);
				(*iter)->GetPhysicsPoint()->SetVelocity(ppConvertrb.GetVelocity());
				*(*iter)->GetPhysicsPoint()->CenterPos = ppConvertrb.GetPosition();
				(*iter)->GetPhysicsPoint()->SetAccel(ppConvertrb.GetAccel());

				(*iter)->UpdatePPosCenterPos();
			}
		}


	}
}

void CRigidCubeObject::Collision(list<CBulletObject*>* bullets, double deltime)
{
	for (auto iter = bullets->begin(); iter != bullets->end(); ++iter)
	{
		if ((*iter)->GetPhysicsPoint() != nullptr)
		{
			RigidBody ppConvertrb;
			ppConvertrb.SetVelocity((*iter)->GetPhysicsPoint()->GetVelocity());
			ppConvertrb.SetPosition(&(*iter)->GetCenterPos4f());
			ppConvertrb.SetMass((*iter)->GetPhysicsPoint()->GetMass(false));
			ppConvertrb.SetHalfBox((*iter)->GetPhysicsPoint()->GetHalfBox().x, (*iter)->GetPhysicsPoint()->GetHalfBox().y, (*iter)->GetPhysicsPoint()->GetHalfBox().z);
			ppConvertrb.SetE(1);
			ppConvertrb.SetDamping((*iter)->GetPhysicsPoint()->GetDamping(), 0);
			ppConvertrb.SetBounce((*iter)->GetPhysicsPoint()->GetBounce());
			ppConvertrb.SetAngularVelocity(0, 0, 0);
			ppConvertrb.SetAccel((*iter)->GetPhysicsPoint()->GetAccel());


			bool test = rb->CollisionTest(ppConvertrb, m_Lookvector, m_Rightvector, m_Upvector,
				(*iter)->GetLookVector(), (*iter)->GetRightVector(), (*iter)->GetUpVector());

			if (test)
			{
				if (rb->CollisionPointVector[0].pAxis.y < 0)
				{
					ppConvertrb.SetVelocity(ppConvertrb.GetVelocity().x, 0, ppConvertrb.GetVelocity().z);
					(*iter)->SetAirbone(false);
				}

				(*iter)->SetAlive(false);
				(*iter)->m_bulldata.alive = false;

				//충돌 처리. 충격량을 가하고 겹침부분해소
				rb->CollisionResolve(ppConvertrb, XMFLOAT3(0, 0, 0), deltime, 6000, 1400, 1.5);

				(*iter)->GetPhysicsPoint()->SetVelocity(ppConvertrb.GetVelocity());
				*(*iter)->GetPhysicsPoint()->CenterPos = ppConvertrb.GetPosition();
				(*iter)->GetPhysicsPoint()->SetAccel(ppConvertrb.GetAccel());

				(*iter)->UpdatePPosCenterPos();

			}
		}


	}
}

CMoveCubeObject::CMoveCubeObject(unsigned int id, float len)
{
	m_selectedColor = rand() % 7;
	m_len = len;
	m_n = rand() % 30;

	pp = new PhysicsPoint();

	//---------------------오브젝트에 대한 기본 데이터값 업데이트---------------------//

	m_id = id;
	m_fixed = false;
	m_alive = true;
	m_pos4f = g_objectData[m_id].pos;
	m_rot4f = { 0.f, 0.f, 0.f, 1.f };
	m_dir = 0;
	m_ai = true;
	m_godmode = true;
	m_type = INSTALLED_OBJECT_TYPE::MoveCube;
	m_degree = 0;

	m_createfirst = true;

	//----------rot값을 이용한 Lookvector, Rightvector, Upvector 값 업데이트----------//

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookvector();
	UpdateUpvector();

	//---------기타 물리효과 데이터 초기화(충돌박스, 댐핑계수, 바운스유무, 질량)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(10, 5, 10);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------패킷 통신을 위한 StaticObject 패킷송신용데이터 업데이트---------------//
	//1. 고정 데이터
	m_stc_mvobjdata.id = m_id;
	m_stc_mvobjdata.texture_color = m_selectedColor;
	m_stc_mvobjdata.len = m_len;
	m_stc_mvobjdata.n = m_n;
	m_stc_mvobjdata.create_first = m_createfirst;

	//2. 업데이트되는 데이터
	UpdateDataForPacket();
}

void CMoveCubeObject::Tick(double deltime)
{
	m_n += deltime;

	m_pos4f.y = m_len * sinf(MMPE_PI * m_n * 0.15f ) + 50;

	m_stc_mvobjdata.pos4f.y = m_pos4f.y;
}

void CMoveCubeObject::UpdateDataForPacket()
{
	m_stc_mvobjdata.pos4f = m_pos4f;
	m_stc_mvobjdata.rot4f = m_rot4f;
}
