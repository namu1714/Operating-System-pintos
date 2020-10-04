#include "main.h"

int main(){
	char cmd[MAX_COMMAND_SIZE];
	while(1){
		fgets(cmd, MAX_COMMAND_SIZE, stdin);
		cmd[strlen(cmd)-1] = '\0';
		for(int i=0;i<MAX_ARGS;i++)
			args[i][0] = '\0';

		//initialize args
		memset(args, 0, sizeof(args));

		processCommand(cmd);

		if(!strcmp(args[0], "quit")){
			break;
		}
		else if(!strcmp(args[0], "create")){
			createHandler();
		}
		else if(!strcmp(args[0], "delete")){
			deleteHandler();
		}
		else if(!strcmp(args[0], "dumpdata")){
			dumpdataHandler();
		}
		else{ //other command
			if(!strncmp(args[0], "list", 4)){
				listFunctions(OTHERS);
			}
			else if(!strncmp(args[0], "hash", 4)){
				hashFunctions(OTHERS);
			}
			else if(!strncmp(args[0], "bitmap", 6)){
				bitmapFunctions(OTHERS);
			}
		}
	}
	return 0;
}

void processCommand(char* cmd){
	char s[MAX_COMMAND_SIZE];
	char *token;

	strcpy(s, cmd);
	
	//create args
	token = strtok(s, " ");
	strcpy(args[0], token);
	for(int i=1; i<MAX_ARGS; i++){
		token = strtok(NULL, " ");
		if(token == NULL) break;
		strcpy(args[i], token);
	}
	return;
}

void hashFunctions(int flag){
	int n, value;
	struct hash_item *item;
	struct hash_iterator it;

	switch(flag){
	case CREATE:
		n = args[2][4]-'0';
		H[n] = (struct hash *)malloc(sizeof(struct hash));
		if(!hash_init(H[n], hash_hash_function, hash_less_function, NULL)){
			printf("hash_init error");
			exit(1);
		}
		break;
	case DELETE:
		n = args[1][4]-'0';
		hash_destroy(H[n], hash_destructor);
		free(H[n]);
		break;
	case DUMP:
		n = args[1][4]-'0';
		hash_first(&it, H[n]);
		while(hash_next(&it)){
			item = hash_entry(hash_cur(&it), struct hash_item, elem);
			printf("%d ", item->data);
		}
		printf("\n");
		break;
	}
	if(flag == OTHERS){
		n = args[1][4]-'0';
		if(!strcmp("hash_insert", args[0])){
			item = (struct hash_item*)malloc(sizeof(struct hash_item));
			item->data = atoi(args[2]);
			hash_insert(H[n], &(item->elem));
		}
		else if(!strcmp("hash_delete", args[0])){
			item = (struct hash_item*)malloc(sizeof(struct hash_item));
			item->data = atoi(args[2]);
			free(hash_delete(H[n], &(item->elem)));
			free(item);
		}
		else if(!strcmp("hash_empty", args[0])){
			if(hash_empty(H[n]) == true){
				printf("true\n");
			} else{
				printf("false\n");
			}
		}
		else if(!strcmp("hash_size", args[0])){
			printf("%zu\n", hash_size(H[n]));
		}
		else if(!strcmp("hash_clear", args[0])){
			hash_clear (H[n], hash_destructor); 
		}
		else if(!strcmp("hash_find", args[0])){
			value = atoi(args[2]);
			item = (struct hash_item*)malloc(sizeof(struct hash_item));
			item->data = value;
			if(hash_find(H[n], &(item->elem)) != NULL){ //find an element
				printf("%d\n", value);
			}
		}
		else if(!strcmp("hash_replace", args[0])){
			struct hash_elem* old;
			value = atoi(args[2]);
			item = (struct hash_item*)malloc(sizeof(struct hash_item));
			item->data = value;
			old = hash_replace(H[n], &(item->elem));
			
			if(old != NULL){
				item = hash_entry(old, struct hash_item, elem);
				free(item);
			}
		}
		else if(!strcmp("hash_apply", args[0])){
			int flag;

			if(!strcmp("square", args[2])){
				flag = 2;
			}
			else if(!strcmp("triple", args[2])){
				flag = 3;
			}
			H[n]->aux = &flag;
			hash_apply(H[n], hash_multiply);
		}
	}
}

void bitmapFunctions(int flag){
	int n;
	bool value;
	size_t size, idx, start, cnt;

	switch(flag){
	case CREATE:
		n = args[2][2]-'0';
		size = atoi(args[3]);
		B[n] = bitmap_create(size);
		break;
	case DELETE:
		n = args[1][2]-'0';
		bitmap_destroy(B[n]);
		break;
	case DUMP:
		n = args[1][2]-'0';
		size = B[n]->bit_cnt;
		for(size_t i=0; i<size; i++){
			if(bitmap_test(B[n], i) == true){
				printf("1");
			}else{
				printf("0");
			}
		}
		printf("\n");
		break;
	}
	if(flag == OTHERS){
		n = args[1][2]-'0';
		if(!strcmp("bitmap_mark", args[0])){
			idx = atoi(args[2]);
			bitmap_mark(B[n], idx);
		}
		else if(!strcmp("bitmap_all", args[0])){
			start = atoi(args[2]);
			cnt = atoi(args[3]);
			if(bitmap_all(B[n], start, cnt) == true){
				printf("true\n");
			} else{
				printf("false\n");
			}
		}
		else if(!strcmp("bitmap_any", args[0])){
			start = atoi(args[2]);
			cnt = atoi(args[3]);
			if(bitmap_any(B[n], start, cnt) == true){
				printf("true\n");
			} else{
				printf("false\n");
			}
		}
		else if(!strcmp("bitmap_contains", args[0])){
			start = atoi(args[2]);
			cnt = atoi(args[3]);
			value = stringToBool(args[4]);
			if(bitmap_contains(B[n], start, cnt, value) == true){
				printf("true\n");
			}else{
				printf("false\n");
			}
		}
		else if(!strcmp("bitmap_count", args[0])){
			start = atoi(args[2]);
			cnt = atoi(args[3]);
			value = stringToBool(args[4]);
			size = bitmap_count(B[n], start, cnt, value);
			printf("%zu\n", size);
		}
		else if(!strcmp("bitmap_dump", args[0])){
			bitmap_dump(B[n]);
		}
		else if(!strcmp("bitmap_expand", args[0])){
			B[n] = bitmap_expand(B[n], atoi(args[2]));
		}
		else if(!strcmp("bitmap_set", args[0])){
			idx = atoi(args[2]);
			value = stringToBool(args[3]);
			bitmap_set(B[n], idx, value);
		}
		else if(!strcmp("bitmap_set_all", args[0])){
			value = stringToBool(args[2]);
			bitmap_set_all(B[n], value);
		}
		else if(!strcmp("bitmap_set_multiple", args[0])){
			start = atoi(args[2]);
			cnt = atoi(args[3]);
			value = stringToBool(args[4]);
			bitmap_set_multiple(B[n], start, cnt, value);
		}
		else if(!strcmp("bitmap_flip", args[0])){
			idx = atoi(args[2]);
			bitmap_flip(B[n], idx);
		}
		else if(!strcmp("bitmap_none", args[0])){
			start = atoi(args[2]);
			cnt = atoi(args[3]);
			if(bitmap_none(B[n], start, cnt) == true){
				printf("true\n");
			} else{
				printf("false\n");
			}
		}
		else if(!strcmp("bitmap_reset", args[0])){
			idx = atoi(args[2]);
			bitmap_reset(B[n], idx);
		}
		else if(!strcmp("bitmap_scan", args[0])){
			start = atoi(args[2]);
			cnt = atoi(args[3]);
			value = stringToBool(args[4]);
			printf("%zu\n", bitmap_scan(B[n], start, cnt, value));
		}
		else if(!strcmp("bitmap_scan_and_flip", args[0])){
			start = atoi(args[2]);
			cnt = atoi(args[3]);
			value = stringToBool(args[4]);
			printf("%zu\n", bitmap_scan_and_flip(B[n], start, cnt, value));
		}
		else if(!strcmp("bitmap_size", args[0])){
			printf("%zu\n", bitmap_size(B[n]));
		}
		else if(!strcmp("bitmap_test", args[0])){
			idx = atoi(args[2]);
			if(bitmap_test(B[n], idx) == true){
				printf("true\n");
			}else{
				printf("false\n");
			}
		}
	}
}

void listFunctions(int flag){
	int n, index, value;
	size_t size;
	struct list_elem *e;
	struct list_item *item;

	switch(flag){
	case CREATE:
		n = args[2][4]-'0';
		L[n] = (struct list*)malloc(sizeof(struct list));
		list_init(L[n]);
		break;
	case DELETE:
		n = args[1][4]-'0';
		while(!list_empty(L[n])){
			e = list_pop_back(L[n]);
			item = list_entry(e, struct list_item, elem);
			free(item);
		}
		free(L[n]);
		break;
	case DUMP:
		n = args[1][4]-'0';
		e = list_begin(L[n]);
		
		while(e != list_end(L[n])){
			item = list_entry(e, struct list_item, elem);
			printf("%d ", item->data);
			e = list_next(e);
		}
		printf("\n");
		break;
	}
	if(flag == OTHERS){
		n = args[1][4]-'0';
		if(!strcmp("list_front", args[0])){
			e = list_front(L[n]);
			item = list_entry(e, struct list_item, elem);
			printf("%d\n", item->data);
		}
		else if(!strcmp("list_back", args[0])){
			e = list_rbegin(L[n]);
			item = list_entry(e, struct list_item, elem);
			printf("%d\n", item->data);
		}
		else if(!strcmp("list_push_front", args[0])){
			item = (struct list_item*)malloc(sizeof(struct list_item));
			item->data = atoi(args[2]);
			list_push_front(L[n], &(item->elem));
       		}
       		else if(!strcmp("list_push_back", args[0])){
			item = (struct list_item*)malloc(sizeof(struct list_item));
			item->data = atoi(args[2]);
			list_push_back(L[n], &(item->elem));
        	}
        	else if(!strcmp("list_pop_front", args[0])){
			e = list_pop_front(L[n]);
			item = list_entry(e, struct list_item, elem);
			free(item);
        	}
        	else if(!strcmp("list_pop_back", args[0])){
			e = list_pop_back(L[n]);
			item = list_entry(e, struct list_item, elem);
			free(item);
		}
		else if(!strcmp("list_insert", args[0])){
			index = atoi(args[2]);
			value = atoi(args[3]);
			e = list_find(L[n], index);
			item = (struct list_item*)malloc(sizeof(struct list_item));
			item->data = value;
			list_insert(e, &(item->elem));
		}
		else if(!strcmp("list_insert_ordered", args[0])){
			value = atoi(args[2]);
			item = (struct list_item*)malloc(sizeof(struct list_item));
			item->data = value;
			list_insert_ordered(L[n], &(item->elem), list_less_function, NULL);
		}
		else if(!strcmp("list_empty", args[0])){
			if(list_empty(L[n]) == true){
				printf("true\n");
			}else{
				printf("false\n");
			}
		}
		else if(!strcmp("list_size", args[0])){
			size = list_size(L[n]);
			printf("%zu\n", size);
		}
		else if(!strcmp("list_max", args[0])){
			e = list_max(L[n], list_less_function, NULL);
			item = list_entry(e, struct list_item, elem);
			printf("%d\n", item->data);
		}
		else if(!strcmp("list_min", args[0])){
			e = list_min(L[n], list_less_function, NULL);
			item = list_entry(e, struct list_item, elem);
			printf("%d\n", item->data);
		}
		else if(!strcmp("list_remove", args[0])){
			e = list_find(L[n], atoi(args[2]));
			list_remove(e);
			item = list_entry(e, struct list_item, elem);
			free(item);
		}
		else if(!strcmp("list_reverse", args[0])){
			list_reverse(L[n]);
		}
		else if(!strcmp("list_shuffle", args[0])){
			list_shuffle(L[n]);
		}
		else if(!strcmp("list_sort", args[0])){
			list_sort(L[n], list_less_function, NULL);
		}
		else if(!strcmp("list_splice", args[0])){
			int n2 = args[3][4]-'0';
			index = atoi(args[2]);
			struct list_elem *before = list_find(L[n], index);

			int start = atoi(args[4]);
			int end = atoi(args[5]);
			struct list_elem *first = list_find(L[n2], start);
			struct list_elem *last = list_find(L[n2], end);

			list_splice(before, first, last);
		}
		else if(!strcmp("list_swap", args[0])){
			int index1 = atoi(args[2]);
			int index2 = atoi(args[3]);
			struct list_elem *e1 = list_find(L[n], index1);
			struct list_elem *e2 = list_find(L[n], index2);
			list_swap(e1, e2);
		}
		else if(!strcmp("list_unique", args[0])){
			if(strcmp(args[2], "")) { //duplicate 존재
				int duplicates = args[2][4]-'0';
				list_unique(L[n], L[duplicates], list_less_function, NULL);
			}
			else{
				list_unique(L[n], NULL, list_less_function, NULL);
			}
		}
	}
}

void createHandler(){
	if(!strcmp(args[1], "list"))
		listFunctions(CREATE);
	else if(!strcmp(args[1], "hashtable"))
		hashFunctions(CREATE);
        else if(!strcmp(args[1], "bitmap"))
		bitmapFunctions(CREATE);
}

void deleteHandler(){
	if(!strncmp(args[1], "list", 4))
                listFunctions(DELETE);
        else if(!strncmp(args[1], "hash", 4))
                hashFunctions(DELETE);
        else if(!strncmp(args[1], "bm", 2))
                bitmapFunctions(DELETE);
}

void dumpdataHandler(){
	if(!strncmp(args[1], "list", 4))
                listFunctions(DUMP);
        else if(!strncmp(args[1], "hash", 4))
                hashFunctions(DUMP);
        else if(!strncmp(args[1], "bm", 2))
                bitmapFunctions(DUMP);
}

bool stringToBool(char *s){
	bool flag = false;
	if(!strcmp(s, "true")){
		flag = true;
	} else if(!strcmp(s, "false")){
		flag = false;
	}
	return flag;
}
