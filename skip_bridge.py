import ctypes
import os
from typing import List, Tuple

lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../libskip_list.so"))

class SkipListStruct(ctypes.Structure):
    pass

skip_lib = ctypes.CDLL(lib_path)

skip_lib.skiplist_create.argtypes = []
skip_lib.skiplist_create.restype = ctypes.POINTER(SkipListStruct)

skip_lib.skiplist_free.argtypes = [ctypes.POINTER(SkipListStruct)]
skip_lib.skiplist_free.restype = None

skip_lib.skiplist_insert.argtypes = [
    ctypes.POINTER(SkipListStruct),
    ctypes.c_double,
    ctypes.c_int
]
skip_lib.skiplist_insert.restype = None

skip_lib.skiplist_get_top_k.argtypes = [
    ctypes.POINTER(SkipListStruct),
    ctypes.c_size_t,
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_double)
]
skip_lib.skiplist_get_top_k.restype = ctypes.c_int

class SkipListReranker:
    def __init__(self):
        self.list_ptr = skip_lib.skiplist_create()
        if not self.list_ptr:
            raise RuntimeError("Failed to allocate SkipList")

    def __del__(self):
        if hasattr(self, 'list_ptr') and self.list_ptr:
            skip_lib.skiplist_free(self.list_ptr)

    def add_candidate(self, doc_id: int, score: float):
        skip_lib.skiplist_insert(self.list_ptr, ctypes.c_double(score), ctypes.c_int(doc_id))

    def get_top_k(self, k: int) -> List[Tuple[int, float]]:
        out_ids = (ctypes.c_int * k)()
        out_scores = (ctypes.c_double * k)()
        
        extracted = skip_lib.skiplist_get_top_k(self.list_ptr, k, out_ids, out_scores)
        return [(out_ids[i], float(out_scores[i])) for i in range(extracted)]
