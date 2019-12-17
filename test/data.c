#include "file.c"
#include <time.h>
enum key_in {list = 1, search, store, use, exit_pg};

int menu();
node_t *print_list(node_t *pnode);//리스트 출력
node_t *search_item(node_t *pnode);//특정 아이템 탐색
node_t *store_item(node_t *pnode);//리스트에 새 데이터 추가
node_t *use_item(node_t *pnode);//리스트 데이터 변경 또는 제거
node_t *exit_prog(node_t *pnode);//파일 저장 및 할당된  동적 메모리 해제
node_t *sort_list(node_t *pnode);//리스트 정렬
void print_node(node_t *pnode);//노드 하나 출력
node_t *search_node(node_t *pnode, int flag);//노드 하나 검색

int menu(){
	int buff;
	int flag = 1;

	while(flag){
		printf("===================================\n");
		printf("(1)list up (2)search (3)store (4)use (5)exit\n");
		printf("===================================\n");
		printf(":");
		scanf("%d", &buff);

		if(!(buff > 0 && buff < 6)){
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
		print_node(pnode);
		pnode = pnode->next;
	}
	return list_head;
}
void print_node(node_t *pnode){
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
}
node_t *search_item(node_t *list_head){
	node_t *tmp;

	tmp = search_node(list_head, 1);
	if(tmp == NULL)
		printf("Not found item\n");
	while(tmp){
		print_node(tmp);
		tmp = search_node(tmp->next, 1);
	}
	
	return list_head;
}
node_t *search_node(node_t *list_head,int flg){
	node_t *tmp_node;
	tmp_node = (node_t*)malloc(sizeof(node_t));
	node_t *next_p;
	next_p = list_head;
	int flag;


	switch(flg){
		case 0:
			printf("name:");
			scanf("%s",tmp_node->item.name);
			printf("expriy date(xxxx.xx.xx):");
			scanf("%s",tmp_node->item.exp_d);
			while(next_p){
				flag = strcmp(tmp_node->item.name, next_p->item.name);
				if(flag == 0){
					flag = strcmp(tmp_node->item.exp_d, next_p->item.exp_d);
					if(flag == 0)
						tmp_node = next_p;
				}
				next_p = next_p->next;
			}
			break;
		case 1:
			printf("name:");
			scanf("%s",tmp_node->item.name);
			while(next_p){
				flag = strcmp(tmp_node->item.name, next_p->item.name);
				if(flag == 0)
				tmp_node = next_p;
				next_p = next_p->next;
			}
			break;
		default:
			printf("wrong search flag\n");
	}
	next_p = tmp_node;
	return next_p;
}	
node_t *store_item(node_t *list_head){
	struct tm *date;
	const time_t t = time(NULL);
	node_t *pnode;
	pnode = (node_t*)malloc(sizeof(node_t));

	printf("itme name: ");//name
	fgets(pnode->item.name, NAME_SIZE, stdin);

	printf("셀 수 있나요?(0 or 1): ");//remain
	scanf("%d", &pnode->item.flag);
	if(pnode->item.flag == 0){
		printf("quantity(integer): ");
		scanf("%d", &pnode->item.remain.qnt);
	}
	else{
		printf("amount(): ");
		scanf("%s", pnode->item.remain.amt);
	}

	printf("name:");
	//fgets(pnode->item.name, NAME_SIZE, stdin);//name
	scanf("%s",pnode->item.name);	

	printf("expiry date(xxxx.xx.xx): ");//expriy date
	//sscanf(pnode->item.exp_d, DATE_SIZE, stdin);
	scanf("%s",pnode->item.exp_d);

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

