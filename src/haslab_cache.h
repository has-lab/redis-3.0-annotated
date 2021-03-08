#include <stdbool.h>
#define HASLAB_BUFFER_SIZE 1024

typedef struct Promotion_Info {
    void* buffer[HASLAB_BUFFER_SIZE];
    int head; //下标 后台线程去取出
    int tail; //下标 主线程插入
} Promotion_Info;

void InitP();
void InitPromotion(Promotion_Info *pi);
bool PromotionIsEmpty(Promotion_Info *pi);
bool PromotionIsFull(Promotion_Info *pi);
void PromotionPush(Promotion_Info *pi, void* key);
void PromotionPop(Promotion_Info *pi, void *key);