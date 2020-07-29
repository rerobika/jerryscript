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

#include "ecma-array-object.h"
#include "ecma-builtins.h"
#include "ecma-helpers.h"
#include "jcontext.h"
#include "lit-char-helpers.h"
#include "vm.h"

/**
 * Get backtrace. The backtrace is an array of strings where
 * each string contains the position of the corresponding frame.
 * The array length is zero if the backtrace is not available.
 *
 * @return array ecma value
 */
ecma_value_t
vm_get_backtrace (uint32_t max_depth) /**< maximum backtrace depth, 0 = unlimited */
{
#if ENABLED (JERRY_LINE_INFO)
  ecma_value_t result_array = ecma_op_create_array_object (NULL, 0, false);

  if (max_depth == 0)
  {
    max_depth = UINT32_MAX;
  }

  ecma_call_stack_t *call_stack_p = JERRY_CONTEXT (call_stack_p);
  ecma_object_t *array_p = ecma_get_object_from_value (result_array);
  JERRY_ASSERT (ecma_op_object_is_fast_array (array_p));
  uint32_t index = 0;

  while (call_stack_p != NULL)
  {
    ecma_stringbuilder_t builder = ecma_stringbuilder_create ();
    ecma_object_t *func_obj_p = call_stack_p->func_args_p->func_obj_p;

    if (ecma_get_object_type (func_obj_p) == ECMA_OBJECT_TYPE_EXTERNAL_FUNCTION)
    {
      ecma_stringbuilder_append_raw (&builder, (const lit_utf8_byte_t *) "<external callback>", 19);
    }
    else if (func_obj_p == ecma_builtin_get_global ())
    {
      ecma_stringbuilder_append_raw (&builder, (const lit_utf8_byte_t *) "<global>", 8);
    }
    else
    {
      JERRY_ASSERT (ecma_get_object_type (func_obj_p) == ECMA_OBJECT_TYPE_FUNCTION);

      if (ecma_get_object_is_builtin (func_obj_p))
      {
        ecma_stringbuilder_append_raw (&builder, (const lit_utf8_byte_t *) "<native function>", 17);
      }
      else
      {
        vm_frame_ctx_t *call_frame_p = (vm_frame_ctx_t *) call_stack_p;

        ecma_value_t resource_name = ecma_get_resource_name (call_frame_p->bytecode_header_p);
        ecma_string_t *str_p = ecma_get_string_from_value (resource_name);

        if (ecma_string_is_empty (str_p))
        {
          ecma_stringbuilder_append_raw (&builder, (const lit_utf8_byte_t *) "<unknown>:", 10);
        }
        else
        {
          ecma_stringbuilder_append (&builder, str_p);
          ecma_stringbuilder_append_byte (&builder, LIT_CHAR_COLON);
        }

        ecma_string_t *line_str_p = ecma_new_ecma_string_from_uint32 (call_frame_p->current_line);
        ecma_stringbuilder_append (&builder, line_str_p);
        ecma_deref_ecma_string (line_str_p);
      }
    }

    ecma_string_t *builder_str_p = ecma_stringbuilder_finalize (&builder);
    ecma_fast_array_set_property (array_p, index, ecma_make_string_value (builder_str_p));
    ecma_deref_ecma_string (builder_str_p);

    call_stack_p = call_stack_p->prev_p;
    index++;

    if (index >= max_depth)
    {
      break;
    }
  }

  return result_array;
#else /* !ENABLED (JERRY_LINE_INFO) */
  JERRY_UNUSED (max_depth);

  return ecma_op_create_array_object (NULL, 0, false);
#endif /* ENABLED (JERRY_LINE_INFO) */
} /* vm_get_backtrace */
