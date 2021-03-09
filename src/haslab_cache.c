#include "haslab_cache.h"
#include "redis.h"

Promotion_Info promotion_info;
int aaa = 0;
bool to_promotion;
//to do只用锁，不用条件变量
//pthread_t cache_threads[HASLAB_CACHE_NUM_OPS];
pthread_mutex_t cache_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cache_cond = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t dict_mutex;

pthread_t haslab_cacheloop_tid[MAX_PROMOTION_THREAD_NUM]; //后台页迁移线程表

void HandlePromotion(Promotion_Info* pi){
    void * data;
    PromotionBufferNode *tmp=NULL;
    //开始执行promotion流程
    void *key;
    while(true){
        //如果producer比consumer快,则产生额外一个线程处理该请求（while循环下线程产生速度是2的幂）
        if((!PromotionIsEmpty(pi))&&(pi->len>1)){

        }
        else if(PromotionIsEmpty(pi)){ //如果为空，则删除线程表中额外线程
            


        }

        // while(promotion_info.head == promotion_info.tail){
        //     pthread_cond_wait(&cache_cond, &cache_mutex);
        // }
        if(!PromotionIsEmpty(pi)){
        //取值并更新head
        tmp=PromotionPop(&promotion_info, data); //用data取数据（数据迁移），用tmp取指针（释放空间）
        //执行真正的数据promtoion（分配空间并写入）

        //promotion完成，检查相关条目（promotion==-1？）是否需要放弃此次promotion并free slab slot空间；
        //否则更新相关条目的指针


        // 释放原head占用空间
        free(tmp);
        tmp=NULL;
        }

    }
}

void InitPromotion(Promotion_Info *pi)//初始化
{
    redisAssert(pi != NULL);
    pi->promotionbuferhead = NULL;
    pi->promotionbufertail = NULL;
    pi->len = 0;
}

void InitP(){
    pthread_attr_t attr;
    pthread_t thread;
    void *arg = NULL;
    pthread_attr_init(&attr);
    if (pthread_create(&thread, &attr, HandlePromotion, arg) != 0) {
        redisLog(REDIS_WARNING,"Fatal: Can't initialize Promotion Threads.");
        exit(1);
    }
    haslab_cacheloop_tid[0] = thread;
    InitPromotion(&promotion_info);
}

bool PromotionIsEmpty(Promotion_Info *pi)//判断是否为空
{
    if(pi->len==0){	//若前后指针指向同一个节点，则判断为空
    
        return true;
    }
    else
        return false;
}

/*bool PromotionIsFull(void)//判断是否为满
{
    if(pi->head==(pi->tail+1)%HASLAB_BUFFER_SIZE)	//若前指针等于(后指针+1)%数组大小，则判断为满
        return true;
    else
        return false;
}*/

void PromotionPush(Promotion_Info *pi, void* key)//进队,由主线程执行
{
    PromotionBufferNode* new_node=NULL;
    new_node=(PromotionBufferNode*)malloc(sizeof(PromotionBufferNode));
    alloc_assert(new_node, "new_node");
	memset(new_node, 0, sizeof(PromotionBufferNode));

    new_node->data=key;

    pthread_mutex_lock(&cache_mutex);

    if(!PromotionIsEmpty(pi)){// buffer is not empty
        pi->promotionbufertail->next=new_node;
        pi->promotionbufertail=new_node;
    }
    else{
        pi->promotionbuferhead=new_node;
        pi->promotionbufertail=new_node;
    }
        pi->len++;
    pthread_mutex_unlock(&cache_mutex);
}

PromotionBufferNode* PromotionPop(Promotion_Info *pi, (void *) & data)//出从队首去取
{
    
    PromotionBufferNode *tmp=NULL;
    if(PromotionIsEmpty(pi))	//若队列为空，则出队失败
        return tmp;
    else{
        pthread_mutex_lock(&cache_mutex);
        tmp=pi->promotionbuferhead;
        pi->promotionbuferhead = tmp->next;
        pi->len--;
        pthread_mutex_unlock(&cache_mutex);
        data=tmp->data;
        return tmp;
    }
}

void promotionbufferdelete(Promotion_Info *pi){//遍历每一个

    PromotionBufferNode *tmp=NULL;
    
    pthread_mutex_lock(&cache_mutex);

    tmp=pi->promotionbuferhead;
    

};