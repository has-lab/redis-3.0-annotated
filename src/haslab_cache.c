#include "haslab_cache.h"
#include "redis.h"

Promotion_Info promotion_info;
int aaa = 0;
bool to_promotion;
//只用锁，不用条件变量
//pthread_t cache_threads[HASLAB_CACHE_NUM_OPS];
pthread_mutex_t cache_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cache_cond = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t dict_mutex;

pthread_t haslab_cacheloop_pid;

void HandlePromotion(dictEntry *de, robj* val){
    void *key;
    while(true){
        pthread_mutex_lock(&cache_mutex);
        while(!to_promotion){
            pthread_cond_wait(&cache_cond, &cache_mutex);
        }
        pthread_mutex_unlock(&cache_mutex);
        printf("aaa = %d\n", aaa);
        to_promotion = false;
        PromotionPop(&promotion_info, key);
        printf("key = %s\n", key);
    }
}

void PromotionCallback(void* key){
    //检查条目是否存在


    //更新条目信息
   //pthread_mutex_lock(&dict_mutex);
    
    
   //pthread_mutex_unlock(&dict_mutex);
}

void InitPromotion(Promotion_Info *pi)//初始化
{
    pi->head = pi->tail = 0;	//初始化前后指针都指向0
    memset(pi->buffer, 0, HASLAB_BUFFER_SIZE);
}

void InitP(){
    InitPromotion(&promotion_info);
}

bool PromotionIsEmpty(Promotion_Info *pi)//判断是否为空
{
    if(pi->head==pi->tail)	//若前后指针指向同一个节点，则判断为空
        return true;
    else
        return false;
}

bool PromotionIsFull(Promotion_Info *pi)//判断是否为满
{
    if(pi->head==(pi->tail+1)%HASLAB_BUFFER_SIZE)	//若前指针等于(后指针+1)%数组大小，则判断为满
        return true;
    else
        return false;
}

void PromotionPush(Promotion_Info *pi, void* key)//进队
{
    if(PromotionIsFull(pi))		//若队列为满，则进队失败
        return;
    pi->buffer[pi->tail] = key;	//否则将进队元素赋值给后指针所指的位置，后指针往后移动一格
    pi->tail=(pi->tail+1)%HASLAB_BUFFER_SIZE;
}

void PromotionPop(Promotion_Info *pi, void *key)//出队
{
    if(PromotionIsEmpty(pi))	//若队列为空，则出队失败
        return;
    key = pi->buffer[pi->head]; //传入一个指针保留出队元素，然后队首指向下一个元素
    pi->head=(pi->head+1)%HASLAB_BUFFER_SIZE;
}