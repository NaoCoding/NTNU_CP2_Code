#include "database.h"
static int64_t didSetup = 0;
static struct list_head * setTable;

int32_t setup_table( const struct list_head *pLabelList ){

    

    didSetup = 1;
    setTable = pLabelList;

    return 0;

}

int32_t add( struct list_head *pRecordList , sRecord *pRecord ){

    if(!didSetup) return -1;
    struct list_head * labelNow = setTable;
    struct list_head * records = pRecordList;
    struct list_head * pt = &pRecord->data;

    while(labelNow->next != NULL && pt->next != NULL){
        pt = pt->next;
        labelNow = labelNow->next;
    }
    if(pt->next!=NULL || labelNow != NULL) return -1;


    pRecord->list.next = pRecordList;
    
    pRecordList = &pRecord->list;

    return 0;
}

int32_t get_size( struct list_head *pRecordList ){

    if(!didSetup) return -1;
    int32_t count = 0;
    while(pRecordList){
        count += 1;
        pRecordList = pRecordList->next;
    }

    return count;
}

int32_t query( struct list_head *pResultList , struct list_head *pRecordList ,char *pCmd ){

    if(!didSetup) return -1;
    pResultList = NULL;
    return 0;
}