#include "haslab_cache.h"
#include "redis.h"

#define HASLAB_BUFFER_SIZE 1024
#define HASLAB_CACHE_NUM_OPS 2

static struct{
    void* buffer[HASLAB_BUFFER_SIZE];
    int head; //下标 后台线程去取出
    int tail; //下标 主线程插入
} promotion_info;

//只用锁，不用条件变量
static pthread_t cache_threads[HASLAB_CACHE_NUM_OPS];
//static pthread_mutex_t cache_mutex[HASLAB_CACHE_NUM_OPS];
//static pthread_cond_t cache_condvar[HASLAB_CACHE_NUM_OPS];
static pthread_mutex_t dict_mutex;

pthread_t haslab_cacheloop_pid;

void haslab_cache_loop(){

    printf("haslab \n");
    printf("%ld\n", haslab_cacheloop_pid);
    //handle_promotion();
}

void handle_promotion(dictEntry *de, robj* val){
    


}

void promotion_callback(void* key){
    //检查条目是否存在


    //更新条目信息
   pthread_mutex_lock(&dict_mutex);
    
    
   pthread_mutex_unlock(&dict_mutex);
}