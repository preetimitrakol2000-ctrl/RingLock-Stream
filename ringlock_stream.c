#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define WINDOW_CAPACITY 3

typedef struct {
    int command_sequence_id;
    char text_query_payload[128];
    bool processing_locked;
} StreamTask;

typedef struct {
    StreamTask ring[WINDOW_CAPACITY];
    int read_pointer;
    int write_pointer;
    int element_counter;
} SyncRingBuffer;

#ifdef _WIN32
    __declspec(dllexport) SyncRingBuffer* init_sync_ring();
    __declspec(dllexport) int enqueue_stream_task(SyncRingBuffer* srb, int seq_id, const char* text);
    __declspec(dllexport) const char* dequeue_stream_task(SyncRingBuffer* srb, int* retrieved_id);
#endif

SyncRingBuffer* init_sync_ring() {
    SyncRingBuffer* srb = (SyncRingBuffer*)malloc(sizeof(SyncRingBuffer));
    srb->read_pointer = 0; srb->write_pointer = 0; srb->element_counter = 0;
    for (int i = 0; i < WINDOW_CAPACITY; i++) {
        srb->ring[i].processing_locked = false;
    }
    return srb;
}

int enqueue_stream_task(SyncRingBuffer* srb, int seq_id, const char* text) {
    if (srb->element_counter >= WINDOW_CAPACITY) return -1; // Buffer full condition boundary

    int idx = srb->write_pointer;
    srb->ring[idx].command_sequence_id = seq_id;
    strncpy(srb->ring[idx].text_query_payload, text, sizeof(srb->ring[idx].text_query_payload) - 1);
    srb->ring[idx].processing_locked = true;

    srb->write_pointer = (srb->write_pointer + 1) % WINDOW_CAPACITY;
    srb->element_counter++;
    return 0;
}

const char* dequeue_stream_task(SyncRingBuffer* srb, int* retrieved_id) {
    if (srb->element_counter == 0) return "STREAM_EMPTY";

    int idx = srb->read_pointer;
    *retrieved_id = srb->ring[idx].command_sequence_id;
    
    static char return_msg[128];
    strncpy(return_msg, srb->ring[idx].text_query_payload, sizeof(return_msg) - 1);
    
    srb->ring[idx].processing_locked = false;
    srb->read_pointer = (srb->read_pointer + 1) % WINDOW_CAPACITY;
    srb->element_counter--;

    return return_msg;
}
File 2: ring_lock_stream.h
C
#ifndef RING_LOCK_STREAM_H
#define RING_LOCK_STREAM_H

typedef struct StreamTask StreamTask;
typedef struct SyncRingBuffer SyncRingBuffer;

SyncRingBuffer* init_sync_ring();
int enqueue_stream_task(SyncRingBuffer* srb, int seq_id, const char* text);
const char* dequeue_stream_task(SyncRingBuffer* srb, int* retrieved_id);

#endif
