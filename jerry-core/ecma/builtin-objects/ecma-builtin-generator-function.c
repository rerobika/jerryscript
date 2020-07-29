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

#include "ecma-globals.h"

#if ENABLED (JERRY_ESNEXT)

#define ECMA_BUILTINS_INTERNAL
#include "ecma-builtins-internal.h"
#include "ecma-function-object.h"

#define BUILTIN_INC_HEADER_NAME "ecma-builtin-generator-function.inc.h"
#define BUILTIN_UNDERSCORED_ID generator_function
#include "ecma-builtin-internal-routines-template.inc.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmabuiltins
 * @{
 *
 * \addtogroup generator ECMA GeneratorFunction object built-in
 * @{
 */

/**
 * Handle [[Call]]/[[Construct]] of built-in GeneratorFunction object
 *
 * @return constructed generator function object - if success
 *         raised error otherwise
 */
ecma_value_t
ecma_builtin_generator_function_dispatch (ecma_func_args_t *func_args_p) /**< function arguments */
{
  JERRY_ASSERT (func_args_p != NULL);

  return ecma_op_create_dynamic_function (func_args_p->argv, func_args_p->argc, ECMA_PARSE_GENERATOR_FUNCTION);
} /* ecma_builtin_generator_function_dispatch */

/**
 * @}
 * @}
 * @}
 */

#endif /* ENABLED (JERRY_ESNEXT) */
