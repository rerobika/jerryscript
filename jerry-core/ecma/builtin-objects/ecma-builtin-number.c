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

#include <math.h>

#include "ecma-alloc.h"
#include "ecma-builtins.h"
#include "ecma-conversion.h"
#include "ecma-exceptions.h"
#include "ecma-gc.h"
#include "ecma-globals.h"
#include "ecma-helpers.h"
#include "ecma-number-object.h"
#include "ecma-objects.h"
#include "ecma-try-catch-macro.h"
#include "jrt.h"

#if ENABLED (JERRY_BUILTIN_NUMBER)

#define ECMA_BUILTINS_INTERNAL
#include "ecma-builtins-internal.h"

#define BUILTIN_INC_HEADER_NAME "ecma-builtin-number.inc.h"
#define BUILTIN_UNDERSCORED_ID number
#include "ecma-builtin-internal-routines-template.inc.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmabuiltins
 * @{
 *
 * \addtogroup number ECMA Number object built-in
 * @{
 */

/**
 * Handle [[Call]]/[[Construct]] of built-in Number object
 *
 * @return ecma value
 */
ecma_value_t
ecma_builtin_number_dispatch (ecma_func_args_t *func_args_p) /**< function arguments */
{
  JERRY_ASSERT (func_args_p != NULL);

  if (func_args_p->new_target_p == NULL)
  {
    if (func_args_p->argc == 0)
    {
      return ecma_make_integer_value (0);
    }

    return ecma_op_to_number (func_args_p->argv[0]);
  }

  if (func_args_p->argc == 0)
  {
    return ecma_op_create_number_object (ecma_make_integer_value (0));
  }

  return ecma_op_create_number_object (func_args_p->argv[0]);
} /* ecma_builtin_number_dispatch */

#if ENABLED (JERRY_ESNEXT)

/**
 * The Number object 'isFinite' routine
 *
 * See also:
 *          ECMA-262 v6, 20.1.2.2
 *
 * @return ecma value
 *         Returned value must be freed with ecma_free_value.
 */
static ecma_value_t
ecma_builtin_number_object_is_finite (ecma_value_t this_arg, /**< this argument */
                                      ecma_value_t arg) /**< routine's argument */
{
  JERRY_UNUSED (this_arg);

  if (ecma_is_value_number (arg))
  {
    ecma_number_t num = ecma_get_number_from_value (arg);
    if (!(ecma_number_is_nan (num) || ecma_number_is_infinity (num)))
    {
      return ECMA_VALUE_TRUE;
    }
  }
  return ECMA_VALUE_FALSE;
} /* ecma_builtin_number_object_is_finite */

/**
 * The Number object 'isNan' routine
 *
 * See also:
 *          ECMA-262 v6, 20.1.2.4
 *
 * @return ecma value
 *         Returned value must be freed with ecma_free_value.
 */
static ecma_value_t
ecma_builtin_number_object_is_nan (ecma_value_t this_arg, /**< this argument */
                                   ecma_value_t arg) /**< routine's argument */
{
  JERRY_UNUSED (this_arg);

  if (ecma_is_value_number (arg))
  {
    ecma_number_t num_val = ecma_get_number_from_value (arg);

    if (ecma_number_is_nan (num_val))
    {
      return ECMA_VALUE_TRUE;
    }
  }
  return ECMA_VALUE_FALSE;
} /* ecma_builtin_number_object_is_nan */

/**
 * The Number object 'isInteger' routine
 *
 * See also:
 *          ECMA-262 v6, 20.1.2.3
 *
 * @return ecma value
 *         Returned value must be freed with ecma_free_value.
 */
static ecma_value_t
ecma_builtin_number_object_is_integer (ecma_value_t this_arg, /**< this argument */
                                       ecma_value_t arg) /**< routine's argument */
{
  JERRY_UNUSED (this_arg);
  if (!ecma_is_value_number (arg))
  {
    return ECMA_VALUE_FALSE;
  }

  ecma_number_t num = ecma_get_number_from_value (arg);

  if (ecma_number_is_nan (num) || ecma_number_is_infinity (num))
  {
    return ECMA_VALUE_FALSE;
  }

  ecma_number_t int_num;
  ecma_op_to_integer (arg, &int_num);

  if (int_num != num)
  {
    return ECMA_VALUE_FALSE;
  }

  return ECMA_VALUE_TRUE;
} /* ecma_builtin_number_object_is_integer */

/**
 * The Number object 'isSafeInteger' routine
 *
 * See also:
 *          ECMA-262 v6, 20.1.2.3
 *
 * @return ecma value
 *         Returned value must be freed with ecma_free_value.
 */
static ecma_value_t
ecma_builtin_number_object_is_safe_integer (ecma_value_t this_arg, /**< this argument */
                                            ecma_value_t arg) /**< routine's argument */
{
  JERRY_UNUSED (this_arg);

  if (!ecma_is_value_number (arg))
  {
    return ECMA_VALUE_FALSE;
  }

  ecma_number_t num = ecma_get_number_from_value (arg);

  if (ecma_number_is_nan (num) || ecma_number_is_infinity (num))
  {
    return ECMA_VALUE_FALSE;
  }

  ecma_number_t int_num = ecma_number_trunc (num);

  if (int_num == num && fabs (int_num) <= ECMA_NUMBER_MAX_SAFE_INTEGER)
  {
    return ECMA_VALUE_TRUE;
  }

  return ECMA_VALUE_FALSE;
} /* ecma_builtin_number_object_is_safe_integer */

#endif /* ENABLED (JERRY_ESNEXT) */

/**
 * @}
 * @}
 * @}
 */

#endif /* ENABLED (JERRY_BUILTIN_NUMBER) */
