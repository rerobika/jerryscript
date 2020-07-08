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

#include "ecma-builtin-typedarray-helpers.h"

#if ENABLED (JERRY_BUILTIN_TYPEDARRAY)

#include "ecma-builtins.h"
#include "ecma-exceptions.h"
#include "ecma-gc.h"
#include "ecma-objects.h"
#include "ecma-typedarray-object.h"

#define ECMA_BUILTINS_INTERNAL
#include "ecma-builtins-internal.h"

/**
 * Common implementation of the [[Call]]/[[Construct]] of TypedArrays.
 *
 * @return ecma value of the new TypedArray object
 *         Returned value must be freed with ecma_free_value
 */
ecma_value_t
ecma_typedarray_helper_dispatch (ecma_func_args_t *func_arg_p, /**< function arguments */
                                 ecma_typedarray_type_t typedarray_id) /**< id of the typedarray */
{
  JERRY_ASSERT (func_arg_p != NULL);

  if (func_arg_p->new_target_p == NULL)
  {
    return ecma_raise_type_error (ECMA_ERR_MSG ("TypedArray is not a constructor"));
  }

  ecma_object_t *prototype_obj_p = ecma_builtin_get (ecma_typedarray_helper_get_prototype_id (typedarray_id));
  ecma_value_t val = ecma_op_create_typedarray (func_arg_p->argv,
                                                func_arg_p->argc,
                                                prototype_obj_p,
                                                ecma_typedarray_helper_get_shift_size (typedarray_id),
                                                typedarray_id);

  return val;
} /* ecma_typedarray_helper_dispatch */

#endif /* ENABLED (JERRY_BUILTIN_TYPEDARRAY) */
