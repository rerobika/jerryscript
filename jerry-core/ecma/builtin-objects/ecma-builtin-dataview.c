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
#include "ecma-dataview-object.h"

#if ENABLED (JERRY_BUILTIN_DATAVIEW)

#define ECMA_BUILTINS_INTERNAL
#include "ecma-builtins-internal.h"

#define BUILTIN_INC_HEADER_NAME "ecma-builtin-dataview.inc.h"
#define BUILTIN_UNDERSCORED_ID dataview
#include "ecma-builtin-internal-routines-template.inc.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmabuiltins
 * @{
 *
 * \addtogroup dataview ECMA DataView object built-in
 * @{
 */

/**
 * Handle [[Call]]/[[Construct]] of built-in DataView object
 *
 * @return ecma value
 */
ecma_value_t
ecma_builtin_dataview_dispatch (ecma_func_args_t *func_args_p) /**< function arguments */
{
  JERRY_ASSERT (func_args_p != NULL);

  if (func_args_p->new_target_p == NULL)
  {
    return ecma_raise_type_error (ECMA_ERR_MSG ("Constructor DataView requires 'new'."));
  }

  return ecma_op_dataview_create (func_args_p);
} /* ecma_builtin_dataview_dispatch */

/**
 * @}
 * @}
 * @}
 */

#endif /* ENABLED (JERRY_BUILTIN_DATAVIEW */
