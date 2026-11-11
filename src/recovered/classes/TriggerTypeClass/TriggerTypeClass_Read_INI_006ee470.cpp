// bool TriggerTypeClass::Read_INI(CCINIClass &ini) (0x006EE470, slot 25)
// The trigger's [Triggers] line (house, next trigger, name, disabled, easy,
// normal, hard and the +0xA0 flag), then its event and action lists from
// [Events] and [Actions] (a count, then each entry's own fields). Events are
// pushed on the front of the list, actions appended in order.
// @ref ?Read_INI@TActionClass@@QAEXXZ=006ad860
// @ref ??_C@_00A@?$AA@=0083c55c
// @ref ?Find_Index@HouseTypeClass@@SIHPBD@Z=004f94d0
// @ref ?HouseTypes@@3V?$DynamicVectorClass@PAVHouseTypeClass@@@@A=00a35d08
#include "shared/object.hpp"
#include "shared/trigger_type.hpp"
#include "shared/taction.hpp"
#include "shared/house_type.hpp"

bool TriggerTypeClass::Read_INI(CCINIClass &ini)
{
    char buffer[512];
    if (ini.Get_String(unknown_triggers_section_7ada50, ID, "", buffer, sizeof(buffer))) {
        char *token = strtok(buffer, ",");
        if (_stricmp(token, "<none>") == 0) {
            House = HouseTypes[0];
        } else {
            House = HouseTypes[HouseTypeClass::Find_Index(token)];
        }

        token = strtok(0, ",");
        LinkedTo = 0;
        if (_stricmp(token, "<none>") != 0) {
            LinkedTo = Find_Or_Make(token);
        }

        token = strtok(0, ",");
        Name = FixedString<49>(token);

        token = strtok(0, ",");
        if (token != 0 && atoi(token) == 0) {
            IsEnabled = true;
        } else {
            IsEnabled = false;
        }
        token = strtok(0, ",");
        if (token != 0 && atoi(token) != 0) {
            IsEnabledOnEasy = true;
        } else {
            IsEnabledOnEasy = false;
        }
        token = strtok(0, ",");
        if (token != 0 && atoi(token) != 0) {
            IsEnabledOnMedium = true;
        } else {
            IsEnabledOnMedium = false;
        }
        token = strtok(0, ",");
        if (token != 0 && atoi(token) != 0) {
            IsEnabledOnHard = true;
        } else {
            IsEnabledOnHard = false;
        }
        token = strtok(0, ",");
        if (token != 0 && atoi(token) != 0) {
            IsToInherit = true;
        }

        if (ini.Get_String(unknown_events_section_7ada58, ID, "", buffer, sizeof(buffer))) {
            int count = atoi(strtok(buffer, ","));
            for (; count != 0; count--) {
                TEventClass *event = new TEventClass();
                event->Read_INI();
                event->Next = FirstEvent;
                FirstEvent = event;
            }
        }

        if (ini.Get_String(unknown_actions_section_7ada54, ID, "", buffer, sizeof(buffer))) {
            int count = atoi(strtok(buffer, ","));
            TActionClass *last = 0;
            FirstAction = 0;
            for (; count != 0; count--) {
                TActionClass *action = new TActionClass();
                action->Read_INI();
                if (FirstAction == 0) {
                    FirstAction = action;
                } else {
                    last->Next = action;
                }
                last = action;
            }
        }
        return true;
    }
    return false;
}
