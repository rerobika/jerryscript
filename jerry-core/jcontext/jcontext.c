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

#include "jcontext.h"

#include "ecma-array-object.h"
#include "ecma-function-object.h"
#include "ecma-helpers.h"
#include "ecma-line-info.h"

#include "jcontext.h"
#include "lit-char-helpers.h"

/** \addtogroup context Context
 * @{
 */

/**
 * Check the existence of the ECMA_STATUS_EXCEPTION flag.
 *
 * @return true - if the flag is set
 *         false - otherwise
 */
extern inline bool JERRY_ATTR_ALWAYS_INLINE
jcontext_has_pending_exception (void)
{
  return JERRY_CONTEXT (status_flags) & ECMA_STATUS_EXCEPTION;
} /* jcontext_has_pending_exception */

/**
 * Check the existence of the ECMA_STATUS_ABORT flag.
 *
 * @return true - if the flag is set
 *         false - otherwise
 */
extern inline bool JERRY_ATTR_ALWAYS_INLINE
jcontext_has_pending_abort (void)
{
  return JERRY_CONTEXT (status_flags) & ECMA_STATUS_ABORT;
} /* jcontext_has_pending_abort */

/**
 * Set the abort flag for the context.
 */
extern inline void JERRY_ATTR_ALWAYS_INLINE
jcontext_set_abort_flag (bool is_abort) /**< true - if the abort flag should be set
                                         *   false - if the abort flag should be removed */
{
  JERRY_ASSERT (jcontext_has_pending_exception ());

  if (is_abort)
  {
    JERRY_CONTEXT (status_flags) |= ECMA_STATUS_ABORT;
  }
  else
  {
    JERRY_CONTEXT (status_flags) &= (uint32_t) ~ECMA_STATUS_ABORT;
  }
} /* jcontext_set_abort_flag */

/**
 * Set the exception flag for the context.
 */
extern inline void JERRY_ATTR_ALWAYS_INLINE
jcontext_set_exception_flag (bool is_exception) /**< true - if the exception flag should be set
                                                 *   false - if the exception flag should be removed */
{
  if (is_exception)
  {
    JERRY_CONTEXT (status_flags) |= ECMA_STATUS_EXCEPTION;
  }
  else
  {
    JERRY_CONTEXT (status_flags) &= (uint32_t) ~ECMA_STATUS_EXCEPTION;
  }
} /* jcontext_set_exception_flag */

/**
 * Raise exception from the given error value.
 */
extern inline void JERRY_ATTR_ALWAYS_INLINE
jcontext_raise_exception (ecma_value_t error) /**< error to raise */
{
  JERRY_ASSERT (!jcontext_has_pending_exception ());
  JERRY_ASSERT (!jcontext_has_pending_abort ());

  JERRY_CONTEXT (error_value) = error;
  jcontext_set_exception_flag (true);
} /* jcontext_raise_exception */

/**
 * Release the current exception/abort of the context.
 */
void
jcontext_release_exception (void)
{
  JERRY_ASSERT (jcontext_has_pending_exception ());

  ecma_free_value (jcontext_take_exception ());
} /* jcontext_release_exception */

/**
 * Take the current exception/abort of context.
 *
 * @return current exception as an ecma-value
 */
ecma_value_t
jcontext_take_exception (void)
{
  JERRY_ASSERT (jcontext_has_pending_exception ());

  JERRY_CONTEXT (status_flags) &= (uint32_t) ~(ECMA_STATUS_EXCEPTION
#if JERRY_VM_THROW
                                               | ECMA_STATUS_ERROR_THROWN
#endif /* JERRY_VM_THROW */
                                               | ECMA_STATUS_ABORT);
  return JERRY_CONTEXT (error_value);
} /* jcontext_take_exception */

/**
 * TODO
 *
 * @return current exception as an ecma-value
 */
ecma_global_object_t *
jcontext_get_global_object (void)
{
#if JERRY_BUILTIN_REALMS
  if (JERRY_LIKELY (!(JERRY_CONTEXT (status_flags) & ECMA_STATUS_API_REALM_CREATED)))
  {
    return JERRY_CONTEXT (global_object_p);
  }

  ecma_call_frame_t *call_frame_p = JERRY_CONTEXT (call_stack_p);

  while (call_frame_p)
  {
    ecma_object_t *callee_p = call_frame_p->callee_p;

    switch (ecma_get_object_type (callee_p))
    {
      case ECMA_OBJECT_TYPE_FUNCTION:
      case ECMA_OBJECT_TYPE_BUILT_IN_FUNCTION:
      case ECMA_OBJECT_TYPE_NATIVE_FUNCTION:
      {
        return ecma_op_function_get_function_realm (callee_p);
      }
      case ECMA_OBJECT_TYPE_CLASS:
      {
        ecma_extended_object_t *ext_obj_p = (ecma_extended_object_t *) callee_p;

#if JERRY_ESNEXT
        JERRY_ASSERT (ext_obj_p->u.cls.type == ECMA_OBJECT_CLASS_SCRIPT);
        if (ext_obj_p->u.cls.type == ECMA_OBJECT_CLASS_MODULE)
        {
          call_frame_p = call_frame_p->prev_p;
          continue;
        }
#endif /* JERRY_ESNEXT */

        JERRY_ASSERT (ext_obj_p->u.cls.type == ECMA_OBJECT_CLASS_SCRIPT);
        ecma_compiled_code_t *byte_code_p =
          ECMA_GET_INTERNAL_VALUE_POINTER (ecma_compiled_code_t, ext_obj_p->u.cls.u3.value);

        return ecma_op_function_get_realm (byte_code_p);

        break;
      }
      case ECMA_OBJECT_TYPE_BUILT_IN_GENERAL:
      {
        ecma_global_object_t *global_object_p = (ecma_global_object_t *) callee_p;
        JERRY_ASSERT (global_object_p->extended_object.u.built_in.id == ECMA_BUILTIN_ID_GLOBAL);

        return global_object_p;
      }
#if JERRY_ESNEXT
      case ECMA_OBJECT_TYPE_PROXY:
      {
        call_frame_p = call_frame_p->prev_p;
        continue;
      }
#endif /* JERRY_ESNEXT */
      default:
      {
        JERRY_UNREACHABLE ();
        break;
      }
    }
  }
#endif /* JERRY_BUILTIN_REALMS */

  return JERRY_CONTEXT (global_object_p);
} /* jcontext_get_global_object */

#if JERRY_ESNEXT

/**
 * TODO
 *
 * @return current exception as an ecma-value
 */
ecma_object_t *
jcontext_get_new_target (void)
{
  JERRY_ASSERT (JERRY_CONTEXT (call_stack_p) != 0);
  ecma_call_frame_t *call_frame_p = JERRY_CONTEXT (call_stack_p);

  while (call_frame_p)
  {
    ecma_object_t *callee_p = call_frame_p->callee_p;

    switch (ecma_get_object_type (callee_p))
    {
      case ECMA_OBJECT_TYPE_FUNCTION:
      {
        vm_frame_ctx_t *frame_ctx_p = (vm_frame_ctx_t *) call_frame_p;

        if (frame_ctx_p->status_flags & VM_FRAME_CTX_EXECUTE_DIRECT_EVAL)
        {
          call_frame_p = call_frame_p->prev_p;
          JERRY_ASSERT (ECMA_CALL_FRAME_HAS_FRAME_CTX (call_frame_p));
          continue;
        }

        if (!(frame_ctx_p->shared_p->status_flags & VM_FRAME_CTX_SHARED_NON_ARROW_FUNC))
        {
          ecma_arrow_function_t *arrow_func_p = (ecma_arrow_function_t *) callee_p;

          return (ecma_is_value_undefined (arrow_func_p->new_target)
                    ? NULL
                    : ecma_get_object_from_value (arrow_func_p->new_target));
        }

        if (frame_ctx_p->shared_p->status_flags & VM_FRAME_CTX_SHARED_CONSTUCTOR_CALL)
        {
          return ((vm_frame_ctx_new_target_shared_args_t *) (frame_ctx_p->shared_p))->new_target_p;
        }

        return NULL;
      }
      case ECMA_OBJECT_TYPE_BUILT_IN_FUNCTION:
      {
        return ((ecma_builtin_call_frame_t *) call_frame_p)->new_target_p;
      }
      case ECMA_OBJECT_TYPE_NATIVE_FUNCTION:
      {
        ecma_native_call_frame_t *native_frame_p = ((ecma_native_call_frame_t *) call_frame_p);

        if (ecma_is_value_undefined (native_frame_p->info.new_target))
        {
          return NULL;
        }

        return ecma_get_object_from_value (native_frame_p->info.new_target);
      }
      case ECMA_OBJECT_TYPE_CLASS:
      {
        ecma_extended_object_t *ext_obj_p = (ecma_extended_object_t *) callee_p;

        JERRY_ASSERT (ext_obj_p->u.cls.type == ECMA_OBJECT_CLASS_SCRIPT
                      || ext_obj_p->u.cls.type == ECMA_OBJECT_CLASS_MODULE);
        return NULL;
      }
      case ECMA_OBJECT_TYPE_BUILT_IN_GENERAL:
      {
        ecma_global_object_t *global_object_p = (ecma_global_object_t *) callee_p;
        JERRY_ASSERT (global_object_p->extended_object.u.built_in.id == ECMA_BUILTIN_ID_GLOBAL);

        call_frame_p = call_frame_p->prev_p;
        continue;
      }
      case ECMA_OBJECT_TYPE_PROXY:
      {
        call_frame_p = call_frame_p->prev_p;
        continue;
      }
      default:
      {
        break;
      }
    }
  }

  return NULL;
} /* jcontext_get_new_target */

#endif /* JERRY_ESNEXT */

#if JERRY_LINE_INFO
/**
 * TODO
 *
 * @return current exception as an ecma-value
 */
static void
jcontext_generator_backtrace_simple (ecma_stringbuilder_t *builder_p, /**< frame builder */
                                     vm_frame_ctx_t *context_p) /**< interpreter context */
{
  const ecma_compiled_code_t *bytecode_header_p = context_p->shared_p->bytecode_header_p;
  ecma_value_t resource_name = ecma_get_resource_name (bytecode_header_p);
  ecma_string_t *str_p = ecma_get_string_from_value (resource_name);

  if (ecma_string_is_empty (str_p))
  {
    ecma_stringbuilder_append_raw (builder_p, (const lit_utf8_byte_t *) "<unknown>:", 10);
  }
  else
  {
    ecma_stringbuilder_append (builder_p, str_p);
    ecma_stringbuilder_append_byte (builder_p, LIT_CHAR_COLON);
  }

  if (bytecode_header_p->status_flags & CBC_CODE_FLAGS_HAS_LINE_INFO)
  {
    jerry_backtrace_location_t location;
    ecma_line_info_get (ecma_compiled_code_get_line_info (bytecode_header_p),
                        (uint32_t) (context_p->byte_code_p - context_p->byte_code_start_p),
                        &location);

    ecma_string_t *line_str_p = ecma_new_ecma_string_from_uint32 (location.line);
    ecma_stringbuilder_append (builder_p, line_str_p);
    ecma_deref_ecma_string (line_str_p);

    ecma_stringbuilder_append_byte (builder_p, LIT_CHAR_COLON);

    line_str_p = ecma_new_ecma_string_from_uint32 (location.column);
    ecma_stringbuilder_append (builder_p, line_str_p);
    ecma_deref_ecma_string (line_str_p);
  }
  else
  {
    ecma_stringbuilder_append_raw (builder_p, (const lit_utf8_byte_t *) "1:1", 3);
  }
} /* jcontext_generator_backtrace_simple */

/**
 * TODO
 *
 * @return current exception as an ecma-value
 */
static void
jcontext_generator_backtrace_builtin (ecma_stringbuilder_t *builder_p, /**< frame builder */
                                      ecma_construct_frame_t *frame_p) /**< builtin function frame */
{
  JERRY_UNUSED (frame_p);

  const lit_utf8_byte_t frame_str_p[] = "<builtin function>";
  ecma_stringbuilder_append_raw (builder_p, frame_str_p, sizeof (frame_str_p) - 1);
} /* jcontext_generator_backtrace_builtin */

/**
 * TODO
 *
 * @return current exception as an ecma-value
 */
static void
jcontext_generator_backtrace_native (ecma_stringbuilder_t *builder_p, /**< frame builder */
                                     ecma_native_call_frame_t *frame_p) /**< native function frame */
{
  JERRY_UNUSED (frame_p);

  const lit_utf8_byte_t frame_str_p[] = "<native function>";
  ecma_stringbuilder_append_raw (builder_p, frame_str_p, sizeof (frame_str_p) - 1);
} /* jcontext_generator_backtrace_native */

#endif /* JERRY_LINE_INFO */

/**
 * Get backtrace. The backtrace is an array of strings where
 * each string contains the position of the corresponding frame.
 * The array length is zero if the backtrace is not available.
 *
 * @return array ecma value
 */
ecma_value_t
jcontext_get_backtrace (uint32_t max_depth) /**< maximum backtrace depth, 0 = unlimited */
{
#if JERRY_LINE_INFO
  if (max_depth == 0)
  {
    max_depth = UINT32_MAX;
  }

  ecma_object_t *array_p = ecma_op_new_array_object (0);
  JERRY_ASSERT (ecma_op_object_is_fast_array (array_p));
  uint32_t index = 0;

  ecma_call_frame_t *call_frame_p = JERRY_CONTEXT (call_stack_p);

  while (call_frame_p)
  {
    ecma_stringbuilder_t builder = ecma_stringbuilder_create ();
    ecma_object_t *callee_p = call_frame_p->callee_p;

    switch (ecma_get_object_type (callee_p))
    {
      case ECMA_OBJECT_TYPE_CLASS:
      {
        ecma_extended_object_t *ext_obj_p = (ecma_extended_object_t *) callee_p;
#if JERRY_ESNEXT
        JERRY_ASSERT (ext_obj_p->u.cls.type == ECMA_OBJECT_CLASS_SCRIPT);
#else /* !JERRY_ESNEXT */
        JERRY_ASSERT (ext_obj_p->u.cls.type == ECMA_OBJECT_CLASS_SCRIPT
                      || ext_obj_p->u.cls.type == ECMA_OBJECT_CLASS_MODULE);
#endif /* JERRY_ESNEXT */
        /* FALLTHRU */
      }
      case ECMA_OBJECT_TYPE_FUNCTION:
      {
        jcontext_generator_backtrace_simple (&builder, (vm_frame_ctx_t *) call_frame_p);
        break;
      }
      case ECMA_OBJECT_TYPE_BUILT_IN_FUNCTION:
      {
        jcontext_generator_backtrace_builtin (&builder, (ecma_construct_frame_t *) call_frame_p);
        break;
      }
      case ECMA_OBJECT_TYPE_NATIVE_FUNCTION:
      {
        jcontext_generator_backtrace_native (&builder, (ecma_native_call_frame_t *) call_frame_p);
        break;
      }
      case ECMA_OBJECT_TYPE_BUILT_IN_GENERAL:
      {
        ecma_global_object_t *global_object_p = (ecma_global_object_t *) callee_p;
        JERRY_ASSERT (global_object_p->extended_object.u.built_in.id == ECMA_BUILTIN_ID_GLOBAL);

        /* FALLTHRU */
      }
#if JERRY_ESNEXT
      case ECMA_OBJECT_TYPE_PROXY:
#endif /* JERRY_ESNEXT */
      case ECMA_OBJECT_TYPE_BOUND_FUNCTION:
      {
        call_frame_p = call_frame_p->prev_p;
        continue;
      }
      default:
      {
        return ecma_make_object_value (array_p);
      }
    }

    ecma_string_t *frame_str_p = ecma_stringbuilder_finalize (&builder);
    ecma_fast_array_set_property (array_p, index, ecma_make_string_value (frame_str_p));
    ecma_deref_ecma_string (frame_str_p);

    call_frame_p = call_frame_p->prev_p;
    index++;

    if (index >= max_depth)
    {
      break;
    }
  }

  return ecma_make_object_value (array_p);
#else /* !JERRY_LINE_INFO */
  JERRY_UNUSED (max_depth);

  return ecma_make_object_value (ecma_op_new_array_object (0));
#endif /* JERRY_LINE_INFO */
} /* jcontext_get_backtrace */

#if !JERRY_EXTERNAL_CONTEXT

/**
 * Global context.
 */
jerry_context_t jerry_global_context;

#if !JERRY_SYSTEM_ALLOCATOR

/**
 * Check size of heap is corresponding to configuration
 */
JERRY_STATIC_ASSERT (sizeof (jmem_heap_t) <= JMEM_HEAP_SIZE,
                     size_of_mem_heap_must_be_less_than_or_equal_to_JMEM_HEAP_SIZE);

/**
 * Global heap.
 */
jmem_heap_t jerry_global_heap JERRY_ATTR_ALIGNED (JMEM_ALIGNMENT) JERRY_ATTR_GLOBAL_HEAP;

#endif /* !JERRY_SYSTEM_ALLOCATOR */

#endif /* !JERRY_EXTERNAL_CONTEXT */

/**
 * @}
 */
