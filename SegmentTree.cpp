#include "SegmentTree.h"

#include <algorithm>

SegmentTree::SegmentTree() : n(0) {}

SegmentTree::SegmentTree(const std::vector<long long>& values) : n(0) {
	setArray(values);
	if (n > 0) {
		build(1, 0, n - 1);
	}
}

void SegmentTree::setArray(const std::vector<long long>& values) {
	array = values;
	n = static_cast<int>(array.size());
	tree.assign(4 * std::max(1, n) + 4, 0);
	lazy.assign(4 * std::max(1, n) + 4, 0);
}

void SegmentTree::build(int node, int left, int right) {
	if (n == 0) {
		return;
	}

	if (left == right) {
		tree[node] = array[left];
		return;
	}

	int middle = left + (right - left) / 2;
	build(node * 2, left, middle);
	build(node * 2 + 1, middle + 1, right);
	tree[node] = tree[node * 2] + tree[node * 2 + 1];
}

int SegmentTree::size() const {
	return n;
}

const std::vector<long long>& SegmentTree::data() const {
	return array;
}

void SegmentTree::push(int node, int left, int right) {
	if (lazy[node] == 0 || left == right) {
		return;
	}

	int middle = left + (right - left) / 2;
	long long value = lazy[node];

	tree[node * 2] += value * (middle - left + 1);
	tree[node * 2 + 1] += value * (right - middle);
	lazy[node * 2] += value;
	lazy[node * 2 + 1] += value;
	lazy[node] = 0;
}

void SegmentTree::update(int node, int left, int right, int pos, long long new_value) {
	if (n == 0 || pos < left || pos > right) {
		return;
	}

	if (left == right) {
		array[pos] = new_value;
		tree[node] = new_value;
		lazy[node] = 0;
		return;
	}

	push(node, left, right);

	int middle = left + (right - left) / 2;
	if (pos <= middle) {
		update(node * 2, left, middle, pos, new_value);
	} else {
		update(node * 2 + 1, middle + 1, right, pos, new_value);
	}

	tree[node] = tree[node * 2] + tree[node * 2 + 1];
}

void SegmentTree::Delete(int pos) {
	if (pos < 0 || pos >= n || n == 0) {
		return;
	}

	update(1, 0, n - 1, pos, 0);
}

void SegmentTree::updateRange(int node, int left, int right, int update_L, int update_R, long long value) {
	if (n == 0 || update_R < left || right < update_L) {
		return;
	}

	if (update_L <= left && right <= update_R) {
		tree[node] += value * (right - left + 1);
		lazy[node] += value;
		return;
	}

	push(node, left, right);

	int middle = left + (right - left) / 2;
	updateRange(node * 2, left, middle, update_L, update_R, value);
	updateRange(node * 2 + 1, middle + 1, right, update_L, update_R, value);
	tree[node] = tree[node * 2] + tree[node * 2 + 1];
}

long long SegmentTree::query(int node, int left, int right, int query_L, int query_R) {
	if (n == 0 || query_R < left || right < query_L) {
		return 0;
	}

	if (query_L <= left && right <= query_R) {
		return tree[node];
	}

	push(node, left, right);

	int middle = left + (right - left) / 2;
	long long left_result = query(node * 2, left, middle, query_L, query_R);
	long long right_result = query(node * 2 + 1, middle + 1, right, query_L, query_R);
	return left_result + right_result;
}

