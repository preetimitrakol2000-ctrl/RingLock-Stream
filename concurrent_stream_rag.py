from ring_bridge import RingBridge

if __name__ == "__main__":
    stream_pipeline = RingBridge()

    # Enqueue pipeline data requests sequentially into raw memory ring cells
    stream_pipeline.stream_push(task_id=501, content="Execute Vector Search Route Alpha.")
    stream_pipeline.stream_push(task_id=502, content="Execute Vector Search Route Beta.")

    print("=== RINGLOCK-STREAM CONCURRENT MEMORY EXCHANGE ===")
    
    # Extract tasks via sliding atomic ring indexes
    id_a, query_a = stream_pipeline.stream_pop()
    print(f"[*] Worker Node extracted task sequence reference ID [{id_a}]:\n -> Run Command: {query_a}")

    id_b, query_b = stream_pipeline.stream_pop()
    print(f"[*] Worker Node extracted task sequence reference ID [{id_b}]:\n -> Run Command: {query_b}")
