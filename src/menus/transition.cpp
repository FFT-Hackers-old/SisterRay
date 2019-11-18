#include "transition.h"

void controlStateView(const TransitionData& transition){
    auto ctx = getGraphicsCtx();
    runContextCallbacks();
    else if (!sub_41AB67(128) && !(byte_DC206D != 2 && byte_DC206E != 2 && byte_DC206F != 2 && byte_DC206C != 2)) {
        setViewport(
            transition.field_8 + dword_DC105C + transition.field_0,
            transition.field_A + dword_DC1060 + transition.field_2,
            transition.field_C,
            20,
            ctx);
    }
    setViewport(
        transition.field_8 + dword_DC105C + transition.field_0,
        transition.field_A + dword_DC1060 + transition.field_2,
        transition.field_C,
        transition.field_E,
        ctx);
}
