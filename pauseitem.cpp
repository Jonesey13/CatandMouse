#include "pauseitem.h"

PauseItem::PauseItem(string Text1, voidptr SelectionFunction){
    Item=Text1;
    ActionFunction=SelectionFunction;
}
