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

#include "ecma-builtins.h"
#include "ecma-exceptions.h"
#include "ecma-gc.h"
#include "ecma-globals.h"
#include "ecma-helpers.h"
#include "ecma-typedarray-object.h"
#include "jrt.h"

#if ENABLED (JERRY_BUILTIN_TYPEDARRAY)

#define ECMA_BUILTINS_INTERNAL
#include "ecma-builtins-internal.h"

#define BUILTIN_INC_HEADER_NAME "ecma-builtin-int32array.inc.h"
#define BUILTIN_UNDERSCORED_ID int32array
#include "ecma-builtin-internal-routines-template.inc.h"

#include "ecma-builtin-typedarray-helpers.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmabuiltins
 * @{
 *
 * \addtogroup int32array ECMA Int32Array object built-in
 * @{
 */

/**
 * Handle [[Call]]/[[Construct]] of Int32Array
 *
 * @return ecma value
 */
ecma_value_t
ecma_builtin_int32array_dispatch (ecma_func_args_t *func_args_p) /**< function arguments */
{
  JERRY_ASSERT (func_args_p != NULL);

  return ecma_typedarray_helper_dispatch (func_args_p, ECMA_INT32_ARRAY);
} /* ecma_builtin_int32array_dispatch */

/**
  * @}
  * @}
  * @}
  */

#endif /* ENABLED (JERRY_BUILTIN_TYPEDARRAY) */
