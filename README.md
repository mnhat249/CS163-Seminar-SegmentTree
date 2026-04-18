# CS163-Seminar-SegmentTree

## Brief Notes: Implementation

This project implements a Segment Tree for range sum queries with lazy propagation.

- Data structure:
	- `array`: original values.
	- `tree`: segment tree nodes, size about `4 * n`.
	- `lazy`: delayed updates for range-add operations.
- Main operations:
	- `build(node, left, right)`: recursively builds tree from `array`.
	- `update(node, left, right, pos, new_value)`: point update.
	- `query(node, left, right, query_L, query_R)`: range sum query.
	- `Delete(pos)`: simulates deletion by setting value at `pos` to `0`.
	- `updateRange(node, left, right, update_L, update_R, value)`: range add with lazy propagation.
- Lazy propagation idea:
	- If a segment is fully covered, update the node immediately and store pending value in `lazy[node]`.
	- Push pending updates to children only when visiting deeper nodes (`push`).

## Brief Notes: Complexity

Let `n` be the number of elements.

- Build: `O(n)`
- Point update: `O(log n)`
- Range query: `O(log n)`
- Range update (lazy): `O(log n)`
- Deletion (via point update): `O(log n)`
- Space usage: `O(n)` (tree + lazy arrays)

## Brief Notes: Evaluation

Evaluation was done using a small functional test in `SegmentTreeTest.cpp` (`smallTest()`).

- Scenario 1: Build from `{1,2,3,4,5}` and query `[0,4]` -> expected `15`.
- Scenario 2: Point update `pos=2` to `10` and query `[0,4]` -> expected `22`.
- Scenario 3: Range add `+1` on `[1,3]` and query `[0,4]` -> expected `25`.
- Scenario 4: Delete `pos=4` (set to `0`) and query `[0,4]` -> expected `20`.

All scenarios pass in the current implementation.

For larger datasets, Segment Tree with lazy propagation is expected to outperform naive iteration for repeated range updates and range queries, because each operation is reduced from linear time to logarithmic time.