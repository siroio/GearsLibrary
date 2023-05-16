#ifndef GEARS_FUNC_DEFINITION_H
#define GEARS_FUNC_DEFINITION_H

#include <Internal/HasFunctionObject.h>

HasFuncObject(Initialize);
HasFuncObject(BeginDraw);
HasFuncObject(DebugDraw);
HasFuncObject(EndDraw);
HasFuncObject(Finalize);

HasFuncObject(Start);
HasFuncObject(Update);
HasFuncObject(LateUpdate);
HasFuncObject(Draw);

#endif // !GEARS_FUNC_DEFINITION_H
