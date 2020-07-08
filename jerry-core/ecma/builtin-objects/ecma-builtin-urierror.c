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
#include "ecma-builtin-helpers.h"
#include "ecma-objects.h"
#include "ecma-try-catch-macro.h"
#include "jrt.h"

#if ENABLED (JERRY_BUILTIN_ERRORS)

#define ECMA_BUILTINS_INTERNAL
#include "ecma-builtins-internal.h"

#define BUILTIN_INC_HEADER_NAME "ecma-builtin-urierror.inc.h"
#define BUILTIN_UNDERSCORED_ID uri_error
#include "ecma-builtin-internal-routines-template.inc.h"

/** \addtogroup ecma ECMA
 * @{
 *
 * \addtogroup ecmabuiltins
 * @{
 *
 * \addtogroup urierror ECMA UriError object built-in
 * @{
 */

/**
 * Handle [[Call]]/[[Construct]] of built-in UriError object
 *
 * @return ecma value
 */
ecma_value_t
ecma_builtin_uri_error_dispatch (ecma_func_args_t *func_args_p) /**< function arguments */
{
  return ecma_builtin_helper_error_dispatch (func_args_p, ECMA_ERROR_URI);
} /* ecma_builtin_uri_error_dispatch */

/**
 * @}
 * @}
 * @}
 */

#endif /* ENABLED (JERRY_BUILTIN_ERRORS) */
