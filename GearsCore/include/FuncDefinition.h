#pragma once
#include <Internal/HasFunctionObject.h>

/* Normal Function */

HAS_FUNC_OBJECT(Initialize);
HAS_FUNC_OBJECT(BeginDraw);
HAS_FUNC_OBJECT(DebugDraw);
HAS_FUNC_OBJECT(Draw);
HAS_FUNC_OBJECT(DrawShadow);
HAS_FUNC_OBJECT(EndDraw);
HAS_FUNC_OBJECT(Finalize);
HAS_FUNC_OBJECT(Start);
HAS_FUNC_OBJECT(Update);
HAS_FUNC_OBJECT(FixedUpdate);
HAS_FUNC_OBJECT(LateUpdate);

/* Event Function */

HAS_FUNC_OBJECT(OnTriggerEnter);
HAS_FUNC_OBJECT(OnTriggerStay);
HAS_FUNC_OBJECT(OnTriggerExit);
HAS_FUNC_OBJECT(OnCollisionEnter);
HAS_FUNC_OBJECT(OnCollisionStay);
HAS_FUNC_OBJECT(OnCollisionExit);
