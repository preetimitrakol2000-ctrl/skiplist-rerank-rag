from bindings.skip_bridge import SkipListReranker

def main():
    documents = {
        101: "GraphRAG builds sub-graph hierarchies to answer holistic query structures.",
        102: "Hybrid search fuses sparse BM25 scores with dense vector similarities.",
        103: "Cross-encoder rerankers process query and candidate passages jointly.",
        104: "In-memory SkipLists grant logarithmic insertion and extraction efficiency.",
        105: "Contextual compression truncates non-essential tokens before prompting."
    }

    # Simulated cross-encoder / dense relevance scoring pairs
    scored_candidates = [
        (101, 0.621),
        (102, 0.894),
        (103, 0.945),
        (104, 0.712),
        (105, 0.833)
    ]

    reranker = SkipListReranker()
    for doc_id, score in scored_candidates:
        reranker.add_candidate(doc_id, score)

    top_k = reranker.get_top_k(k=3)

    print("=== Top Ranked Passages (SkipList Priority Queue) ===")
    for rank, (doc_id, score) in enumerate(top_k, 1):
        print(f"Rank {rank} | Score: {score:.3f} | ID: {doc_id} => {documents[doc_id]}")

if __name__ == "__main__":
    main()
