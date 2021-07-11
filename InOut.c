#include"privet.h"

void packet_read(FILE *fp, packet *pkt)
{
	fscanf(fp, "%u %hhu %hhu %hhu %hhu", &pkt->time, &pkt->Da, &pkt->Sa, &pkt->prio, &pkt->Data_length);
	pkt->data = (unsigned char*)malloc(pkt->Data_length * sizeof(unsigned char));
	assert(pkt->data);
	for (int i = 0; i < (unsigned char)pkt->Data_length; i++)
		fscanf(fp, "%hhu", &pkt->data[i]);
	fscanf(fp, "%hhu", &pkt->checksum);
}

void packet_write(FILE *fp, const packet *pkt)
{
	fprintf(fp, "%u %hhu %hhu %hhu %hhu", pkt->time, pkt->Da, pkt->Sa, pkt->prio, pkt->Data_length);
	for (int i = 0; i < pkt->Data_length; i++)
		fprintf(fp, "%hhu ", pkt->data[i]);
	fprintf(fp, "%hhu\n", pkt->checksum);
}

Bool checksum_check(const packet *pkt)
{
	unsigned char check = pkt->Da^pkt->Sa;
	check ^= pkt->prio;
	check ^= pkt->Data_length;
	for (int i = 0; i < pkt->Data_length; i++)
		check ^= pkt->data[i];
	if (check == pkt->checksum)return true;
	else return false;
}

S_node * add_route(S_node *root, char da, char output_port)
{
	unsigned char da1 = da;
	unsigned char output_port1 = output_port;
	if (root == NULL)
		return root = createNode(da1, output_port1);
	if (da <= root->da)
	{
		root->left = add_route(root->left, da1, output_port1);
		return root;
	}
	else {
		root->right = add_route(root->right, da1, output_port1);
		return root;
	}
}

S_node *delete_route(S_node *root, char da)
{
	if (root == NULL)
	{
		printf("The tree is empty.\n");
		return NULL;
	}
	if (da == root->da)
	{
		S_node* ptr = root, *tmp, *min;
		if (ptr->left != NULL && ptr->right != NULL); //delete main root-extrem case.
		{
			min = find_min_root(ptr->right);
			min->left = ptr->left;
			root = root->right;
			free(ptr);
			return root;
		}
		if (ptr->left == NULL && ptr->right != NULL)//delete right side from main root-extrem case
		{
			root = root->right;
			free(ptr);
			return root;
		}
		if (ptr->left != NULL && ptr->right == NULL)//delete left side from maon root-extrem case
		{
			root = root->left;
			free(ptr);
			return root;
		}
		if (ptr->left == NULL && ptr->right == NULL)//delete main root 
		{
			free(ptr);
			return NULL;
		}
	}
	S_node* ptr, *tmp, *min;
	ptr = find_Da_father(root, da);
	if (ptr->right == NULL || ptr->left == NULL)
		tmp = (ptr->left == NULL) ? ptr->right : ptr->left;
	else tmp = ptr->right->da == da ? ptr->right : ptr->left;
	if (tmp->left != NULL && tmp->right != NULL); //delete midele root.
	{
		min = find_min_root(tmp->right);
		min->left = tmp->left;
		if (tmp->right->da < ptr->da)
			ptr->left = tmp->right;
		else ptr->right = tmp->right;
		free(tmp);
		return root;
	}
	if (tmp->left == NULL && tmp->right == NULL)//delete leaf
	{
		free(tmp);
		return root;
	}
	if (tmp->left == NULL && tmp->right != NULL)//delete one child rigth side
	{
		if (tmp->da == ptr->left->da)
		{
			ptr->left = tmp->right;
			free(tmp);
			return root;
		}
		ptr->right = tmp->right;
		free(ptr);
		return root;
	}
	if (tmp->left != NULL && tmp->right == NULL)//delete one child left side
	{
		if (tmp->da == ptr->right->da)
		{
			ptr->right = tmp->left;
			free(tmp);
			return root;
		}
		ptr->left = tmp->left;
		free(ptr);
		return root;
	}
}

S_node *search_route(const S_node *root, char da)
{
	unsigned char da1 = da;
	if (!root)return NULL;
	if (root->da == da1) return root;
	else if (da1 > root->da) return search_route(root->right, da);
	else return search_route(root->left, da);
}

void print_routing_table(const S_node *root)
{
	if (root != NULL)
	{
		print_routing_table(root->left);
		printf("%d ", root->da);
		print_routing_table(root->right);
	}
}

S_node * build_route_table(FILE *fp, S_node *root)
{
	unsigned char act;
	int da, port;
	do
	{
		fscanf(fp, "%c %d ", &act, &da);
		if (act == 'a')
		{
			fscanf(fp, "%d ", &port);
			root = add_route(root, (unsigned char)da, (unsigned char)port);
		}
		else delete_route(root, (unsigned char)da);
	} while (!feof(fp));
	print_routing_table(root);
	return root;
}

void initQueue(S_Out_Qs_mgr* q)
{
	q->head_p0 = NULL;
	q->head_p1 = NULL;
	q->tail_p0 = NULL;
	q->tail_p1 = NULL;
}

void enque_pkt(S_Out_Qs_mgr *QM_ptr, packet *pkt)
{
	if (!checksum_check(pkt))
	{
		printf("\nWrong data delete packet.");
		free(pkt->data);
		free(pkt);
		return;
	}
	S_pkt* q = (S_pkt*)calloc(1, sizeof(S_pkt));
	assert(q);
	q->pkt = pkt;
	if (pkt->prio == 0)
		makeOrder_0(QM_ptr, q);

	if (pkt->prio == 1)
		makeOrder_1(QM_ptr, q);
}

packet *deque_pkt(S_Out_Qs_mgr *QM_ptr, char priority)
{
	if (priority == 0)
	{
		if (isEmpty_p0(QM_ptr))
		{
			printf("The queue is empty.\n");
			return  NULL;
		}
		packet* out = (packet*)calloc(1, sizeof(packet));
		out = QM_ptr->head_p0->pkt;
		S_pkt* ptr = QM_ptr->head_p0;
		QM_ptr->head_p0 = QM_ptr->head_p0->next;
		if (QM_ptr->head_p0 == NULL)
			QM_ptr->tail_p0 = NULL;
		free(ptr);
		return out;
	}
	if (priority == 1)
	{
		if (isEmpty_p1(QM_ptr))
		{
			printf("The queue is empty.\n");
			return  NULL;
		}
		packet* out = (packet*)calloc(1, sizeof(packet));
		out = QM_ptr->head_p1->pkt;
		S_pkt* ptr = QM_ptr->head_p1;
		QM_ptr->head_p1 = QM_ptr->head_p1->next;
		if (QM_ptr->head_p1 == NULL)
			QM_ptr->tail_p1 = NULL;
		free(ptr);
		return out;
	}
}