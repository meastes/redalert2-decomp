#define LIST_DECLARE_CONSTRUCTOR
#define GENERIC_LIST_IMPLEMENT_CONSTRUCTOR
#include "shared/ini_class.hpp"
GenericList::GenericList() { FirstNode.Link(&LastNode); }
