#include <stdbool.h>
#include <pthread.h>
//#define HASLAB_BUFFER_SIZE 64
#define MAX_PROMOTION_THREAD_NUM 100

typedef struct KVCache_Node{
    const char *key;
    const char *value;
} KVCache_Node;

typedef struct PromotionBufferNode{
    void *data; //数据,结构体形式
    struct PromotionBufferNode *next;
} PromotionBufferNode;

typedef struct Promotion_Info {
    int len;
    struct PromotionBufferNode * promotionbuferhead; //后台线程去取出
    struct PromotionBufferNode * promotionbufertail; //主线程插入
    pthread_mutex_t pi_mutex;
} Promotion_Info;

typedef struct Thread_Pool{
    pthread_t haslab_cacheloop_tid; //后台页迁移线程
    int promotion_thread_num;
    pthread_mutex_t tp_mutex;
} Thread_Pool;

void InitP();
void InitFirstThread();
void InitNewThread(int tp_len);
void InitPromotion(Promotion_Info *pi, Thread_Pool *tp);
bool PromotionIsEmpty(Promotion_Info *pi);
void PromotionPush(Promotion_Info *pi, char *key, char *val);
PromotionBufferNode* PromotionPop(Promotion_Info *pi);
void promotionbufferdelete(Promotion_Info *pi);//用于主线程在更新时删除无效的条目
