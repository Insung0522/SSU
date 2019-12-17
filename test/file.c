#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 5
#define DATE_SIZE 11

enum order {exp_asc = 1, exp_desc, str_asc, str_desc};

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

node_t *file_read();//파일로부터 데이터를 읽어와 구조체에 저장하는 함수
void file_write(node_t *pnode);//구조체의 데이터를 파일에 저장하는 함수
node_t *sort(node_t *new_node, node_t *list_head, int order);//내림차순 정렬

//파일 읽기
node_t *file_read(){
	node_t *new_node;
	node_t *list_head = NULL;
	enum order odr;

	FILE *fp = fopen("food.dat", "r");

	if(fp == NULL){//예외처리
		printf("Failed to open file for read\n");
		return NULL;
	}

	while(!feof(fp)){
		//새 노드 생성
		new_node = (node_t*)malloc(sizeof(node_t));

		fscanf(fp,"%d", &new_node->item.flag);
		if(new_node->item.flag == 0)//remain이 quantity일 경우
			fscanf(fp," name:%s remain:%d store:%s expiry:%s\n",
				new_node->item.name, &new_node->item.remain.qnt, 
				new_node->item.str_d, new_node->item.exp_d);
		else//remain이 amount일 경우
			fscanf(fp," name:%s remain:%s store:%s expiry:%s\n",
				new_node->item.name, new_node->item.remain.amt, 
				new_node->item.str_d, new_node->item.exp_d);
		
		list_head = sort(new_node, list_head, odr = exp_asc);
	}
	fclose(fp);
	return list_head;
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
//정렬
node_t *sort(node_t *new_node, node_t *list_head, int order){
	node_t *next_p;
	node_t *prev_p;
	int flag;
	
	//삽입 위치 탐색
	next_p = list_head;
	prev_p = NULL;
	
	while(next_p){
		switch(order){
			case exp_asc://유통기한 오름차순
				flag = strcmp(new_node->item.exp_d, next_p->item.exp_d);
				if(flag < 0)//기준 노드가 유통기한이 더 긺
					break;
				break;
			case exp_desc://유통기한 내림차순
				flag = strcmp(new_node->item.exp_d, next_p->item.exp_d);
				if(flag > 0)//기준 노드가 유통기한이 더 짧음
					break;
				break;
			case str_asc://저장날짜 오름차순
				flag = strcmp(new_node->item.str_d, next_p->item.str_d);
				if(flag < 0)//기준 노드가 저장한 날이 더 빠를 때
					break;
				break;
			case str_desc://저장날짜 내림차순
				flag = strcmp(new_node->item.exp_d, next_p->item.exp_d);
				if(flag > 0)//기준 노드가 저장한 날이 더 늦을 때
					break;
				break;
			default:
				printf("wrong ordering option\n");
				return list_head;
		}
		prev_p = next_p;
		next_p = next_p->next;
	}
	//노드 삽입
	new_node->next = next_p;
	if(prev_p)
		prev_p->next = new_node;
	else
		list_head = new_node;

	return list_head;
}
