#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define FILE_OPEN(a) if(a==NULL) printf("EROR opening file\n")
#include"InOut.h"

//make oreder in enter to the queue//
void makeOrder_1(S_Out_Qs_mgr* out, S_pkt* q);
void makeOrder_0(S_Out_Qs_mgr* out, S_pkt* q);
//find root father//
S_node* find_Da_father(S_node *root, char da);
//find min root just for case of delete	specific root//
S_node *find_min_root(S_node *root);
void initTree(BST*bst);
S_node* createNode(unsigned char da, unsigned char output_port);
//check if queue is empty//
int isEmpty_p0(S_Out_Qs_mgr* q);
int isEmpty_p1(S_Out_Qs_mgr* q);
// void pktselction read from files and create new packets and put in queue by order//
void pktselction(S_node *root, FILE *fp, S_Out_Qs_mgr *q);
//collect data from tree and text , and send to write in ports and output text//
void printout(char* file_name, S_Out_Qs_mgr* queue);
void freeTree(S_node *root);