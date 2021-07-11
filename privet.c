#include "privet.h"


S_node* find_Da_father(S_node *root, char da)
{
	if (root->right == NULL)
	{
		if (root->left->da == da)
			return root;
		else return find_Da_father(root->left, da);
	}
	if (root->left == NULL)
	{
		if (root->right->da == da)
			return root;
		else return find_Da_father(root->right, da);
	}
	if (root->left->da == da || root->right->da == da)
		return root;
	if (da < root->da)	return find_Da_father(root->left, da);
	else return find_Da_father(root->right, da);
}

S_node *find_min_root(S_node *root)
{
	if (root->left == NULL)
		return root;
	find_min_root(root->left);
}

void initTree(BST*bst)
{
	bst->root = NULL;
}

int isEmpty_p0(S_Out_Qs_mgr* q)
{
	return (q->head_p0 == NULL && q->tail_p0 == NULL);
}

int isEmpty_p1(S_Out_Qs_mgr* q)
{
	return (q->head_p1 == NULL && q->tail_p1 == NULL);
}

S_node* createNode(unsigned char da, unsigned char output_port)
{
	S_node* tmp = (S_node*)calloc(1, sizeof(S_node));
	assert(tmp);
	tmp->da = da;
	tmp->output_port = output_port;
	return tmp;
}

void makeOrder_0(S_Out_Qs_mgr* out, S_pkt* q)
{
	if (isEmpty_p0(out))
	{
		out->head_p0 = q;
		out->tail_p0 = q;
		return;
	}
	S_pkt *ptr = out->head_p0;
	S_pkt *tmp = ptr->next;
	if (tmp == NULL)//second S_pkt case
	{
		if (q->pkt->time < ptr->pkt->time)
		{
			out->head_p0 = q;
			q->next = ptr;
		}
		else
		{
			ptr->next = q;
			out->tail_p0 = q;
		}
		return;
	}
	if (q->pkt->time < ptr->pkt->time)//add to head(first)
	{
		q->next = ptr;
		out->head_p0 = q;
		return;
	}
	if (q->pkt->time > out->tail_p0->pkt->time)//add to tail(last)
	{
		out->tail_p0->next = q;
		out->tail_p0 = q;
		return;
	}
	while (tmp->next != NULL)
	{
		if (ptr->pkt->time<q->pkt->time &&tmp->pkt->time>q->pkt->time)//add between
		{
			ptr->next = q;
			q->next = tmp;
			return;
		}
		ptr = ptr->next;
		tmp = tmp->next;
	}
}

void makeOrder_1(S_Out_Qs_mgr* out, S_pkt* q)
{
	if (isEmpty_p1(out))//makro
	{
		out->head_p1 = q;
		out->tail_p1 = q;
		return;
	}
	S_pkt *ptr = out->head_p1;
	S_pkt *tmp = ptr->next;
	if (tmp == NULL)//second S_pkt case

	{
		if (q->pkt->time < ptr->pkt->time)
		{
			out->head_p1 = q;
			q->next = ptr;
		}
		else
		{
			ptr->next = q;
			out->tail_p1 = q;
		}
		return;
	}
	if (q->pkt->time < ptr->pkt->time)//add to head(first)
	{
		q->next = ptr;
		out->head_p1 = q;
		return;
	}
	if (q->pkt->time > out->tail_p1->pkt->time)//add to tail(last)
	{
		out->tail_p1->next = q;
		out->tail_p1 = q;
		return;
	}
	while (tmp->next != NULL)
	{
		if (ptr->pkt->time<q->pkt->time &&tmp->pkt->time>q->pkt->time)//add between
		{
			ptr->next = q;
			q->next = tmp;
			return;
		}
		ptr = ptr->next;
		tmp = tmp->next;
	}
}

void pktselction(S_node *root, FILE *fp, S_Out_Qs_mgr *q)
{
	unsigned char tmp;
	S_node* ptr;
	rewind(fp);
	while (!feof(fp))
	{
		packet* pkt = (packet*)calloc(1, sizeof(packet));
		packet_read(fp, pkt);
		ptr = search_route(root, pkt->Da);
		if (ptr == NULL) free(pkt);
		else enque_pkt(&q[((int)ptr->output_port) - 1], pkt);
	}
}

void printout(char* file_name, S_Out_Qs_mgr* queue)
{
	FILE* file = fopen(file_name, "w");
	if (!file)
	{
		printf("Error opening file.\n");
		return;
	}
	while (queue->head_p0 != NULL) {
		packet* pkt = deque_pkt(queue, 0);
		packet_write(file, pkt);
		free(pkt->data);
		free(pkt);
	}
	while (queue->head_p1 != NULL) {
		packet* pkt = deque_pkt(queue, 1);
		packet_write(file, pkt);
		free(pkt->data);
		free(pkt);
	}
	fclose(file);
}

void freeTree(S_node *root)
{
	if (!root) return;
	if (root->right == NULL && root->left == NULL)
	{
		free(root);
		return;
	}
	freeTree(root->right);
	freeTree(root->left);
	free(root);
}
