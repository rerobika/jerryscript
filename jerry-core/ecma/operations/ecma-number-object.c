/* Copyright JS Foundation and other contributors, http://js.foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ecma-alloc.h"
#include "ecma-builtins.h"
#include "ecma-exceptions.h"
#include "ecma-gc.h"
#include "ecma-globals.h"
#include "ecma-helpers.h"
#include "ecma-number-object.h"
#include "ecma-objects.h"
#include "ecma-objects-general.h"
#include "ecma-function-object.h"
#include "jcontext.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmanumberobject ECMA Number object related routines
 * @{
 */

/**
 * Number object creation operation.
 *
 * See also: ECMA-262 v5, 15.7.2.1
 *
 * @return ecma value
 *         Returned value must be freed with ecma_free_value
 */
ecma_value_t
ecma_op_create_number_object (ecma_value_t arg) /**< argument passed to the Number constructor */
{
  ecma_number_t num;
  ecma_value_t conv_to_num_completion = ecma_op_to_number (arg, &num);

  if (ECMA_IS_VALUE_ERROR (conv_to_num_completion))
  {
    return conv_to_num_completion;
  }

  conv_to_num_completion = ecma_make_number_value (num);
  ecma_builtin_id_t proto_id = ECMA_BUILTIN_ID_NUMBER_PROTOTYPE;
  ecma_object_t *prototype_obj_p = ecma_builtin_get (proto_id);
#if ENABLED (JERRY_ESNEXT)
  ecma_object_t *new_target = JERRY_CONTEXT (current_new_target);
  if (new_target)
  {
    prototype_obj_p = ecma_op_get_prototype_from_constructor (new_target, proto_id);
    if (JERRY_UNLIKELY (prototype_obj_p == NULL))
    {
      return ECMA_VALUE_ERROR;
    }
  }
#endif /* ENABLED (JERRY_ESNEXT) */
  ecma_object_t *object_p = ecma_create_object (prototype_obj_p,
                                                sizeof (ecma_extended_object_t),
                                                ECMA_OBJECT_TYPE_CLASS);

  ecma_extended_object_t *ext_object_p = (ecma_extended_object_t *) object_p;
  ext_object_p->u.class_prop.class_id = LIT_MAGIC_STRING_NUMBER_UL;

  /* Pass reference (no need to free conv_to_num_completion). */
  ext_object_p->u.class_prop.u.value = conv_to_num_completion;
#if ENABLED (JERRY_ESNEXT)
  if (new_target)
  {
    ecma_deref_object (prototype_obj_p);
  }
#endif /* ENABLED (JERRY_ESNEXT) */
  return ecma_make_object_value (object_p);
} /* ecma_op_create_number_object */

/**
 * @}
 * @}
 */
