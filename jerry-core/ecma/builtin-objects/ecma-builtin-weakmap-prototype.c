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

#include "ecma-container-object.h"

#if ENABLED (JERRY_BUILTIN_WEAKMAP)

#define ECMA_BUILTINS_INTERNAL
#include "ecma-builtins-internal.h"

/**
 * This object has a custom dispatch function.
 */
#define BUILTIN_CUSTOM_DISPATCH
#define BUILTIN_INC_HEADER_NAME "ecma-builtin-weakmap-prototype.inc.h"
#define BUILTIN_UNDERSCORED_ID weakmap_prototype
#include "ecma-builtin-internal-routines-template.inc.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmabuiltins
 * @{
 *
 * \addtogroup weakmap ECMA WeakMap object built-in
 * @{
 */
/**
 * Dispatcher of the built-in's routines
 *
 * @return ecma value
 *         Returned value must be freed with ecma_free_value.
 */
ecma_value_t
ecma_builtin_weakmap_prototype_dispatch_routine (ecma_func_args_t *func_args_p, /**< function arguments */
                                                 uint16_t builtin_routine_id) /**< builtin-routine ID */
{
  return ecma_builtin_container_dispatch_routine (func_args_p,
                                                  builtin_routine_id,
                                                  LIT_MAGIC_STRING_WEAKMAP_UL);
} /* ecma_builtin_weakmap_prototype_dispatch_routine */
/**
 * @}
 * @}
 * @}
 */

#endif /* ENABLED (JERRY_BUILTIN_WEAKMAP) */
