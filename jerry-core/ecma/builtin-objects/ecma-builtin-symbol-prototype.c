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
 #include "ecma-conversion.h"
 #include "ecma-exceptions.h"
 #include "ecma-gc.h"
 #include "ecma-globals.h"
 #include "ecma-helpers.h"
 #include "ecma-objects.h"
 #include "ecma-symbol-object.h"
 #include "ecma-try-catch-macro.h"
 #include "jrt.h"

#if ENABLED (JERRY_ESNEXT)

#define ECMA_BUILTINS_INTERNAL
#include "ecma-builtins-internal.h"

/**
 * This object has a custom dispatch function.
 */
#define BUILTIN_CUSTOM_DISPATCH

/**
 * List of built-in routine identifiers.
 */
enum
{
  ECMA_SYMBOL_PROTOTYPE_ROUTINE_START = ECMA_BUILTIN_ID__COUNT - 1,
  ECMA_SYMBOL_PROTOTYPE_VALUE_OF, /**< ECMA-262 v11, 19.4.3.4 */
  ECMA_SYMBOL_PROTOTYPE_TO_PRIMITIVE, /**< ECMA-262 v11, 19.4.3.5 */
  ECMA_SYMBOL_PROTOTYPE_TO_STRING, /**< ECMA-262 v11, 19.4.3.3 */
  ECMA_SYMBOL_PROTOTYPE_DESCRIPTION, /**< ECMA-262 v11, 19.4.3.2 */
};

#define BUILTIN_INC_HEADER_NAME "ecma-builtin-symbol-prototype.inc.h"
#define BUILTIN_UNDERSCORED_ID symbol_prototype
#include "ecma-builtin-internal-routines-template.inc.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmabuiltins
 * @{
 *
 * \addtogroup symbolprototype ECMA Symbol prototype object built-in
 * @{
 */

/**
 * Dispatcher of the Symbol built-in's routines
 *
 * @return ecma value
 *         Returned value must be freed with ecma_free_value.
 */
ecma_value_t
ecma_builtin_symbol_prototype_dispatch_routine (ecma_func_args_t *func_args_p, /**< function arguments */
                                                uint16_t builtin_routine_id) /**< builtin-routine ID */
{
  ecma_value_t sym = ecma_symbol_this_value (func_args_p->this_value);

  if (ECMA_IS_VALUE_ERROR (sym))
  {
    return sym;
  }

  if (builtin_routine_id < ECMA_SYMBOL_PROTOTYPE_TO_STRING)
  {
    return ecma_copy_value (sym);
  }

  if (builtin_routine_id == ECMA_SYMBOL_PROTOTYPE_TO_STRING)
  {
    return ecma_get_symbol_descriptive_string (sym);
  }

  JERRY_ASSERT (builtin_routine_id == ECMA_SYMBOL_PROTOTYPE_DESCRIPTION);
  ecma_string_t *symbol_p = ecma_get_symbol_from_value (sym);
  ecma_string_t *desc_p = ecma_get_symbol_description (symbol_p);
  ecma_ref_ecma_string (desc_p);

  return ecma_make_string_value (desc_p);
} /* ecma_builtin_symbol_prototype_dispatch_routine */

/**
 * @}
 * @}
 * @}
 */

#endif /* ENABLED (JERRY_ESNEXT) */
