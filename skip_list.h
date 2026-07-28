#define SKIP_LIST_H

#include <stddef.h>

#define MAX_LEVEL 16
#define SKIPLIST_P 0.5f

typedef struct SkipNode {
    double score;
    int doc_id;
    struct SkipNode **forward;
} SkipNode;

typedef struct {
    int level;
    size_t size;
    SkipNode *header;
} SkipList;

#ifdef __cplusplus
extern "C" {
#endif

SkipList* skiplist_create(void);
void skiplist_free(SkipList *list);
void skiplist_insert(SkipList *list, double score, int doc_id);
int skiplist_get_top_k(SkipList *list, size_t k, int *out_doc_ids, double *out_scores);

#ifdef __cplusplus
}
#endif

#endif // SKIP_LIST_H
