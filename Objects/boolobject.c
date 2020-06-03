#include "boolobject.h"


/*static ORG_Bool * _bool_new_imp() {
    ORG_Bool *v = (ORG_Bool *) org_malloc(sizeof(ORG_Bool));
    ((ORG_Value *)v)->type = ORG_BOOLEAN_VALUE;
    return v;
}*/





ORG_Bool _ORG_TrueStruct = {
    { ORG_BOOLEAN_VALUE, { ORG_TRUE }},
    ORG_TRUE,
};
ORG_Bool _ORG_FalseStruct = {
    { ORG_BOOLEAN_VALUE, { ORG_FALSE }},
    ORG_FALSE,
};
