# SkipList-Rerank-RAG

Probabilistic multi-level Skip List engine written in C for $O(\log N)$ real-time candidate score insertion, ordering, and context window truncation.

## Compilation & Execution

```bash
# Compile shared C library
gcc -O3 -shared -fPIC -Iinclude src/skip_list.c -o libskip_list.so

# Run python driver
python3 app/rerank_rag_app.py
