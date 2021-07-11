#pragma once
#include "privet.h"

typedef struct route_node
{
	unsigned char da;
	unsigned char output_port;
	struct route_node *left, *right;
} S_node;
typedef enum Bool { false, true } Bool;
typedef struct packet//לצמצם את המבנה אחרי בדיקה
{
	unsigned int time;
	unsigned char Da;
	unsigned char Sa;
	unsigned char prio; // לנסות לעשות מקרו שיקבע 0/1
	unsigned char Data_length;
	unsigned char* data; // יש לשים לב שבהכנסה לתוף המבנה אנחנו צריכים להקצות את גודל המערך הנ"ל
	unsigned char checksum;
}packet;
typedef struct BST
{
	S_node* root;
}BST;
typedef struct pkt_node {
	packet *pkt;
	struct pkt_node *next;
} S_pkt;
typedef struct Out_Qs_mgr {
	struct pkt_node *head_p1;
	struct pkt_node *tail_p1;
	struct pkt_node *head_p0;
	struct pkt_node *tail_p0;
} S_Out_Qs_mgr;

void packet_read(FILE *fp, packet *pkt);
void packet_write(FILE *fp, const packet *pkt);
Bool checksum_check(const packet *pkt);
S_node* createNode(unsigned char da, unsigned char output_port);
S_node * add_route(S_node *root, char da, char output_port);
S_node *delete_route(S_node *root, char da);
S_node *search_route(const S_node *root, char da);
void print_routing_table(const S_node *root);
S_node * build_route_table(FILE *fp, S_node *root);
void enque_pkt(S_Out_Qs_mgr *QM_ptr, packet *pkt);
packet *deque_pkt(S_Out_Qs_mgr *QM_ptr, char priority);

