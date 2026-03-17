#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define ROW 2
#define COL 2
#define MAX_CUPSIZE 5

typedef struct icecream {
	int number;
	char name[15];
	int stock;
}Icecream;

typedef struct customer {
	int cup_size;
	int order[5];
} Customer;

Icecream* icecream_tb[ROW][COL];

Icecream* create_icecream()
{
	Icecream* ice = NULL;
	ice = (Icecream*)malloc(sizeof(Icecream));
	memset(ice, 0, sizeof(Icecream));
	return ice;
}

Customer* create_customer()
{
	Customer* cus = (Customer*)malloc(sizeof(Customer));
	memset(cus, 0, sizeof(Customer));
	return cus;
}

typedef struct owner {
	int total_income;
	//price per cup_size
	int price_of_cupsize[5];
} Owner;

void table_init(int num_of_icecream)
{
	int i, j;
	int ice_num = 0;
	char name[15];
	srand((unsigned int)time(NULL));
	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			Icecream* ice = create_icecream();
			ice->number = ice_num++;
			ice->stock = rand() % 5;
			printf("진열할 아이스크림명을 입력하세요:");
			scanf("%s", name);
			strcpy(ice->name, name);
			icecream_tb[i][j] = ice;
		}
	}
}

void owner_init(Owner* ice_owner) {
	int i;
	for (i = 0; i < 5; i++)
	{
		ice_owner->price_of_cupsize[i] = (i + 1) * 100;
	}
	ice_owner->total_income = 0;
}

Customer* get_order()
{
	Customer* cus = create_customer();
	int i;

	Sleep(1000);
	printf("사장: 고객님 주문받겠습니다.\n");
	cus->cup_size = (rand() % MAX_CUPSIZE) + 1;

	printf("고객: 저 cupsize %d으로 ", cus->cup_size);

	for (i = 0; i < cus->cup_size; i++) {
		int ice_num = rand() % (ROW * COL);
		int row = ice_num / COL;
		int col = ice_num % COL;

		cus->order[i] = ice_num;

		printf("No. %d번 %s, ",
			icecream_tb[row][col]->number,
			icecream_tb[row][col]->name);
	}

	printf("주세요\n\n");

	return cus;   // ⭐ 핵심
}

void cal_sales(Owner* ice_owner, int cupsize)
{
	int income = ice_owner->price_of_cupsize[cupsize - 1];
	ice_owner->total_income += income;
}

int main(void)
{
	int i = 0;
	int j = 0;

	srand((unsigned int)time(NULL));

	table_init(ROW * COL);


	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 2; j++)
		{
			printf("[%d: %15s]", icecream_tb[i][j]->number, icecream_tb[i][j]->name);
		}
		printf("\n");
	}

	Customer* c1 = get_order();
	Customer* c2 = get_order();

	Owner ice_owner;
	owner_init(&ice_owner);

	cal_sales(&ice_owner, c1->cup_size);
	cal_sales(&ice_owner, c2->cup_size);

	printf("Total income :%d\n", ice_owner.total_income);

	free(c1);
	free(c2);
	
	return 0;
}