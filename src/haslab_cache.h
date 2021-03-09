#include <stdbool.h>
//#define HASLAB_BUFFER_SIZE 64
#define MAX_PROMOTION_THREAD_NUM 64
//to do :修改成链表，取消使用条件变量

typedef struct PromotionBufferNode{ 
    void* data;                         //数据,结构体形式
   // struct PromotionBufferNode * pre;   
    struct PromotionBufferNode * next;  
} PromotionBufferNode;

typedef struct Promotion_Info {
    int len;
    struct PromotionBufferNode * promotionbuferhead; //后台线程去取出
    struct PromotionBufferNode * promotionbufertail; //主线程插入
} Promotion_Info;

void InitP();
void InitPromotion(Promotion_Info *pi);
bool PromotionIsEmpty(Promotion_Info *pi);
void PromotionPush(Promotion_Info *pi, void* key);
void* PromotionPop(Promotion_Info *pi, (void *)  data);
void promotionbufferdelete(Promotion_Info *pi);//用于主线程在更新时删除无效的条目


/*****************************************************
*Assert,malloc failed，printf“malloc ... error”
******************************************************/
void alloc_assert(void *p, char *s)
{
	if (p != NULL) return;
	printf("malloc %s error\n", s);
	//getchar();
	exit(-1);
}