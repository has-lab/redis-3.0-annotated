#include "haslab_cache.h"
#include "redis.h"

#define HASLAB_BUFFER_SIZE 1024
#define HASLAB_CACHE_NUM_OPS 2

static struct{
    void* buffer[HASLAB_BUFFER_SIZE];
    int head; //下标 后台线程去取出
    int tail; //下标 主线程插入
} promotion_info;

static pthread_t cache_threads[HASLAB_CACHE_NUM_OPS];
static pthread_mutex_t cache_mutex[HASLAB_CACHE_NUM_OPS];
static pthread_cond_t cache_condvar[HASLAB_CACHE_NUM_OPS];

pthread_t haslab_cacheloop_pid;

void haslab_cache_loop(){

    printf("haslab \n");
    printf("%ld\n", haslab_cacheloop_pid);
    handle_promotion();
}

void handle_promotion(){
    printf("handle \n");

}