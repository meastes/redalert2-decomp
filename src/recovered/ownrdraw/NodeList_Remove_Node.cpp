// void UnknownNodeList_006003D0::Remove_Node(TextNode *node) -- 0x00600410
// Unlinks the node from the list at +0x34 and frees it (nothing when it is
// not in the list).
#include "shared/unknown_node_list.hpp"

void NodeList::Remove_Node(TextNode *node)
{
    TextNode *previous = 0;
    TextNode *current = Head;
    while (current != 0) {
        if (node == current) {
            break;
        }
        previous = current;
        current = current->Next;
    }
    if (current != 0) {
        if (previous != 0) {
            previous->Next = current->Next;
        } else {
            Head = current->Next;
        }
        operator delete(current);
    }
}
