#ifndef RING_LOCK_STREAM_H
#define RING_LOCK_STREAM_H

typedef struct StreamTask StreamTask;
typedef struct SyncRingBuffer SyncRingBuffer;

SyncRingBuffer* init_sync_ring();
int enqueue_stream_task(SyncRingBuffer* srb, int seq_id, const char* text);
const char* dequeue_stream_task(SyncRingBuffer* srb, int* retrieved_id);

#endif
