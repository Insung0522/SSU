#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NAME_SIZE 5
#define DATE_SIZE 11
#define FUNC_NUM 5
typedef struct food{
	int flag;
	char name[NAME_SIZE];
	union {
		int qnt;//quantity
		char amt[4];//amount
	}remain;
	char str_d[DATE_SIZE];
	char exp_d[DATE_SIZE];
}food;

typedef struct node{
	food item;
	struct node *next;
}node_t;

enum key_in {list = 1, search, store, use, exit_pg};

node_t *file_read();//파일로부터 데이터를 읽어와 구조체에 저장하는 함수
void file_write(node_t *pnode);//구조체의 데이터를 파일에 저장하는 함수
int menu();
node_t *print_list(node_t *pnode);//리스트 출력
node_t *search_item(node_t *pnode);//특정 아이템 탐색
node_t *store_item(node_t *pnode);//리스트에 새 데이터 추가
node_t *use_item(node_t *pnode);//리스트 데이터 변경 또는 제거
node_t *exit_prog(node_t *pnode);//파일 저장 및 할당된  동적 메모리 해제

void main(){
	enum key_in key_buff = 0;
	//기능 실행을 위한 함수포인터
	node_t *(*pfunc[FUNC_NUM])(node_t *pnode) =
		{print_list, search_item, store_item, use_item, exit_prog};

	node_t *list_head;
	list_head = file_read();
	if(list_head == NULL) return;//파일 오픈 실패시 프로그램 종료
	
	while(key_buff != exit_pg){
		key_buff = menu();
		list_head = pfunc[key_buff-1](list_head);
	}
}
//파일 읽기
node_t *file_read(){
	node_t *pnode;
	node_t *next_head = NULL;

	FILE *fp = fopen("food.dat", "r");

	if(fp == NULL){//예외처리
		printf("Failed to open file for read\n");
		return NULL;
	}

	while(!feof(fp)){
		pnode = (node_t*)malloc(sizeof(node_t));

		fscanf(fp,"%d", &pnode->item.flag);
		if(pnode->item.flag == 0)//remain이 quantity일 경우
			fscanf(fp,"name:%s remain:%d store:%s expiry:%s\n",
				pnode->item.name, &pnode->item.remain.qnt, 
				pnode->item.str_d, pnode->item.exp_d);
		else//remain이 amount일 경우
			fscanf(fp,"name:%s remain:%s store:%s expiry:%s\n",
				pnode->item.name, pnode->item.remain.amt, 
				pnode->item.str_d, pnode->item.exp_d);

		pnode->next = next_head;
		next_head = pnode;
	}
	fclose(fp);
	return pnode;
}

//파일 쓰기
void file_write(node_t *pnode){
	FILE *fp = fopen("food.dat", "w");

	if(fp == NULL){
		printf("Failed to open file for write\n");
		return ;
	}

	while(pnode != NULL){
		if(pnode->item.flag == 0)//quantity
			fprintf(fp,"%d name:%s remain:%d store:%s expiry:%s\n",
				pnode->item.flag, pnode->item.name, pnode->item.remain.qnt, 
				pnode->item.str_d, pnode->item.exp_d);
		else//amount
			fprintf(fp,"%d name:%s remain:%s store:%s expiry:%s\n",
				pnode->item.flag, pnode->item.name, pnode->item.remain.amt, 
				pnode->item.str_d, pnode->item.exp_d);

		pnode = pnode->next;
	}
	
	fclose(fp);
	return ;
}

int menu(){
	int buff;
	int flag = 1;

	while(flag){
		printf("===================================\n");
		printf("(1)list up (2)store (3)use (4)exit\n");
		printf("===================================\n");
		printf(":");
		scanf("%d", &buff);

		if(!(buff > 0 && buff < 5)){
			printf("Wrong command.\n");
			flag = 1;
		}
		else
			flag = 0;
	}

	return buff;
}

node_t *print_list(node_t *list_head){
	node_t *pnode;
	pnode = list_head;

	printf("===================================\n");
	printf("name     remain     store_date  expiry_date\n");

	while(pnode != NULL){//다음 노드가 없을 때까지 반복
		if(pnode->item.flag == 0){//quantity
			printf("%-10s %-4d %s %s\n",
				pnode->item.name, pnode->item.remain.qnt,
				pnode->item.str_d, pnode->item.exp_d);
		}
		else{//amount
			printf("%-10s %-4s %s %s\n",
				pnode->item.name, pnode->item.remain.amt,
				pnode->item.str_d, pnode->item.exp_d);
		}
		pnode = pnode->next;
	}
	return list_head;
}

node_t *search_item(node_t *list_head){
	return list_head;
}

node_t *store_item(node_t *list_head){
	struct tm *date;
	const time_t t = time(NULL);
	node_t *pnode;
	pnode = (node_t*)malloc(sizeof(node_t));

	printf("itme name: ");//name
	fgets(pnode->item.name, NAME_SIZE, stdin);

	printf("셀 수 있나요?(0 or 1): ");//remain
	pnode->item.flag = getchar();//버퍼 비우기 위해 두 번 사용
	pnode->item.flag = getchar();
	if(!pnode->item.flag){
		printf("quantity(integer): ");
		scanf("%d", &pnode->item.remain.qnt);
	}
	else{
		printf("amount(): ");
		fgets(pnode->item.remain.amt, DATE_SIZE, stdin);
	}

	printf("expiry date(xxxx.xx.xx): ");//expriy date
	fgets(pnode->item.exp_d, DATE_SIZE, stdin);
	
	date = localtime(&t);//store date
		sprintf(pnode->item.str_d,"%d.%d.%d",
			date->tm_year + 1900, date->tm_mon +1, date->tm_mday);

	pnode->next = list_head;
	list_head = pnode;
	return pnode;
}

node_t *use_item(node_t *list_head){
	return list_head;
}
node_t *exit_prog(node_t *pnode){
	file_write(pnode);
	node_t *tmp;
	while(pnode != NULL){
		tmp = pnode;
		pnode = pnode->next;
		free(tmp);
	}
	return NULL;
}

