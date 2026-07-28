#include "../include/skip_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static SkipNode* create_node(int level, double score, int doc_id) {
    SkipNode *n = (SkipNode*)malloc(sizeof(SkipNode));
    n->score = score;
    n->doc_id = doc_id;
    n->forward = (SkipNode**)malloc(sizeof(SkipNode*) * (level + 1));
    for (int i = 0; i <= level; i++) {
        n->forward[i] = NULL;
    }
    return n;
}

static int random_level(void) {
    int lvl = 0;
    while ((float)rand() / RAND_MAX < SKIPLIST_P && lvl < MAX_LEVEL) {
        lvl++;
    }
    return lvl;
}

SkipList* skiplist_create(void) {
    SkipList *list = (SkipList*)malloc(sizeof(SkipList));
    list->level = 0;
    list->size = 0;
    list->header = create_node(MAX_LEVEL, -1.0e9, -1);
    return list;
}

void skiplist_free(SkipList *list) {
    if (!list) return;
    SkipNode *current = list->header;
    while (current) {
        SkipNode *next = current->forward[0];
        free(current->forward);
        free(current);
        current = next;
    }
    free(list);
}

void skiplist_insert(SkipList *list, double score, int doc_id) {
    SkipNode *update[MAX_LEVEL + 1];
    SkipNode *current = list->header;

    // Descend levels to locate insertion point in descending order of score
    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->score > score) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    int r_level = random_level();
    if (r_level > list->level) {
        for (int i = list->level + 1; i <= r_level; i++) {
            update[i] = list->header;
        }
        list->level = r_level;
    }

    SkipNode *new_node = create_node(r_level, score, doc_id);
    for (int i = 0; i <= r_level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }

    list->size++;
}

int skiplist_get_top_k(SkipList *list, size_t k, int *out_doc_ids, double *out_scores) {
    if (!list || !out_doc_ids || !out_scores) return 0;

    SkipNode *curr = list->header->forward[0];
    size_t count = 0;

    while (curr && count < k) {
        out_doc_ids[count] = curr->doc_id;
        out_scores[count] = curr->score;
        count++;
        curr = curr->forward[0];
    }

    return (int)count;
}
