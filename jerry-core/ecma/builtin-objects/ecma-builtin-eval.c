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
#include "ecma-builtin-helpers.h"
#include "ecma-exceptions.h"
#include "ecma-eval.h"
#include "ecma-globals.h"
#include "ecma-helpers.h"
#include "jcontext.h"

#define ECMA_BUILTINS_INTERNAL
#include "ecma-builtins-internal.h"

#define BUILTIN_INC_HEADER_NAME "ecma-builtin-eval.inc.h"
#define BUILTIN_UNDERSCORED_ID eval
#include "ecma-builtin-internal-routines-template.inc.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmabuiltins
 * @{
 *
 * \addtogroup eval ECMA Eval object built-in
 * @{
 */

/**
 * Handle calling [[Call]] of built-in Eval object
 *
 * @return ECMA_VALUE_ERROR - if the array construction fails
 *         constructed array object - otherwise
 */
ecma_value_t
ecma_builtin_eval_dispatch_call (const ecma_value_t *arguments_list_p, /**< arguments list */
                                 uint32_t arguments_list_len) /**< number of arguments */
{
  JERRY_ASSERT (arguments_list_len == 0 || arguments_list_p != NULL);

  if (arguments_list_len < 1)
  {
    return ECMA_VALUE_UNDEFINED;
  }

  if (!ecma_is_value_string (arguments_list_p[0]))
  {
    return ecma_copy_value (arguments_list_p[0]);
  }

  return ecma_op_eval (ecma_get_string_from_value (arguments_list_p[0]), ECMA_PARSE_NO_OPTS);
} /* ecma_builtin_eval_dispatch_call */

/**
 * Handle calling [[Construct]] of built-in Eval object
 *
 * @return ECMA_VALUE_ERROR
 */
ecma_value_t
ecma_builtin_eval_dispatch_construct (const ecma_value_t *arguments_list_p, /**< arguments list */
                                       uint32_t arguments_list_len) /**< number of arguments */
{
  JERRY_ASSERT (arguments_list_len == 0 || arguments_list_p != NULL);

  return ecma_raise_type_error (ECMA_ERR_MSG ("eval is not a constructor"));
} /* ecma_builtin_eval_dispatch_construct */

/**
 * @}
 * @}
 * @}
 */
