#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

WP* new_wp()
{
	WP* temp;
	
	temp = free_;
	if (temp == NULL)
		assert(0);
	free_ = free_->next;
	temp->next = head;
	head = temp;
	
	return temp;
}

void free_wp(WP *wp)
{
	WP* temp;
	
	temp = head;
	if (temp == wp)
	{
		head->next = free_;
		free_ = head;
		head = head->next;
		if (head == NULL) printf("%p\n",head);
	}
	else
	{
		while (temp->next != wp)
			temp = temp->next;
		temp->next = wp->next;
		wp->next = free_;
		free_ = wp;
	}
}

WP* get_head()
{
	return head;
}
/* TODO: Implement the functionality of watchpoint */
