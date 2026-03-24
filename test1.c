#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define ROW 2
#define COL 3
#define MAX_CUPSIZE 6

typedef struct icecream {
    int number;
    char name[15];
    int stock;
}Icecream;

typedef struct customer {
    int cup_size;
    int order[MAX_CUPSIZE];
} Customer;

Icecream*** icecream_tb;
int reverse;

Icecream*** create_icecream_tb(int row, int col)
{
    int i;
    Icecream*** table;
    table = (Icecream***)malloc(sizeof(Icecream**) * row);
    for (i = 0; i < row; i++)
    {
        table[i] = (Icecream**)malloc(sizeof(Icecream*) * col);
    }
    return table;
}

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
    int price_of_cupsize[MAX_CUPSIZE];
} Owner;

void table_init()
{
    int i, j;
    int ice_num = 0;
    icecream_tb = create_icecream_tb(ROW, COL);
    reverse = 0;
    srand((unsigned int)time(NULL));
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            Icecream* ice = create_icecream();
            char name[15];
            ice->number = ice_num++;
            ice->stock = rand() % 5;
            printf("진열할 아이스크림명을 입력하세요:");
            scanf("%s", name);
            strcpy(ice->name, name);
            icecream_tb[i][j] = ice;
        }
    }
}

void transposed_icecream_tb()
{
    int row = ROW;
    int col = COL;
    int i, j;
    if (reverse == 1)
    {
        row = COL;
        col = ROW;
    }
    Icecream*** table = create_icecream_tb(col, row);
    for (i = 0; i < row; i++)
    {
        for (j = 0; j < col; j++)
        {
            table[j][i] = icecream_tb[i][j];
        }
    }
    free(icecream_tb);
    icecream_tb = table;
    for (i = 0; i < col; i++)
    {
        for (j = 0; j < row; j++)
            printf("[(%d)%15s]", icecream_tb[i][j]->number, icecream_tb[i][j]->name);
        printf("\n");
    }
    reverse = (reverse + 1) % 2;
}


void owner_init(Owner* ice_owner) {
    int i;
    for (i = 0; i < MAX_CUPSIZE; i++)
    {
        ice_owner->price_of_cupsize[i] = (i + 1) * 100;
    }
    ice_owner->total_income = 0;
}

Customer* get_order()
{
    Customer* cus = create_customer();
    int i;
    int row = ROW;
    int col = COL;

    Sleep(1000);
    printf("사장: 고객님 주문받겠습니다.\n");
    cus->cup_size = (rand() % MAX_CUPSIZE) + 1;

    printf("고객: 저 cupsize %d으로 ", cus->cup_size);

    for (i = 0; i < cus->cup_size; i++)
    {
        int ice_num = rand() % (ROW * COL);
        if (reverse == 0)
        {
            row = ice_num / COL;
            col = ice_num % COL;
        }
        else
        {
            row = ice_num % COL;
            col = ice_num / COL;
        }
        cus->order[i] = ice_num;
        printf("No. %d인 %s, ", icecream_tb[row][col]->number, icecream_tb[row][col]->name);
    }

    printf("주세요\n\n");

    return cus;
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

    table_init();

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            printf("[(%d)%15s]", icecream_tb[i][j]->number, icecream_tb[i][j]->name);
        }
        printf("\n");
    }
    printf("\n");

    Customer* c1 = get_order();
    printf("단속원떴다!!\n\n");
    transposed_icecream_tb();
    printf("\n");

    Customer* c2 = get_order();
    printf("단속원떴다!!\n\n");
    transposed_icecream_tb();
    printf("\n");

    Owner ice_owner;
    owner_init(&ice_owner);

    cal_sales(&ice_owner, c1->cup_size);
    cal_sales(&ice_owner, c2->cup_size);

    printf("Total income :%d\n", ice_owner.total_income);

    free(c1);
    free(c2);

    return 0;
}
