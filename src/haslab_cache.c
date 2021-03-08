#include "haslab_cache.h"
#include "redis.h"

#define HASLAB_BUFFER_SIZE 1024
#define HASLAB_CACHE_NUM_OPS 2

static struct{
    void* buffer[HASLAB_BUFFER_SIZE];
    int head; //下标 后台线程去取出
    int tail; //下标 主线程插入
} promotion_info;

int aaa = 0;
bool to_promotion;

//只用锁，不用条件变量
//pthread_t cache_threads[HASLAB_CACHE_NUM_OPS];
pthread_mutex_t cache_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cache_cond = PTHREAD_COND_INITIALIZER;
//pthread_mutex_t dict_mutex;

pthread_t haslab_cacheloop_pid;

void handle_promotion(dictEntry *de, robj* val){
    while(true){
        pthread_mutex_lock(&cache_mutex);
        while(!to_promotion){
            pthread_cond_wait(&cache_cond, &cache_mutex);
        }
        pthread_mutex_unlock(&cache_mutex);
        printf("aaa = %d\n", aaa);
        to_promotion = false;
    }
}

void promotion_callback(void* key){
    //检查条目是否存在


    //更新条目信息
   //pthread_mutex_lock(&dict_mutex);
    
    
   //pthread_mutex_unlock(&dict_mutex);
}