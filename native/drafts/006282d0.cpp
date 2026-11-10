// int __fastcall Extract_Compressed_Events(void *buf, int bufsize) (0x006282D0; Red Alert's
// Extract_Compressed_Events with Red Alert 2's megamission unit lists).
#include "shared/queue.hpp"
#include "shared/crt_memory.hpp"
// NATIVE (Scout1209 r10): the type 0x1F data travels as a handle in the 32-bit field at +0x07
// (native/link/native_event_data.cpp); the size stays at +0x0B, as in the original.
int Native_Event_Data_Handle(void *data);
void *Native_Event_Data_Release(int handle);
extern unsigned char unknown_event_lengths_7d659c[];
extern xTargetClass unknown_unit_lists_aabed0[64][128];
extern int unknown_unit_counts_a76b74[64];
// @ref ?Frame@@3HA=00a40d2c
// @ref ?DoList@@3V?$QueueClass@VEventClass@@$0EAAA@@@A=00866270
// @ref ?unknown_event_lengths_7d659c@@3PAEA=007d659c
// @ref ?unknown_unit_lists_aabed0@@3PAY0IA@VxTargetClass@@A=00aabed0
// @ref ?unknown_unit_counts_a76b74@@3PAHA=00a76b74
// @ref ??_C@_0BK@JECE@Decoding?5new?5MEGAMISSION?6?$AA@=007ecec0
// @ref ??_C@_0CE@PFCF@Frame?5?$CFd?3Empty?5megamission?5decod@=007ece9c
// @ref ??_C@_0CM@NLGB@Frame?5?$CFd?3?5Empty?5event?5being?5adde@=007ecdc8
// @ref ??_C@_0DA@CDMK@Used?5old?5unit?5list?5in?5slot?5?$CFd?5?9?5@=007ecdf4
// @ref ??_C@_0DK@MOHO@Got?5new?5unit?5list?5for?5player?5?$CFd?5@=007ece60
// @ref ??_C@_0DM@COCM@Frame?5?$CFd?3Empty?5megamission?5decod@=007ece24

int __fastcall Extract_Compressed_Events(void *buf, int bufsize)
{
    int pos = 0;
    int leftover = bufsize;
    EventClass *event;
    int count = 0;
    int datasize = 0;
    EventClass eventdata;
    unsigned char numunits = 0;
    int house;
    int slot;
    int units;
    xTargetClass *list;
    char *src;

    memset(&eventdata, 0, sizeof(EventClass));
    house = ((EventClass *)buf)->House;
    datasize = 13;
    event = (EventClass *)(((char *)buf) + pos);

    while ((unsigned)leftover >= (datasize + sizeof(eventdata.Type))) {
        if (event->Type != 0x19) {
            if (event->Type == 0x1C) {
                eventdata.House = event->House;
                eventdata.Frame = event->Frame;
                pos += 6;
                leftover -= 6;
            } else if (event->Type == 4) {
                numunits = *(((unsigned char *)buf) + pos + sizeof(eventdata.Type));
                pos += sizeof(numunits);
                leftover -= sizeof(numunits);
            }
            memset(&eventdata.Data, 0, sizeof(eventdata.Data));
            eventdata.Type = event->Type;
            datasize = unknown_event_lengths_7d659c[eventdata.Type];

            switch (eventdata.Type) {
            case 0x1B:
                memcpy(&eventdata.Data.Bytes[6], ((char *)buf) + pos + sizeof(eventdata.Type), datasize);
                break;

            case 0x1F:
                memcpy(&eventdata.LongAt(0x0B), ((char *)buf) + pos + sizeof(eventdata.Type), datasize);
                {
                    char *variable = new char[eventdata.LongAt(0x0B)];
                    eventdata.LongAt(0x07) = Native_Event_Data_Handle(variable);
                    memcpy(variable, ((char *)buf) + pos + sizeof(eventdata.Type) + datasize, eventdata.LongAt(0x0B));
                }
                pos += eventdata.LongAt(0x0B);
                leftover -= eventdata.LongAt(0x0B);
                break;

            case 4:
                memcpy(&eventdata.Data, ((char *)buf) + pos + sizeof(eventdata.Type), datasize);
                if (numunits > 1) {
                    pos += (datasize + sizeof(eventdata.Type));
                    leftover -= (datasize + sizeof(eventdata.Type));
                    datasize = sizeof(eventdata.Data.MegaMissionF.Whom);
                    Debug_Printf("Decoding new MEGAMISSION\n");
                    slot = (char)eventdata.Data.Bytes[6] + house * 8;
                    unknown_unit_lists_aabed0[slot][0] = eventdata.Data.MegaMissionF.Whom;
                    units = 1;
                    unknown_unit_counts_a76b74[slot] = units;
                    if (eventdata.Frame == 0 || eventdata.Type == 0) {
                        Debug_Printf("Frame %d:Empty megamission decoded\n", ::Frame);
                    }
                    if (numunits) {
                        src = ((char *)buf) + pos;
                        list = &unknown_unit_lists_aabed0[slot][1];
                        do {
                            if (!DoList.Add(eventdata)) {
                                return -1;
                            }
                            count++;
                            *list = *(xTargetClass *)src;
                            eventdata.Data.MegaMissionF.Whom = *list++;
                            numunits--;
                            units++;
                            unknown_unit_counts_a76b74[slot] = units;
                            if (numunits == 1) {
                                datasize -= sizeof(eventdata.Type);
                                Debug_Printf("Got new unit list for player %d with %d units in slot %d\n", house, units, (char)eventdata.Data.Bytes[6]);
                                break;
                            }
                            pos += datasize;
                            src += datasize;
                            leftover -= datasize;
                        } while (numunits);
                    }
                } else if (numunits == 0) {
                    pos += (datasize + sizeof(eventdata.Type));
                    leftover -= (datasize + sizeof(eventdata.Type));
                    datasize = sizeof(eventdata.Data.MegaMissionF.Whom);
                    units = 1;
                    slot = (char)eventdata.Data.Bytes[6] + house * 8;
                    numunits = unknown_unit_counts_a76b74[slot];
                    eventdata.Data.MegaMissionF.Whom = unknown_unit_lists_aabed0[slot][0];
                    if (eventdata.Frame == 0 || eventdata.Type == 0) {
                        Debug_Printf("Frame %d:Empty megamission decoded from slot %d. Player %d\n", ::Frame, (char)eventdata.Data.Bytes[6], house);
                    }
                    if (numunits) {
                        list = &unknown_unit_lists_aabed0[slot][1];
                        do {
                            if (!DoList.Add(eventdata)) {
                                return -1;
                            }
                            count++;
                            numunits--;
                            units++;
                            eventdata.Data.MegaMissionF.Whom = *list++;
                            if (numunits == 1) {
                                datasize = -1;
                                Debug_Printf("Used old unit list in slot %d - %d bytes saved\n", (char)eventdata.Data.Bytes[6], units * 5);
                                break;
                            }
                        } while (numunits);
                    }
                }
                break;

            default:
                memcpy(&eventdata.Data, ((char *)buf) + pos + sizeof(eventdata.Type), datasize);
                break;
            }

            if (eventdata.Frame == 0 || eventdata.Type == 0) {
                Debug_Printf("Frame %d: Empty event being added to queue\n", ::Frame);
            }
            if (!DoList.Add(eventdata)) {
                if (eventdata.Type == 0x1F) {
                    delete[] (char *)Native_Event_Data_Release(eventdata.LongAt(0x07));
                }
                return -1;
            }
            count++;
            pos += (datasize + sizeof(eventdata.Type));
            leftover -= (datasize + sizeof(eventdata.Type));
            if (leftover) {
                event = (EventClass *)(((char *)buf) + pos);
                datasize = unknown_event_lengths_7d659c[event->Type];
                if (event->Type == 4) {
                    datasize += sizeof(numunits);
                }
            }
        } else {
            pos += (datasize + sizeof(eventdata.Type));
            leftover -= (datasize + sizeof(eventdata.Type));
            event = (EventClass *)(((char *)buf) + pos);
            datasize = 13;
        }
    }
    return count;
}
