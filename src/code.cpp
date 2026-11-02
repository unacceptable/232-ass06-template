// Robert Jackson
// CSCI 232 Fall 2026
// Programming Assignment #6
// I declare that I am the author of this work, take full responsibility for it, and have disclosed any material external assistance.
// I used Claude (Anthropic) to write the implementations step by step and to explain the C++ concepts behind each one.
// I reviewed, tested, and verified the submitted implementation myself.

// ============================================================
// CSCI 232 Assignment 06 – Doubly Linked List vs. std::list
// Student Implementation
// ============================================================

#include "code.hpp"
#include <algorithm>
#include <iostream>

std::string AUTHOR_NAME       = "Robert Jackson";
std::string AUTHOR_AUTHORSHIP = "I acknowledge that I have worked on this "
"assignment independently, except where explicitly noted and referenced. "
"Any collaboration or use of external resources has been properly cited. "
"I am fully aware of the consequences of academic dishonesty and agree "
"to abide by the university's academic integrity policy.";

// ============================================================
// PART 1: Class DoubleNode
// ============================================================

/// Constructor: initialize ALL THREE fields with a member initializer list.
/// value <- newValue, prevPtr <- nullptr, nextPtr <- nullptr.
DoubleNode::DoubleNode(NodeValue newValue)
    : value(newValue), prevPtr(nullptr), nextPtr(nullptr)
{
}

/// Const getter: return the stored value.
NodeValue DoubleNode::getValue() const
{
    return value;
}

/// Const getter: return the previous node (or nullptr).
DoubleNode *DoubleNode::getPrev() const
{
    return prevPtr;
}

/// Const getter: return the next node (or nullptr).
DoubleNode *DoubleNode::getNext() const
{
    return nextPtr;
}

/// Setter: store newPrevPtr in prevPtr.
void DoubleNode::setPrev(DoubleNode *newPrevPtr)
{
    prevPtr = newPrevPtr;
}

/// Setter: store newNextPtr in nextPtr.
void DoubleNode::setNext(DoubleNode *newNextPtr)
{
    nextPtr = newNextPtr;
}

// ============================================================
// PART 2: Class DoublyLinkedList
// ============================================================

/// Empty list: headPtr = nullptr, tailPtr = nullptr, count = 0.
DoublyLinkedList::DoublyLinkedList()
    : headPtr(nullptr), tailPtr(nullptr), count(0)
{
}

/// RAII: delete ALL nodes. Save the next pointer BEFORE delete.
DoublyLinkedList::~DoublyLinkedList()
{
    DoubleNode *currentPtr = headPtr;
    while (currentPtr != nullptr)
    {
        DoubleNode *nextPtr = currentPtr->getNext();   // save before delete
        delete currentPtr;
        currentPtr = nextPtr;
    }
}

/// Unlink the first node and RETURN it (do not delete it).
/// Fix headPtr, tailPtr, count. Clear the node's own prevPtr and nextPtr.
/// Return nullptr if the list is empty.
DoubleNode *DoublyLinkedList::detachFirst()
{
    if (headPtr == nullptr)
    {
        return nullptr;
    }

    DoubleNode *firstPtr = headPtr;
    headPtr = firstPtr->getNext();

    if (headPtr == nullptr)
    {
        tailPtr = nullptr;              // it was the only node
    }
    else
    {
        headPtr->setPrev(nullptr);      // the new head has nothing before it
    }

    firstPtr->setNext(nullptr);
    firstPtr->setPrev(nullptr);
    count--;
    return firstPtr;
}

/// Unlink the last node and RETURN it (do not delete it).
/// Use the tail's prev link. Return nullptr if the list is empty.
DoubleNode *DoublyLinkedList::detachLast()
{
    if (tailPtr == nullptr)
    {
        return nullptr;
    }

    DoubleNode *lastPtr = tailPtr;
    tailPtr = lastPtr->getPrev();

    if (tailPtr == nullptr)
    {
        headPtr = nullptr;              // it was the only node
    }
    else
    {
        tailPtr->setNext(nullptr);      // the new tail has nothing after it
    }

    lastPtr->setNext(nullptr);
    lastPtr->setPrev(nullptr);
    count--;
    return lastPtr;
}

/// Find the FIRST node equal to value and unlink it (head, tail, only node
/// or middle node). Return nullptr if not found. Reuse detachFirst/detachLast.
DoubleNode *DoublyLinkedList::detachValue(NodeValue value)
{
    DoubleNode *currentPtr = headPtr;
    while (currentPtr != nullptr && currentPtr->getValue() != value)
    {
        currentPtr = currentPtr->getNext();
    }

    if (currentPtr == nullptr)
    {
        return nullptr;                 // not found
    }
    if (currentPtr == headPtr)
    {
        return detachFirst();           // also covers the only node
    }
    if (currentPtr == tailPtr)
    {
        return detachLast();
    }

    // middle node: TWO link repairs, one in each direction
    currentPtr->getPrev()->setNext(currentPtr->getNext());
    currentPtr->getNext()->setPrev(currentPtr->getPrev());
    currentPtr->setPrev(nullptr);
    currentPtr->setNext(nullptr);
    count--;
    return currentPtr;
}

/// Create a node with new and link it in front. Handle the empty list.
void DoublyLinkedList::pushFrontValue(NodeValue value)
{
    DoubleNode *newPtr = new DoubleNode(value);

    if (headPtr == nullptr)
    {
        headPtr = newPtr;               // empty list: both head and tail
        tailPtr = newPtr;
    }
    else
    {
        newPtr->setNext(headPtr);
        headPtr->setPrev(newPtr);
        headPtr = newPtr;
    }
    count++;
}

/// Create a node with new and link it at the end. Handle the empty list.
void DoublyLinkedList::pushBackValue(NodeValue value)
{
    DoubleNode *newPtr = new DoubleNode(value);

    if (tailPtr == nullptr)
    {
        headPtr = newPtr;               // empty list: both head and tail
        tailPtr = newPtr;
    }
    else
    {
        newPtr->setPrev(tailPtr);
        tailPtr->setNext(newPtr);
        tailPtr = newPtr;
    }
    count++;
}

/// Detach the first node, read its value, delete the node, return the value.
/// Empty list -> std::nullopt.
std::optional<NodeValue> DoublyLinkedList::popFrontValue()
{
    DoubleNode *firstPtr = detachFirst();
    if (firstPtr == nullptr)
    {
        return std::nullopt;
    }

    NodeValue value = firstPtr->getValue();
    delete firstPtr;
    return value;
}

/// Detach the last node, read its value, delete the node, return the value.
/// Empty list -> std::nullopt.
std::optional<NodeValue> DoublyLinkedList::popBackValue()
{
    DoubleNode *lastPtr = detachLast();
    if (lastPtr == nullptr)
    {
        return std::nullopt;
    }

    NodeValue value = lastPtr->getValue();
    delete lastPtr;
    return value;
}

/// Remove the FIRST node equal to value and free it.
/// Return true if something was removed.
bool DoublyLinkedList::deleteValue(NodeValue value)
{
    DoubleNode *foundPtr = detachValue(value);
    if (foundPtr == nullptr)
    {
        return false;
    }

    delete foundPtr;
    return true;
}

/// Return true if any node holds value.
bool DoublyLinkedList::containsValue(NodeValue value) const
{
    for (DoubleNode *currentPtr = headPtr; currentPtr != nullptr; currentPtr = currentPtr->getNext())
    {
        if (currentPtr->getValue() == value)
        {
            return true;
        }
    }
    return false;
}

/// Print values head -> tail with std::visit (each followed by a space,
/// then a newline). Return the number of printed values.
int DoublyLinkedList::printForward() const
{
    int printed = 0;
    for (DoubleNode *currentPtr = headPtr; currentPtr != nullptr; currentPtr = currentPtr->getNext())
    {
        std::visit([](const auto &item) { std::cout << item << " "; }, currentPtr->getValue());
        printed++;
    }
    std::cout << "\n";
    return printed;
}

/// Print values tail -> head using prevPtr. Return the number of printed values.
int DoublyLinkedList::printBackward() const
{
    int printed = 0;
    for (DoubleNode *currentPtr = tailPtr; currentPtr != nullptr; currentPtr = currentPtr->getPrev())
    {
        std::visit([](const auto &item) { std::cout << item << " "; }, currentPtr->getValue());
        printed++;
    }
    std::cout << "\n";
    return printed;
}

/// Return count.
int DoublyLinkedList::length() const
{
    return count;
}

// ===== BEGIN PART 3 (do not remove this marker) =====
// ============================================================
// PART 3: Class StdDoublyLinkedList
// ============================================================
// Only std::list methods, <algorithm>, iterators and std::visit.
// A loop is allowed ONLY for printing.

/// Which std::list method inserts at the front?
void StdDoublyLinkedList::pushFrontValue(NodeValue value)
{
    data.push_front(value);
}

/// Which std::list method inserts at the back?
void StdDoublyLinkedList::pushBackValue(NodeValue value)
{
    data.push_back(value);
}

/// Read the front element, remove it, return it. Empty -> std::nullopt.
std::optional<NodeValue> StdDoublyLinkedList::popFrontValue()
{
    if (data.empty())
    {
        return std::nullopt;            // front() on an empty list is undefined
    }

    NodeValue value = data.front();
    data.pop_front();
    return value;
}

/// Read the back element, remove it, return it. Empty -> std::nullopt.
std::optional<NodeValue> StdDoublyLinkedList::popBackValue()
{
    if (data.empty())
    {
        return std::nullopt;            // back() on an empty list is undefined
    }

    NodeValue value = data.back();
    data.pop_back();
    return value;
}

/// Remove only the FIRST element equal to value. Return true if removed.
bool StdDoublyLinkedList::deleteValue(NodeValue value)
{
    // std::list::remove would take out EVERY match, so find the first one
    // and erase just that element.
    auto found = std::find(data.begin(), data.end(), value);
    if (found == data.end())
    {
        return false;
    }

    data.erase(found);
    return true;
}

/// Search without writing your own loop.
bool StdDoublyLinkedList::containsValue(NodeValue value) const
{
    return std::find(data.begin(), data.end(), value) != data.end();
}

/// Print head -> tail (same format as Part 2). Return the number of values.
int StdDoublyLinkedList::printForward() const
{
    for (const NodeValue &item : data)
    {
        std::visit([](const auto &shown) { std::cout << shown << " "; }, item);
    }
    std::cout << "\n";
    return length();
}

/// Print tail -> head (same format as Part 2). Return the number of values.
int StdDoublyLinkedList::printBackward() const
{
    for (auto it = data.rbegin(); it != data.rend(); ++it)
    {
        std::visit([](const auto &shown) { std::cout << shown << " "; }, *it);
    }
    std::cout << "\n";
    return length();
}

/// size() returns size_t, length() returns int.
int StdDoublyLinkedList::length() const
{
    return static_cast<int>(data.size());
}
// ===== END PART 3 (do not remove this marker) =====
