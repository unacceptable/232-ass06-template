// Robert Jackson
// CSCI 232 Fall 2026
// Programming Assignment #6
// I declare that I am the author of this work, take full responsibility for it, and have disclosed any material external assistance.
// I used Claude (Anthropic) to write the tests step by step and to explain the C++ concepts behind each one.
// I reviewed, tested, and verified the submitted implementation myself.

#ifndef UNITY_H
#define UNITY_H
#include "unity.h"
#endif

#include "code.hpp"
#include <iostream>

// ============================================================
// PART 2 Tests: DoublyLinkedList
// ============================================================

/// Create a DoublyLinkedList. Verify length() is 0 on a new list.
void test_new_list_is_empty(void)
{
    DoublyLinkedList list;

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, list.length(), "a new list should have length 0");
}

/// Create a DoublyLinkedList. Call pushFrontValue and pushBackValue.
/// Verify length() is 2.
void test_push_increases_length(void)
{
    DoublyLinkedList list;
    list.pushFrontValue(NodeValue{1});
    list.pushBackValue(NodeValue{2});

    TEST_ASSERT_EQUAL_INT_MESSAGE(2, list.length(), "two pushes should give length 2");
}

/// Create an EMPTY DoublyLinkedList.
/// Verify popFrontValue() and popBackValue() both have NO value (has_value() is false).
void test_pop_on_empty_list(void)
{
    DoublyLinkedList list;

    TEST_ASSERT_FALSE_MESSAGE(list.popFrontValue().has_value(), "popFrontValue on an empty list should have no value");
    TEST_ASSERT_FALSE_MESSAGE(list.popBackValue().has_value(), "popBackValue on an empty list should have no value");
}

/// Push 1, 2, 3 with pushBackValue.
/// Verify popBackValue() gives 3, then popFrontValue() gives 1, then length() is 1.
void test_pop_order(void)
{
    DoublyLinkedList list;
    list.pushBackValue(NodeValue{1});
    list.pushBackValue(NodeValue{2});
    list.pushBackValue(NodeValue{3});

    std::optional<NodeValue> last = list.popBackValue();
    TEST_ASSERT_TRUE_MESSAGE(last == NodeValue{3}, "popBackValue should give 3");

    std::optional<NodeValue> first = list.popFrontValue();
    TEST_ASSERT_TRUE_MESSAGE(first == NodeValue{1}, "popFrontValue should give 1");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, list.length(), "one value should be left");
}

/// Push one value, then call deleteValue with a value that is NOT in the list.
/// Verify it returns false and length() does not change.
void test_delete_value_not_found(void)
{
    DoublyLinkedList list;
    list.pushBackValue(NodeValue{1});

    TEST_ASSERT_FALSE_MESSAGE(list.deleteValue(NodeValue{99}), "deleteValue of a missing value should return false");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, list.length(), "length should not change");
}

/// Push 1, 2, 3 with pushBackValue. Call deleteValue(2).
/// Verify popBackValue() gives 3 and the next popBackValue() gives 1.
/// (popBackValue follows prevPtr: this fails if you forgot to repair the backward link.)
void test_delete_middle_keeps_links(void)
{
    DoublyLinkedList list;
    list.pushBackValue(NodeValue{1});
    list.pushBackValue(NodeValue{2});
    list.pushBackValue(NodeValue{3});

    TEST_ASSERT_TRUE_MESSAGE(list.deleteValue(NodeValue{2}), "deleteValue(2) should find the middle value");

    std::optional<NodeValue> last = list.popBackValue();
    TEST_ASSERT_TRUE_MESSAGE(last == NodeValue{3}, "popBackValue should give 3");

    std::optional<NodeValue> next = list.popBackValue();
    TEST_ASSERT_TRUE_MESSAGE(next == NodeValue{1}, "the next popBackValue should give 1 (backward link repaired)");
}

// ============================================================
// PART 3 Tests: StdDoublyLinkedList
// ============================================================

/// Push 1, 2, 1 with pushBackValue. Call deleteValue(NodeValue{1}).
/// Verify length() is 2 and popFrontValue() gives 2 (only the FIRST 1 was removed).
void test_std_delete_only_first(void)
{
    StdDoublyLinkedList list;
    list.pushBackValue(NodeValue{1});
    list.pushBackValue(NodeValue{2});
    list.pushBackValue(NodeValue{1});

    TEST_ASSERT_TRUE_MESSAGE(list.deleteValue(NodeValue{1}), "deleteValue(1) should find a 1");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, list.length(), "only one element should be removed");

    std::optional<NodeValue> front = list.popFrontValue();
    TEST_ASSERT_TRUE_MESSAGE(front == NodeValue{2}, "the front should now be 2 (the FIRST 1 was removed)");
}

/// Create an EMPTY StdDoublyLinkedList.
/// Verify popFrontValue() and popBackValue() have NO value and do not crash.
void test_std_pop_on_empty_list(void)
{
    StdDoublyLinkedList list;

    TEST_ASSERT_FALSE_MESSAGE(list.popFrontValue().has_value(), "popFrontValue on an empty list should have no value");
    TEST_ASSERT_FALSE_MESSAGE(list.popBackValue().has_value(), "popBackValue on an empty list should have no value");
}
