#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // Ccompiler <unistd.h>

#define ROW 2
#define COL 3
#define MAX_CUPSIZE 3
#define MAX_CUSTOMER_SIZE 5
#define MAX_DELIVERY_COUNT 5

typedef struct icecream {
    int number;
    char name[15];
    int stock;
} Icecream;

typedef struct stack {
    int* icecream_number;
    int capacity;
    int top;
} Stack;

typedef struct customer {
    int cup_size;
    int order[MAX_CUPSIZE];
    int vip;
    int onlyhome;
    int order_num;
    Stack* icecream_cup;
} Customer;

typedef struct queue {
    int front, rear;
    Customer* cus[MAX_CUSTOMER_SIZE];
} Queue;

typedef struct ListNode {
    Customer* cus;
    struct ListNode* link;
}ListNode;;

typedef struct rider {
    int count;
    ListNode* tail;
} Rider;

Icecream*** icecream_tb;
int reverse;
int global_order_num = 1;

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

void init_cup(Stack* icecream_cup)
{
    icecream_cup->capacity = MAX_CUPSIZE;
    icecream_cup->top = -1;
    icecream_cup->icecream_number = (int*)malloc(icecream_cup->capacity * sizeof(int));
}

Customer* create_customer()
{
    Customer* cus = (Customer*)malloc(sizeof(Customer));
    memset(cus, 0, sizeof(Customer));
    cus->icecream_cup = (Stack*)malloc(sizeof(Stack));
    init_cup(cus->icecream_cup);
    return cus;
}

typedef struct owner {
    int total_income;
    //price per cup_size
    int price_of_cupsize[MAX_CUPSIZE];
    Queue* q;
    ListNode* tail;
} Owner;

int is_full(Stack* icecream_cup)
{
    return(icecream_cup->top == (icecream_cup->capacity - 1));
}

void push(int choice, Stack* icecream_cup)
{
    if (is_full(icecream_cup))
    {
        icecream_cup->capacity *= 2;
        icecream_cup->icecream_number = (int*)realloc(icecream_cup->icecream_number, icecream_cup->capacity * sizeof(int));
    }
    icecream_cup->icecream_number[++icecream_cup->top] = choice;
}

int is_empty(Stack* icecream_cup)
{
    return (icecream_cup->top == -1);
}

int pop(Stack* icecream_cup)
{
    if (is_empty(icecream_cup)) return -1;
    else return icecream_cup->icecream_number[icecream_cup->top--];
}

void init_queue(Queue* q)
{
    q->front = q->rear = 0;
}

int is_empty_queue(Queue* q)
{
    return (q->front == q->rear);
}

int is_full_queue(Queue* q)
{
    return ((q->rear + 1) % MAX_CUSTOMER_SIZE == q->front);
}

int enqueue(Queue* q, Customer* cus)
{
    if (is_full_queue(q))
        return -1;
    q->rear = (q->rear + 1) % MAX_CUSTOMER_SIZE;
    q->cus[q->rear] = cus;
    return 0;
}

void rider_init(Rider* rider)
{
    rider->count = 0;
    rider->tail = NULL;
}

Customer* dequeue(Queue* q)
{
    if (is_empty_queue(q))
        return NULL;
    q->front = (q->front + 1) % MAX_CUSTOMER_SIZE;
    return q->cus[q->front];
}

ListNode* insert_last(ListNode* tail, Customer* cus)
{
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->cus = cus;

    if (tail == NULL)
    {
        tail = node;
        node->link = tail;
    }
    else
    {
        node->link = tail->link;
        tail->link = node;
        tail = node;
    }
    return tail;
}

ListNode* insert_first(ListNode* tail, Customer* cus)
{
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));
    node->cus = cus;

    if (tail == NULL)
    {
        tail = node;
        node->link = tail;
    }
    else
    {
        node->link = tail->link;
        tail->link = node;
    }
    return tail;
}

Customer* delete_first(ListNode** ptail)
{
    ListNode* removed;
    Customer* cus;

    if (*ptail == NULL)
    {
        return NULL;
    }
    else if (*ptail == (*ptail)->link)
    {
        removed = *ptail;
        *ptail = NULL;
    }
    else
    {
        removed = (*ptail)->link;
        (*ptail)->link = removed->link;
    }

    cus = removed->cus;
    free(removed);
    return cus;
}

Customer* delete(ListNode** ptail, ListNode* pre)
{
    ListNode* removed;
    Customer* cus;

    if (*ptail == (*ptail)->link)
    {
        removed = *ptail;
        *ptail = NULL;
    }
    else
    {
        if (pre->link == *ptail) *ptail = pre;
        removed = pre->link;
        pre->link = removed->link;
    }
    cus = removed->cus;
    free(removed);
    return cus;
}

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
    for (i = 0; i < ROW; i++)
        free(icecream_tb[i]);
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

    Queue* q = (Queue*)malloc(sizeof(Queue));
    memset(q, 0, sizeof(Queue));
    init_queue(q);
    ice_owner->q = q;
    ice_owner->tail = NULL;
}

int delivery_order(Customer* cus)
{
    int i, ice_num;
    int penalty = 0;

    for (i = cus->cup_size - 1; i >= 0; i--)
    {
        ice_num = pop(cus->icecream_cup);
        if (ice_num == -1)
        {
            printf("고객:왜 주문한 아이스크림을 다 안주는거야!!!\n");
            penalty += 100;
        }
        else
        {
            if (ice_num == cus->order[i])
            {
                printf("고객:주문한 순서대로 쌓아줬네.\n");
            }
            else
            {
                printf("고객:내가 주문한 아이스크림이 아니잖아!!!!!\n");
                penalty += 50;
            }
        }
    }

    if ((cus->icecream_cup->top) > -1)
    {
        penalty += ((cus->icecream_cup->top) + 1);
        printf("고객:왜 내가 시킨것보다 더 주는거지?\n");
    }
    return penalty;
}

void cal_sales(Owner* ice_owner, int cupsize, int penalty)
{
    int income = ice_owner->price_of_cupsize[cupsize - 1];
    income -= penalty;
    ice_owner->total_income = ice_owner->total_income + income;

    printf("나:이번주문으로 %d원 총 %d원 벌었다\n", income, ice_owner->total_income);
}

int prepare_order(Owner* ice_owner)
{
    Customer* cus = dequeue(ice_owner->q);
    if (cus == NULL)
    {
        return -1;
    }
    Stack* icecream_cup = cus->icecream_cup;
    free(icecream_cup->icecream_number);
    icecream_cup->icecream_number = NULL;
    init_cup(icecream_cup);

    printf("start prepare order\n");

    int cupsize = -1;
    printf("나:고객님이 고른 컵 사이즈가 뭐였지?\n");
    scanf("%d", &cupsize);

    int i, choice;
    for (i = 0; i < cupsize; i++)
    {
        printf("나:몇번 아이스크림을 쌓아야하지?\n");
        scanf("%d", &choice);
        push(choice, icecream_cup);
    }

    ice_owner->tail = insert_last(ice_owner->tail, cus);
    //int penalty = delivery_order(cus);
    //cal_sales(ice_owner, cus->cup_size, penalty);
    return 0;
}

int get_order(Owner* ice_owner)
{
    if (is_full_queue(ice_owner->q)) return -1;

    Customer* cus = create_customer();
    cus->order_num = global_order_num++;
    int i;
    int row = ROW;
    int col = COL;

    Sleep(1000); // Ccompiler sleep(1);
    printf("-----------------------------------------------------\n");
    printf("사장: 고객님 주문받겠습니다.\n");
    cus->cup_size = (rand() % MAX_CUPSIZE) + 1;
    cus->vip = (rand() % 2);
    Sleep(1000);
    cus->onlyhome = (rand() % 2);

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
        printf("%s, ", icecream_tb[row][col]->name);
    }

    printf("순서로 쌓아주세요.\n\n");
    enqueue(ice_owner->q, cus);

    return 0;
}

ListNode* search_list(ListNode* tail)
{
    ListNode* prev = tail;

    do
    {
        if (prev == NULL) return NULL;
        if (prev->link->cus->onlyhome == 0) return prev;
        prev = prev->link;
    } while (prev != tail);
    return NULL;
}

int rider_pickup(Owner* ice_owner, Rider* rider)
{
    Customer* cus = delete_first(&(ice_owner->tail));
    ListNode* prev;
    if (cus == NULL) return -1;

    rider->tail = insert_last(rider->tail, cus);
    rider->count++;
    printf("------------------------------------------------------------------------------\n");
    printf("배달시작!\n");
    if (cus->onlyhome == 0)
    {
        printf("앗! 한집배달이 아니네!\n");
        while (rider->count < MAX_DELIVERY_COUNT)
        {
            prev = search_list(ice_owner->tail);
            if (prev != NULL)
            {
                cus = delete(&(ice_owner->tail), prev);
                rider->tail = insert_last(rider->tail, cus);
                rider->count++;
                printf("하나 더 픽업 진행!\n");
            }
            else break;
        }
    }
    return 1;
}

void delivery__pickup_order(Owner* ice_owner, Rider* rider)
{
    int i, ice_num;
    Customer* cus;

    while (rider->count > 0)
    {
        int penalty = 0;
        cus = delete_first(&(rider->tail));

        printf("------------------------------------------------------------------------------\n");
        printf("[vip %d onlyhome %d]주문번호 %d 고객님 아이스크림 배달왔습니다.\n", cus->vip, cus->onlyhome, cus->order_num);

        for (i = cus->cup_size - 1; i >= 0; i--)
        {
            ice_num = pop(cus->icecream_cup);
            if (ice_num == -1)
            {
                printf("고객:왜 주문한 아이스크림을 다 안주는거야!!\n");
                penalty++;
            }
            else
            {
                if (ice_num == cus->order[i]) { printf("고객:감사합니다!\n");}
                else
                {
                    printf("고객:내가 주문한 아이스크림이 아니잖아!!!\n");
                    penalty++;
                }
            }
        }

        if ((cus->icecream_cup->top) > -1)
        {
            penalty += ((cus->icecream_cup->top) + 1);
            printf("고객:왜 내가 시킨 것보다 더 준거지?\n");
        }
        cal_sales(ice_owner, cus->cup_size, penalty);
        rider->count--;
    }
}

/* 미완성
void stage(Owner* ice_owner)
{
    int i = 0;
    srand((unsigned int)time(NULL));

    for (i = 0; i < (rand() % MAX_CUSTOMER_SIZE) + 1; i++)
    {
        Sleep(1000);
        if (get_order(&ice_owner) == -1)
        {
            printf("더이상 주문을 받을 수 없어요. 다시 찾아와주세요.\n");
            break;
        }
        get_order(&ice_owner);
    }
    for (i = 0; i < MAX_CUSTOMER_SIZE; i++)
    {
        if (prepare_order(&ice_owner) == -1)
        {
            printf("들어온 주문이 더 이상 없어요.\n");
            break;
        }
    }
} */

int main(void)
{
    int i = 0;
    int j = 0;

    srand((unsigned int)time(NULL));

    int order_count = 0;
    Owner ice_owner;
    owner_init(&ice_owner);
    table_init();
    Rider rider;
    rider_init(&rider);

    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            printf("[(%d)%15s]", icecream_tb[i][j]->number, icecream_tb[i][j]->name);
        }
        printf("\n");
    }
    printf("\n");

    // 스테이지 구성
    for (i = 0; i < (rand() % MAX_CUSTOMER_SIZE) + 1; i++)
    {
        Sleep(1000);
        if (get_order(&ice_owner) == -1)
        {
            printf("더이상 주문을 받을 수 없어요. 다시 찾아와주세요.\n");
            break;
        }
    }
    for (i = 0; i < MAX_CUSTOMER_SIZE; i++)
    {
        if (prepare_order(&ice_owner) == -1)
        {
            printf("들어온 주문이 더 이상 없어요.\n");
            break;
        }
    }
    for (i = 0; i < MAX_CUSTOMER_SIZE; i++)
    {
        if (rider_pickup(&ice_owner, &rider) == -1)
        {
            printf("더이상 배달할 주문이 없어요.\n");
            break;
        }
        else { delivery__pickup_order(&ice_owner, &rider); }
    }
    // 스테이지 구성

    printf("단속원 떴다!\n");
    transposed_icecream_tb();

    printf("Total income :%d\n", ice_owner.total_income);

    return 0;
}
