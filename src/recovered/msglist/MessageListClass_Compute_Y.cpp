// void MessageListClass::Compute_Y() -- 0x005B5D80
// Puts the messages one under the other from the list's Y, one row lower when the edit line
// is open and takes the list's last row (IsEdit and AdjustEdit); names.csv
// ?Compute_Y@MessageListClass@@AAEXXZ (Westwood's Compute_Y).
#include "shared/text_label.hpp"
#include "shared/message_list.hpp"

void MessageListClass::Compute_Y()
{
    int y;
    if (IsEdit && AdjustEdit) {
        y = Y + Height;
    } else {
        y = Y;
    }
    TextLabelClass *node = First;
    while (node) {
        node->Y = y;
        node = (TextLabelClass *)node->Get_Next();
        y += Height;
    }
}
