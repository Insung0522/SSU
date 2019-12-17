#include "data.c"
#define FUNC_NUM 5

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
