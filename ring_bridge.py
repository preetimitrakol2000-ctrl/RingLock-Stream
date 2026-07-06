import ctypes
import os
import sys

class RingBridge:
    def __init__(self):
        if not os.path.exists("./libringstream.so") and not os.path.exists("./libringstream.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libringstream.dll ring_lock_stream.c")
                lib_path = "./libringstream.dll"
            else:
                os.system("gcc -shared -fPIC -o libringstream.so ring_lock_stream.c")
                lib_path = "./libringstream.so"
        else:
            lib_path = "./libringstream.dll" if sys.platform.startswith("win") else "./libringstream.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_sync_ring.restype = ctypes.c_void_p
        self.lib.enqueue_stream_task.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]
        self.lib.enqueue_stream_task.restype = ctypes.c_int
        self.lib.dequeue_stream_task.argtypes = [ctypes.c_void_p, ctypes.POINTER(ctypes.c_int)]
        self.lib.dequeue_stream_task.restype = ctypes.c_char_p
        
        self.ring_ptr = self.lib.init_sync_ring()

    def stream_push(self, task_id: int, content: str) -> int:
        return self.lib.enqueue_stream_task(self.ring_ptr, task_id, content.encode('utf-8'))

    def stream_pop(self) -> tuple:
        c_id = ctypes.c_int(0)
        text_res = self.lib.dequeue_stream_task(self.ring_ptr, ctypes.byref(c_id))
        return c_id.value, text_res.decode('utf-8')
