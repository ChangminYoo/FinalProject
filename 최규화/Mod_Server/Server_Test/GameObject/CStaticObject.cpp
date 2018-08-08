#include "../stdafx.h"

CStaticObject::CStaticObject()
{
}

//ť�� ���� ���� �ؽ���
//ObjData.CustomData1.w = rand() % 400 + 100; 

void CStaticObject::MakeStaticObjectBasicData()
{
	//����� ��ü ��ġ�� ���� ����� �ִ´�.
	float BigWall_X1 = 300 * sinf(0.4f * MMPE_PI);
	float BigWall_Z1 = (600 * cosf(0.4f * MMPE_PI) + 600) / 2;

	float BigWall_X2 = ((600 * sinf(0.4f * MMPE_PI)) + (600 * sinf(0.8f * MMPE_PI))) / 2;
	float BigWall_Z2 = ((600 * cosf(0.4f * MMPE_PI)) + (-600 * cosf(0.2f * MMPE_PI))) / 2;

	float BigWall_Z3 = -600 * cosf(0.2f * MMPE_PI);

	float BigWall_Rad1 = MMPE_PI / 5; //36degree

	g_objectData =
	{
		//============2��=============
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

		//============1��=============
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
		//�»� ~�ݽð���� ~���
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
		//�»�
		{ 105,{ { -BigWall_X1 - 200,   0.f, 480.f  * cosf(0.4f * MMPE_PI), 0.f },{ MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 106,{ { -BigWall_X1 - 140,   0.f, 400.f  * cosf(0.4f * MMPE_PI), 0.f },{ MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 107,{ { -BigWall_X1 - 80,	   0.f, 320.f  * cosf(0.4f * MMPE_PI), 0.f },{ MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		
		//�߻�
		{ 108,{ { 0.f,  0.f, 530.f,  0.f },{ MMPE_PI / 2 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 109,{ { 0.f,  0.f, 460.f,  0.f },{ MMPE_PI / 2 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 110,{ { 0.f,  0.f, 390.f,  0.f },{ MMPE_PI / 2 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },

		//���
		{ 111,{ { BigWall_X1 + 200, 0.f, 480.f * cosf(0.4f * MMPE_PI) },{ -MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 112,{ { BigWall_X1 + 140, 0.f, 400.f * cosf(0.4f * MMPE_PI) },{ -MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 113,{ { BigWall_X1 + 80,  0.f, 320.f * cosf(0.4f * MMPE_PI) },{ -MMPE_PI / 8 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },

		//����
		{ 114,{ { 520.f * sinf(0.8f * MMPE_PI), 0.f, -500.f * cosf(0.2f * MMPE_PI) },{ MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 115,{ { 470.f * sinf(0.8f * MMPE_PI), 0.f, -440.f * cosf(0.2f * MMPE_PI) },{ MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 116,{ { 420.f * sinf(0.8f * MMPE_PI), 0.f, -380.f * cosf(0.2f * MMPE_PI) },{ MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },

		//����
		{ 117,{ { -520.f * sinf(0.8f * MMPE_PI), 0.f, -500.f * cosf(0.2f * MMPE_PI) },{ -MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 118,{ { -470.f * sinf(0.8f * MMPE_PI), 0.f, -440.f * cosf(0.2f * MMPE_PI) },{ -MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },
		{ 119,{ { -420.f * sinf(0.8f * MMPE_PI), 0.f, -380.f * cosf(0.2f * MMPE_PI) },{ -MMPE_PI / 3 },{ INSTALLED_OBJECT_TYPE::SmallWall } } },

		//�»� Building
		{ 120,{ { -BigWall_X1 - 20,    0.f, 240.f  * cosf(0.4f * MMPE_PI), 0.f		},{ MMPE_PI / 8 },		{ INSTALLED_OBJECT_TYPE::NormalBuilding } } },
		//�߻� Building
		{ 121,{ { 0.f,  0.f, 320.f,  0.f											},{ MMPE_PI / 2 },		{ INSTALLED_OBJECT_TYPE::NormalBuilding } } },
		//��� Building
		{ 122,{ { BigWall_X1 + 20,  0.f, 240.f * cosf(0.4f * MMPE_PI)               },{ -MMPE_PI / 8 },		{ INSTALLED_OBJECT_TYPE::NormalBuilding } } },
		//���� Building
		{ 123,{ { 370.f * sinf(0.8f * MMPE_PI), 0.f, -320.f * cosf(0.2f * MMPE_PI)  },{ MMPE_PI / 3 },      { INSTALLED_OBJECT_TYPE::NormalBuilding } } },
		//���� Building
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
	//����ƽ������Ʈ �浹 ��, ȸ���� ����ƽ������Ʈ���� Lookvector �� Rightvector�� ȸ���� ���� ���� �ٲ�����Ѵ�
	//�׷����� �浹ó���� �� Lookvector, Rightvector�� �̿��ϴµ�, �ٲ� ���� ����Ǽ� �浹ó���� �Ѵ� 

	pp = new PhysicsPoint();

	//---------------------������Ʈ�� ���� �⺻ �����Ͱ� ������Ʈ---------------------//

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

	//----------rot, degree���� �̿��� Lookvector, Rightvector, Upvector �� ������Ʈ----------//
	
	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookvector();
	UpdateUpvector();

	//---------��Ÿ ����ȿ�� ������ �ʱ�ȭ(�浹�ڽ�, ���ΰ��, �ٿ����, ����)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(5, 5, 5);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------��Ŷ ����� ���� StaticObject ��Ŷ�۽ſ뵥���� ������Ʈ---------------//
	UpdateDataForPacket();

}

CSmallWallObject::CSmallWallObject(unsigned int id)
{
	//����ƽ������Ʈ �浹 ��, ȸ���� ����ƽ������Ʈ���� Lookvector �� Rightvector�� ȸ���� ���� ���� �ٲ�����Ѵ�
	//�׷����� �浹ó���� �� Lookvector, Rightvector�� �̿��ϴµ�, �ٲ� ���� ����Ǽ� �浹ó���� �Ѵ� 

	pp = new PhysicsPoint();

	//---------------------������Ʈ�� ���� �⺻ �����Ͱ� ������Ʈ---------------------//

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

	//----------rot, degree���� �̿��� Lookvector, Rightvector, Upvector �� ������Ʈ----------//

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

	//---------��Ÿ ����ȿ�� ������ �ʱ�ȭ(�浹�ڽ�, ���ΰ��, �ٿ����, ����)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(20, 10, 5);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------��Ŷ ����� ���� StaticObject ��Ŷ�۽ſ뵥���� ������Ʈ---------------//
	UpdateDataForPacket();
}

CBigWallObject::CBigWallObject(unsigned int id)
{
	//����ƽ������Ʈ �浹 ��, ȸ���� ����ƽ������Ʈ���� Lookvector �� Rightvector�� ȸ���� ���� ���� �ٲ�����Ѵ�
	//�׷����� �浹ó���� �� Lookvector, Rightvector�� �̿��ϴµ�, �ٲ� ���� ����Ǽ� �浹ó���� �Ѵ� 

	pp = new PhysicsPoint();

	//---------------------������Ʈ�� ���� �⺻ �����Ͱ� ������Ʈ---------------------//

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


	//----------rot, degree���� �̿��� Lookvector, Rightvector, Upvector �� ������Ʈ----------//

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

	//---------��Ÿ ����ȿ�� ������ �ʱ�ȭ(�浹�ڽ�, ���ΰ��, �ٿ����, ����)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(350, 50, 5);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------��Ŷ ����� ���� StaticObject ��Ŷ�۽ſ뵥���� ������Ʈ---------------//
	UpdateDataForPacket();
}

CBuildingObject::CBuildingObject(unsigned int id)
{
	//����ƽ������Ʈ �浹 ��, ȸ���� ����ƽ������Ʈ���� Lookvector �� Rightvector�� ȸ���� ���� ���� �ٲ�����Ѵ�
	//�׷����� �浹ó���� �� Lookvector, Rightvector�� �̿��ϴµ�, �ٲ� ���� ����Ǽ� �浹ó���� �Ѵ� 

	pp = new PhysicsPoint();

	//---------------------������Ʈ�� ���� �⺻ �����Ͱ� ������Ʈ---------------------//

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

	//----------rot, degree���� �̿��� Lookvector, Rightvector, Upvector �� ������Ʈ----------//

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

	//---------��Ÿ ����ȿ�� ������ �ʱ�ȭ(�浹�ڽ�, ���ΰ��, �ٿ����, ����)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(15, 35, 15);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------��Ŷ ����� ���� StaticObject ��Ŷ�۽ſ뵥���� ������Ʈ---------------//
	UpdateDataForPacket();
}

CSecondFloorObject::CSecondFloorObject(unsigned int id)
{
	//����ƽ������Ʈ �浹 ��, ȸ���� ����ƽ������Ʈ���� Lookvector �� Rightvector�� ȸ���� ���� ���� �ٲ�����Ѵ�
	//�׷����� �浹ó���� �� Lookvector, Rightvector�� �̿��ϴµ�, �ٲ� ���� ����Ǽ� �浹ó���� �Ѵ� 

	pp = new PhysicsPoint();
	
	//---------------------������Ʈ�� ���� �⺻ �����Ͱ� ������Ʈ---------------------//

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

	//----------rot, degree���� �̿��� Lookvector, Rightvector, Upvector �� ������Ʈ----------//

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

	//---------��Ÿ ����ȿ�� ������ �ʱ�ȭ(�浹�ڽ�, ���ΰ��, �ٿ����, ����)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(90, 0.5, 90);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------��Ŷ ����� ���� StaticObject ��Ŷ�۽ſ뵥���� ������Ʈ---------------//
	UpdateDataForPacket();
}

CColumnObject::CColumnObject(unsigned int id)
{
	//����ƽ������Ʈ �浹 ��, ȸ���� ����ƽ������Ʈ���� Lookvector �� Rightvector�� ȸ���� ���� ���� �ٲ�����Ѵ�
	//�׷����� �浹ó���� �� Lookvector, Rightvector�� �̿��ϴµ�, �ٲ� ���� ����Ǽ� �浹ó���� �Ѵ� 

	pp = new PhysicsPoint();

	//---------------------������Ʈ�� ���� �⺻ �����Ͱ� ������Ʈ---------------------//

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

	//----------rot, degree���� �̿��� Lookvector, Rightvector, Upvector �� ������Ʈ----------//

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

	//---------��Ÿ ����ȿ�� ������ �ʱ�ȭ(�浹�ڽ�, ���ΰ��, �ٿ����, ����)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(15, 45, 15);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------��Ŷ ����� ���� StaticObject ��Ŷ�۽ſ뵥���� ������Ʈ---------------//
	UpdateDataForPacket();
}

CBrokenCartObject::CBrokenCartObject(unsigned int id)
{
	//����ƽ������Ʈ �浹 ��, ȸ���� ����ƽ������Ʈ���� Lookvector �� Rightvector�� ȸ���� ���� ���� �ٲ�����Ѵ�
	//�׷����� �浹ó���� �� Lookvector, Rightvector�� �̿��ϴµ�, �ٲ� ���� ����Ǽ� �浹ó���� �Ѵ� 

	pp = new PhysicsPoint();

	//---------------------������Ʈ�� ���� �⺻ �����Ͱ� ������Ʈ---------------------//

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

	//----------rot���� �̿��� Lookvector, Rightvector, Upvector �� ������Ʈ----------//

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

	//---------��Ÿ ����ȿ�� ������ �ʱ�ȭ(�浹�ڽ�, ���ΰ��, �ٿ����, ����)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(15, 8, 15);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------��Ŷ ����� ���� StaticObject ��Ŷ�۽ſ뵥���� ������Ʈ---------------//
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

	//�������� ���� �̿��� pos �� rot�� ������Ʈ
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


		//���� 8 ���� ���� �����´�.
		obj->GetRigidBody()->GetEightPoint(arr, obj->GetUpVector(), obj->GetLookVector(), obj->GetRightVector());
		vector<CollisionPoint> allpoint;
		vector<CollisionPoint> tempcollisionpoint;
		vector<CollisionPoint> contactpoint;

		for (int i = 0; i < 8; ++i)
		{
			float temppenetration = arr[i].x * Normal.x + arr[i].y * Normal.y + arr[i].z * Normal.z;
			//�浹 ���� �����Ѵ��� �����Ѵ�.
			CollisionPoint cp;
			cp.Pos = arr[i];
			cp.penetration = temppenetration;
			cp.pAxis = Normal;
			allpoint.push_back(cp);
		}

		// 5) ���� ó����
		//penetration�� ���� ������ ����
		sort(allpoint.begin(), allpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
		{
			return a.penetration < b.penetration;
		});

		// 6) ���� ó����.
		//������ ���ĵ� �༮�� 1�������� �浹�� �� �༮���� tempcollisionpoint�� �����Ѵ�.
		for (auto i : allpoint)
		{
			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0���ϸ� ������ �浹��
				tempcollisionpoint.push_back(i);

		}
		//���� ���� ���� �༮�� �������� �浹����Ʈ�� ���鼭 penetration�� ������ ����� ����� ������
		//�̳༮�� �о�ų� �������Ҷ� ���� �� �ִ� �浹���̴�. �� �͵��� �������� �浹���̴�.

		// 7) ���� ó����
		for (auto i : tempcollisionpoint)
		{
			//�����浹�� ù��° ���� - penetration���� ���ؼ� Epsilon�̸� ���� ���� �������� ����Ǵ� ���� �ִ´�.
			if (fabsf(tempcollisionpoint.front().penetration - i.penetration) <= 0.12)//���� 0.1������ ������ �浹 ���ɼ��� �ִٰ� �Ǵ��Ѵ�.
			{
				contactpoint.push_back(i);
			}

		}

		//------------- ���Ⱑ ���� �߿��� �б� -----------------//

		//------------- size �� 1/2/4/�׿� �϶� �� ������ ó�� ------------------//
		int size = contactpoint.size();
		if (size == 1)
		{
			//size�� 1�̸� ���ؾ��ұ�? �ٷ� ���� 2���� �ºε������ֵ��� ȸ�����Ѿ��Ѵ�.
			//�׷��� ���ؼ��� ������ ��ġ�� ��ݷ��� ���Ѵ�.
			//������ �̰� ��ȸ���� �Ͼ �� �ִ�.
			//���� ������ ȸ�������� �Ǹ� �� ������ �����Ѵ�. �ٸ� �ش� ��ݷ��� �ʹ� ũ�� �������� �����Ѵ�.

			//2������ - 1�������� �ؼ� 2�������� ���ϴ� ���͸� �����.
			//�̶� 2���� ���� ���ϴ� ������ Ư������ ���ϸ� �� ������ �����Ѵ�.

			//������ 2�������� ��������� �Ұ��ΰ� �̴�. ����Ǽ��� 2������. �����̳� �������� / �Ǵ� �밢�� ��.

			//���尡��� 2������.
			auto V1 = Float4Add(allpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			//�������� �밢���� ��ġ�� 2������
			auto sV1 = Float4Add(allpoint[3].Pos, contactpoint[0].Pos, false);
			sV1 = Float4Normalize(sV1);


			//���尡��� 2�������� ������ ���ϱ�
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
			//������ ����.  
			double theta = acos(tempdot);

			//�밢�� 2�������� ������ ���ϱ�
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
			//������ ����.  
			double stheta = acos(stempdot);

			//��ݷ��� ����. ��ݷ��� Ư�� �� �����϶��� ��������.

			CollisionPoint fp;//��ݷ��� ���� ����
			fp.Pos = XMFLOAT4(0, 0, 0, 0);
			fp.pAxis = Normal;

			fp.Pos = contactpoint[0].Pos;
			fp.penetration = contactpoint[0].penetration;
			float impurse = obj->GetRigidBody()->CalculateImpulse(fp, NULL, 1);

			//�ִ����޽��� ���Ѵ�.
			if (impurse > obj->GetRigidBody()->GetMaxImpurse())
				impurse = obj->GetRigidBody()->GetMaxImpurse();


			//�ּ� ���޽��� ���Ѵ�.
			if (impurse < obj->GetRigidBody()->GetMinImpurse())
				impurse = obj->GetRigidBody()->GetMinImpurse();




			//���� ���հ��� Ư������ ���ϸ� ������Ų��. 
			//�� �̰� double�� �ص� 0�̾ƴѵ� 0�̳����°�찡 �����.
			//���� 0�ϰ�� �׳� ��ݷ��� ���ؼ� ������ �����Ų��.

			if (abs(stheta) <= MMPE_PI / 20 && abs(stheta) != 0 && obj->GetRigidBody()->AmendTime <= 0)
			{
				//ȸ������ ���ϰ�..
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(sV1, sV2));
				mAxis = Float3Normalize(mAxis);
				//������ ��Ų��.
				AmendObject(mAxis, stheta, obj);

				//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
				//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
				obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

			}
			else if (abs(theta) <= MMPE_PI / 25 && abs(theta) != 0 && obj->GetRigidBody()->AmendTime <= 0)
			{
				//�뷫 5�� ���ϸ� ������Ŵ.

				//ȸ������ ����
				XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V2));
				mAxis = Float3Normalize(mAxis);

				//������ ��Ų��
				AmendObject(mAxis, theta, obj);

				//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
				//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
				obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

			}
			else //�ƴ϶�� ���� ��ݷ��� ������ ������ ���Ѵ�!
			{
				//���⿡ �Դٴ°��� ���̻� ������ ���� �ʰų� ������ ���� ���ʿ䰡 ���� ����.

				//Jm = J/M
				//���޽��� ������ ������.  ��. �Ϲ����� 1:1 ���迡�� 1.3: 0.3 ������ �д�.
				//������ 0.7�� �ҽǵ� �������� ġ��.
				if (obj->GetRigidBody()->AmendTime <= 0)
				{
					auto ratioImpurse = impurse * 0.3;

					auto Jm = Normal;
					Jm.x *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);
					Jm.y *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);
					Jm.z *= obj->GetRigidBody()->GetMass() * (impurse + ratioImpurse);


					//���ӵ� ���
					//W = ���� ���ӵ� + ((Q-P)Ximpurse)*InverseI
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

				//���� �ӵ��� ���ӵ��� ���� ������, ������ �κ� �ؼ�
				//���� �������� penetration(���� ����)��ŭ �ø��� �ȴ�.
				auto px = fabsf(contactpoint[0].penetration)*Normal.x;
				auto py = fabsf(contactpoint[0].penetration)*Normal.y;
				auto pz = fabsf(contactpoint[0].penetration)*Normal.z;

				obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
				obj->UpdateRigidCenterPos();
		
			}
		}
		else if (size == 2)
		{
			//�߶վ��� �� 2���� ������ �ؾ��Ѵ�.
			//������ �ؾ��ϴ��� �˻縦 �ϱ� ���� ����� ��ְ� ������ �ΰ��� ���� ���Ͱ� ������ 0�� �������� �˻��Ѵ�.
			auto V1 = Float4Add(contactpoint[1].Pos, contactpoint[0].Pos, false);
			V1 = Float4Normalize(V1);

			float Dot = V1.x * Normal.x + V1.y * Normal.y + V1.z * Normal.z;

			if (fabsf(Dot) <= MMPE_EPSILON)
				Dot = 0;

			//����� 0�̸� ������ �ʿ����.
			if (Dot == 0)
			{
				//���� 2���� �浹�� 4���� ���浹�� �ٲ� �� �ִ� ������ �� �� �ִ��� �˻��Ѵ�.
				//�׷��� ���ؼ� 3��° ��- ù��° �� �� �ؼ� 3��° ���� ���ϴ� ���͸� �����.
				//�� �� �̰� V1�� ������ 0 �� ������ ù��° ���� ����° ���� �밢���� �ƴ϶����̰�,
				//�ƴ϶�� �ι�° ���� ����°���� �̿��� ���͸� �������Ѵ�.
				auto V2 = Float4Add(allpoint[2].Pos, contactpoint[0].Pos, false);
				V2 = Float4Normalize(V2);
				float Dot2 = V1.x * V2.x + V1.y * V2.y + V1.z * V2.z;

				if (fabsf(Dot2) <= MMPE_EPSILON)
					Dot2 = 0;

				//ù��° ���� ����°���� ���� �ʴ°�� �ι�° ���� ����°������ �����.
				if (Dot2 != 0)
				{

					V2 = Float4Add(allpoint[2].Pos, contactpoint[1].Pos, false);
					V2 = Float4Normalize(V2);
				}
				//���� ���� �Դٴ°���, 4���� ���� ���� �� �ִ� �༮���� ã�� ���̴�.
				//�ٸ� �ٷ� �����ϸ� �ȵǰ�, �̰� ������� ���� ���� �� ���� �ȴ�.
				//�׷� �� ������ �����ΰ�??
				/*
				��  1)(V2)
				�� /
				��/
				�ѤѤ�2)(V3)

				���� V2�� �ٷ� 1) �̴�.
				�̰� ����� ��ְ� ������ 2) �� ���ͷ� ����ǵ�
				1)�� 2) ������ ��ٰ� ������ ���� �� 5~10 �� ���� �Ǹ� �׸�ŭ ȸ�� ��Ű�� ���̴�!
				�׷��� ���ؼ� 2)�� ���ؾ��Ѵ�. 2)�� ����� ����� A ���� V2�� B�� �ϰ� ProjAB �Ѱ���� V2�� ������ V2�� ���ȭ �ϸ� �ȴ�.
				*/

				float NdotV2 = Normal.x*V2.x + Normal.y*V2.y + Normal.z*V2.z;
				XMFLOAT3 ProjAB = Normal;
				ProjAB.x *= NdotV2;
				ProjAB.y *= NdotV2;
				ProjAB.z *= NdotV2;

				auto V3 = Float4Add(V2, XMFloat3to4(ProjAB), false);
				V3 = Float4Normalize(V3);
				//���� V2�� V3�� �ƴϱ� V2�� V3�� ���հ��� ���Ѵ��� V2���� V3�� �����ؼ� Axis�� ���Ѵ�.

				//���� ���հ����� ���Ѵ�.

				//�������� float���� �ߴµ�, 0�̾ƴϾ�� �ϴµ� 0�̳����°�찡 ����..


				auto tempdot = V2.x*V3.x + V2.y*V3.y + V2.z*V3.z;
				if (abs(tempdot) > 1)
				{
					if (tempdot > 0)
						tempdot = 1;
					else
						tempdot = -1;
				}
				//������ ����.  
				double theta = acos(tempdot);


				CollisionPoint fp;//��ݷ��� ���� ����
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

				//�ִ����޽��� ���Ѵ�.
				if (impurse > obj->GetRigidBody()->GetMaxImpurse())
					impurse = obj->GetRigidBody()->GetMaxImpurse();


				//�ּ� ���޽��� ���Ѵ�.
				if (impurse < obj->GetRigidBody()->GetMinImpurse())
					impurse = obj->GetRigidBody()->GetMinImpurse();


				//���� ���հ��� Ư������ ���ϸ� ������Ų��. 
				if (abs(theta) <= MMPE_PI / 25 && abs(theta) != 0 && abs(impurse) < obj->GetRigidBody()->GetMaxImpurse() && obj->GetRigidBody()->AmendTime <= 0)//�뷫 5�� ���ϸ� ������Ŵ.
				{
					//ȸ������ ���ϰ�
					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
					mAxis = Float3Normalize(mAxis);
					//������ ��Ų��.
					AmendObject(mAxis, theta, obj);

					//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
					//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
					obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);
					obj->GetRigidBody()->SetAccel(0, 0, 0);


				}
				else//�ƴ϶�� ���� ��ݷ��� ������ ������ ���Ѵ�!
				{
					//���⿡ �Դٴ°��� ���̻� ������ ���� �ʰų� ������ ���� ���ʿ䰡 ���� ����.

					//���� ���� �浹���� 2���̴� 2���� �浹���� ���� �� 2�� �����ָ� �浹���� �ϼ��̴�.
					//��ݷ��� ���Ŀ� �ٽ� ���ؾ߰�����, ����� 0,50,0 ������ ���ϰڴ�.

					//�ٵ� ���⼭ ����� ���� �����. �˴ٽ��� �밢�����·� ��Ȯ�ϰ� 45�� �������� ���ִ� ����Ǽ��� �����.
					//�̰��� ���ַ��� ���� �ش����� ���� ���ϴ����� �˻��ؾ��Ѵ�.

					//���� �� ����Ǽ��� �ذ��ؾ߰ڴ�. �˻縦 �Ϸ��� �¿� ��Ī������ ����ɰͰ���.
					//�׸��� ��Ī�̸� �����̵� �ƴϸ� ���������� ��ݷ��� �ణ ���ϴ� �ϸ� �ɰͰ���.

					//���� 2/3/4/5 �ε����� ���� ���ϸ鼭 �� ���̰� 0.0001���� ���̸� ���.
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

					//4���� ��� ���� ���̸� ������ �̷�°��̹Ƿ� ������ �μŹ�����.
					if (pass)
					{
						theta = -MMPE_PI / 18;
						XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V2, V3));
						mAxis = Float3Normalize(mAxis);
						//������ ��Ų��.
						AmendObject(mAxis, theta, obj);

						//�׸��� ��� ��Ų��. �ֳ��ϸ� �����̵Ǿ����� allpoint,tempcollisionpoint,contactpoint , penetration ��� �� �ٲ��� �ϱ� �����̴�.
						//��� �� �Ƹ� 2���� ����Ǽ��� �ִ�. �浹�� �Ͼ�ų�, �ƴϸ� ��¦ ���ְų�.. ��·�� �� �ذ� �ȴ�.
						obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);


						return;
					}

					//Jm = J/M

					//���޽��� ������ ������.  ��. �Ϲ����� 1:1 ���迡�� 1.3: 0.3 ������ �д�.

					if (obj->GetRigidBody()->AmendTime <= 0)
					{
						auto ratioImpurse = impurse * 0.3;

						auto Jm = Normal;



						Jm.x *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);
						Jm.y *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);
						Jm.z *= obj->GetRigidBody()->GetMass()*(impurse + ratioImpurse);



						//���ӵ� ���
						//W = ���� ���ӵ� + ((Q-P)Ximpurse)*InverseI
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
					//���� �ӵ��� ���ӵ��� ���� ������, ������ �κ� �ؼ�
					//���� �������� penetration(���� ����)��ŭ �ø��� �ȴ�.
					auto px = fabsf(contactpoint[0].penetration)*Normal.x;
					auto py = fabsf(contactpoint[0].penetration)*Normal.y;
					auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
					obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
					obj->UpdateRigidCenterPos();

				}

			}
			else//Dot�� ��� �߶վ��� �� �ϰ�� �� ��ְ� (�ι�°��-ù��°��) ���Ͱ� ���°�� 
			{
				XMFLOAT3 ProjAB = Normal;
				ProjAB.x *= Dot;
				ProjAB.y *= Dot;
				ProjAB.z *= Dot;

				//�����Ǿ����� V1
				auto V4 = Float4Add(V1, XMFloat3to4(ProjAB), false);
				V4 = Float4Normalize(V4);

				//������ ����.


				//�������� float���� �ߴµ�, 0�̾ƴϾ�� �ϴµ� 0�̳����°�찡 ����..


				auto tempdot = V1.x*V4.x + V1.y*V4.y + V1.z*V4.z;
				if (abs(tempdot) > 1)
				{
					if (tempdot > 0)
						tempdot = 1;
					else
						tempdot = -1;
				}
				//������ ����.  
				double theta = acos(tempdot);

				if (abs(theta) != 0 && obj->GetRigidBody()->AmendTime <= 0)
				{

					//ȸ������ ���ϰ�
					XMFLOAT3 mAxis = XMFloat4to3(Float4Cross(V1, V4));
					mAxis = Float3Normalize(mAxis);


					//������ ��Ų��.
					//���� ��ġ�� size = 4 �ΰ����� �̵��� //velocity = 0, 0, 0
					AmendObject(mAxis, theta, obj);
					obj->GetRigidBody()->SetAngularVelocity(0, 0, 0);

				}

			}
		}
		else if (size == 4)
		{
			//�ӽ��������� �׳� �ϴ� penetration�� �ӵ��� ��������.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
			obj->UpdateRigidCenterPos();

			//���� ������� ���� �ӵ��� y�� �ݰ��Ǿ�� �Ѵ�. ������ ź������� ������.. �׳� ���� ���ҽ�Ų�� ��ȣ�� -�� ����.
			auto d = obj->GetRigidBody()->GetVelocity();
			d.y = -0.01 * d.y;
			obj->GetRigidBody()->SetVelocity(d);
			obj->GetRigidBody()->SetAccel(0, 0, 0);
		}
		else if (size != 0) //�׿�
		{
			//�ӽ��������� �׳� �ϴ� penetration�� �ӵ��� ��������.
			auto px = fabsf(contactpoint[0].penetration)*Normal.x;
			auto py = fabsf(contactpoint[0].penetration)*Normal.y;
			auto pz = fabsf(contactpoint[0].penetration)*Normal.z;
			obj->SetCenterPos3f(m_pos4f.x + px, m_pos4f.y + py, m_pos4f.z + pz);
			obj->UpdateRigidCenterPos();

			//���� ������� ���� �ӵ��� y�� �ݰ��Ǿ�� �Ѵ�. ������ ź������� ������.. �׳� ���� ���ҽ�Ų�� ��ȣ�� -�� ����.
			auto d = obj->GetRigidBody()->GetVelocity();
			d.y = -0.01 * d.y;
			obj->GetRigidBody()->SetVelocity(d);
			obj->GetRigidBody()->SetAccel(0, 0, 0);
		}

		//==================================== �浹 �� ������ ��Ȳ�� �����ϱ� ���� ó���� ============================================//

		//�ʹ� ������ġ�� ������ �ӵ��� 0����
		if (obj->GetCenterPos4f().y < -200)
			obj->GetRigidBody()->SetVelocity(0, 0, 0);


		// ���� ������ �ϰų� �Ѵ��� ������ �κ��� ���ذ�.

		XMFLOAT4 tarr[8];
		obj->GetRigidBody()->GetEightPoint(tarr, obj->GetUpVector(), obj->GetLookVector(), obj->GetRightVector());//���� 8 ���� ���� �����´�.
		std::vector<CollisionPoint> tallpoint;
		std::vector<CollisionPoint> ttempcollisionpoint;
		std::vector<CollisionPoint> tcontactpoint;

		for (int i = 0; i < 8; i++)
		{
			float temppenetration = tarr[i].x*Normal.x + tarr[i].y*Normal.y + tarr[i].z*Normal.z;
			//�浹 ���� �����Ѵ��� �����Ѵ�.
			CollisionPoint cp;
			cp.Pos = tarr[i];
			cp.penetration = temppenetration;
			cp.pAxis = Normal;
			tallpoint.push_back(cp);
		}

		//penetration�� ���� ������ ����
		sort(tallpoint.begin(), tallpoint.end(), [](CollisionPoint& a, CollisionPoint& b)
		{
			return a.penetration < b.penetration;
		});

		//������ ���ĵ� �༮�� 1�������� �浹�� �� �༮���� tempcollisionpoint�� �����Ѵ�.
		for (auto i : tallpoint)
		{
			float temppenetration = i.Pos.x*Normal.x + i.Pos.y*Normal.y + i.Pos.z*Normal.z;
			if (temppenetration < 0)//0���ϸ� ������ �浹��
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

				//�浹 ó��. ��ݷ��� ���ϰ� ��ħ�κ��ؼ�
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

	//---------------------������Ʈ�� ���� �⺻ �����Ͱ� ������Ʈ---------------------//

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

	//----------rot���� �̿��� Lookvector, Rightvector, Upvector �� ������Ʈ----------//

	m_OffLookvector = XMFLOAT3(0, 0, 1);
	m_OffRightvector = XMFLOAT3(1, 0, 0);

	UpdateLookvector();
	UpdateUpvector();

	//---------��Ÿ ����ȿ�� ������ �ʱ�ȭ(�浹�ڽ�, ���ΰ��, �ٿ����, ����)---------//

	xmf4_pos = { m_pos4f.x, m_pos4f.y, m_pos4f.z, m_pos4f.w };
	pp->SetPosition(&xmf4_pos);
	pp->SetHalfBox(10, 5, 10);
	pp->SetDamping(0.5f);
	pp->SetBounce(false);
	pp->SetMass(INFINITY);

	//------------��Ŷ ����� ���� StaticObject ��Ŷ�۽ſ뵥���� ������Ʈ---------------//
	//1. ���� ������
	m_stc_mvobjdata.id = m_id;
	m_stc_mvobjdata.texture_color = m_selectedColor;
	m_stc_mvobjdata.len = m_len;
	m_stc_mvobjdata.n = m_n;
	m_stc_mvobjdata.create_first = m_createfirst;

	//2. ������Ʈ�Ǵ� ������
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
