// Copyright (c) 2015, the Dartino project authors. Please see the AUTHORS file
// for details. all rights reserved. use of this source code is governed by a
// BSD-style license that can be found in the LICENSE.md file.

#include "src/shared/assert.h"
#include "src/shared/flags.h"
#include "src/shared/test_case.h"

#include "src/vm/priority_heap.h"

namespace dartino {

TEST_CASE(PRIORITY_HEAP_WITH_INDEX__INCREASING) {
  const word kSize = PriorityHeapWithValueIndex<word, word>::kMinimumSize;

  PriorityHeapWithValueIndex<word, word> heap;
  for (word i = 0; i < kSize; i++) {
    heap.Insert(i, 10 + i);
  }
  ASSERT(heap.backing_size() == kSize);
  for (word i = 0; i < kSize; i++) {
    EXPECT(!heap.IsEmpty());
    EXPECT_EQ(i, heap.Minimum().priority);
    EXPECT_EQ(10 + i, heap.Minimum().value);
    EXPECT(heap.ContainsValue(10 + i));
    heap.RemoveMinimum();
    EXPECT(!heap.ContainsValue(10 + i));
  }
  EXPECT(heap.IsEmpty());
}

TEST_CASE(PRIORITY_HEAP_WITH_INDEX__DECREASING) {
  const word kSize = PriorityHeapWithValueIndex<word, word>::kMinimumSize;

  PriorityHeapWithValueIndex<word, word> heap;
  for (word i = kSize - 1; i >= 0; i--) {
    heap.Insert(i, 10 + i);
  }
  ASSERT(heap.backing_size() == kSize);
  for (word i = 0; i < kSize; i++) {
    EXPECT(!heap.IsEmpty());
    EXPECT_EQ(i, heap.Minimum().priority);
    EXPECT_EQ(10 + i, heap.Minimum().value);
    EXPECT(heap.ContainsValue(10 + i));
    heap.RemoveMinimum();
    EXPECT(!heap.ContainsValue(10 + i));
  }
  EXPECT(heap.IsEmpty());
}

TEST_CASE(PRIORITY_HEAP_WITH_INDEX__DELETE_BY_VALUES) {
  const word kSize = PriorityHeapWithValueIndex<word, word>::kMinimumSize;

  PriorityHeapWithValueIndex<word, word> heap;
  for (word i = kSize - 1; i >= 0; i--) {
    heap.Insert(i, 10 + i);
  }

  ASSERT(heap.backing_size() == kSize);

  EXPECT(heap.RemoveByValue(10 + 0));
  EXPECT(!heap.RemoveByValue(10 + 0));

  EXPECT(heap.RemoveByValue(10 + 5));
  EXPECT(!heap.RemoveByValue(10 + 5));

  EXPECT(heap.RemoveByValue(10 + kSize - 1));
  EXPECT(!heap.RemoveByValue(10 + kSize - 1));

  for (word i = 0; i < kSize; i++) {
    // Jump over the removed [i]s in the loop.
    if (i != 0 && i != 5 && i != (kSize - 1)) {
      EXPECT(!heap.IsEmpty());
      EXPECT_EQ(i, heap.Minimum().priority);
      EXPECT_EQ(10 + i, heap.Minimum().value);
      EXPECT(heap.ContainsValue(10 + i));
      heap.RemoveMinimum();
      EXPECT(!heap.ContainsValue(10 + i));
    }
  }
  EXPECT(heap.IsEmpty());
}

TEST_CASE(PRIORITY_HEAP_WITH_INDEX__GROW_SHRINK) {
  const word kSize = 1024;
  const word kMinimumSize =
      PriorityHeapWithValueIndex<word, word>::kMinimumSize;

  PriorityHeapWithValueIndex<word, word> heap;
  for (word i = 0; i < kSize; i++) {
    heap.Insert(i, 10 + i);
  }

  ASSERT(heap.backing_size() == kSize);

  for (word i = 0; i < kSize; i++) {
    EXPECT(!heap.IsEmpty());
    EXPECT_EQ(i, heap.Minimum().priority);
    EXPECT_EQ(10 + i, heap.Minimum().value);
    EXPECT(heap.ContainsValue(10 + i));
    heap.RemoveMinimum();
    EXPECT(!heap.ContainsValue(10 + i));
  }

  EXPECT(heap.IsEmpty());
  ASSERT(heap.backing_size() == kMinimumSize);

  for (word i = 0; i < kSize; i++) {
    heap.Insert(i, 10 + i);
  }

  for (word i = 0; i < kSize; i++) {
    EXPECT(!heap.IsEmpty());
    EXPECT_EQ(i, heap.Minimum().priority);
    EXPECT_EQ(10 + i, heap.Minimum().value);
    EXPECT(heap.ContainsValue(10 + i));
    heap.RemoveMinimum();
    EXPECT(!heap.ContainsValue(10 + i));
  }

  EXPECT(heap.IsEmpty());
  ASSERT(heap.backing_size() == kMinimumSize);
}

TEST_CASE(PRIORITY_HEAP_WITH_INDEX__CHANGE_PRIORITY) {
  const word kSize = PriorityHeapWithValueIndex<word, word>::kMinimumSize;

  PriorityHeapWithValueIndex<word, word> heap;
  for (word i = 0; i < kSize; i++) {
    if (i % 2 == 0) {
      heap.Insert(i, 10 + i);
    }
  }
  ASSERT(heap.backing_size() == kSize);
  for (word i = 0; i < kSize; i++) {
    bool was_inserted = i % 2 == 0;
    bool increase = i % 3 == 0;
    word new_priority = i + (increase ? 100 : -100);

    EXPECT(was_inserted != heap.InsertOrChangePriority(new_priority, 10 + i));
  }

  for (word i = 0; i < kSize; i++) {
    bool increase = i % 3 == 0;
    if (!increase) {
      word expected_priority = i + (increase ? 100 : -100);
      EXPECT(!heap.IsEmpty());
      EXPECT_EQ(expected_priority, heap.Minimum().priority);
      EXPECT_EQ(10 + i, heap.Minimum().value);
      EXPECT(heap.ContainsValue(10 + i));
      heap.RemoveMinimum();
      EXPECT(!heap.ContainsValue(10 + i));
    }
  }
  for (word i = 0; i < kSize; i++) {
    bool increase = i % 3 == 0;
    if (increase) {
      word expected_priority = i + (increase ? 100 : -100);
      EXPECT(!heap.IsEmpty());
      EXPECT_EQ(expected_priority, heap.Minimum().priority);
      EXPECT_EQ(10 + i, heap.Minimum().value);
      EXPECT(heap.ContainsValue(10 + i));
      heap.RemoveMinimum();
      EXPECT(!heap.ContainsValue(10 + i));
    }
  }
  EXPECT(heap.IsEmpty());
}

}  // namespace dartino.
