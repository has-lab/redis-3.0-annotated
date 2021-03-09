#include <stdbool.h>
#include <pthread.h>
//#define HASLAB_BUFFER_SIZE 64
#define MAX_PROMOTION_THREAD_NUM 100
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
    pthread_mutex_t pi_mutex;
} Promotion_Info;

typedef struct Thread_Pool{
    pthread_t haslab_cacheloop_tid[MAX_PROMOTION_THREAD_NUM]; //后台页迁移线程表
    int promotion_thread_num ;
    pthread_mutex_t tp_mutex;
} Thread_Pool;

void InitP();
void InitNewThread(int len);
void InitPromotion(Promotion_Info *pi, Thread_Pool *tp);
bool PromotionIsEmpty(Promotion_Info *pi);
void PromotionPush(Promotion_Info *pi, void* key);
PromotionBufferNode* PromotionPop(Promotion_Info *pi, void *data);
void promotionbufferdelete(Promotion_Info *pi);//用于主线程在更新时删除无效的条目
