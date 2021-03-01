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

/*
 * eval description
 */

#include "ecma-builtin-helpers-macro-defines.inc.h"

/* Number properties:
 *  (property name, object pointer getter) */

NUMBER_VALUE (LIT_MAGIC_STRING_LENGTH,
              1,
              ECMA_PROPERTY_FLAG_DEFAULT_LENGTH)

STRING_VALUE (LIT_MAGIC_STRING_NAME,
              LIT_MAGIC_STRING_EVAL,
              ECMA_PROPERTY_FLAG_CONFIGURABLE)

#include "ecma-builtin-helpers-macro-undefs.inc.h"
