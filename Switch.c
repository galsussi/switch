#include"privet.h"

void main(int argc,char* argv[])
{
	S_Out_Qs_mgr* list_out_port = (S_Out_Qs_mgr*)calloc(4, sizeof(S_Out_Qs_mgr));
	assert(list_out_port);
	BST* main_bst = (BST*)calloc(1, sizeof(BST));
	assert(main_bst);
	initTree(main_bst);

	FILE* file1 = fopen(argv[1], "r");FILE_OPEN(file1);
	FILE* file2 = fopen(argv[2], "r");FILE_OPEN(file2);
	FILE* file3 = fopen(argv[3], "r");FILE_OPEN(file3);
	FILE* file4 = fopen(argv[4], "r");FILE_OPEN(file4);
	FILE* file5 = fopen(argv[5], "r");FILE_OPEN(file5);

	char* file_name[] = { "port1.out","port2.out","port3.out","port4.out" };
	main_bst->root = build_route_table(file1, main_bst->root); fclose(file1);
	pktselction(main_bst->root, file2, list_out_port); fclose(file2);
	pktselction(main_bst->root, file3, list_out_port); fclose(file3);
	pktselction(main_bst->root, file4, list_out_port); fclose(file4);
	pktselction(main_bst->root, file5, list_out_port); fclose(file5);
	  
	for ( int i = 0;  i < 4; i++)
		printout(file_name[i], &list_out_port[i]);

	freeTree(main_bst->root);
	free(main_bst);
	free(list_out_port);
	system("pause");
}

