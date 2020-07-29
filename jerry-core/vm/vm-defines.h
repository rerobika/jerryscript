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
#ifndef VM_DEFINES_H
#define VM_DEFINES_H

#include "byte-code.h"
#include "ecma-globals.h"

/** \addtogroup vm Virtual machine
 * @{
 *
 * \addtogroup vm_executor Executor
 * @{
 */

/**
 * Helper for += on uint16_t values.
 */
#define VM_PLUS_EQUAL_U16(base, value) (base) = (uint16_t) ((base) + (value))

/**
 * Helper for -= on uint16_t values.
 */
#define VM_MINUS_EQUAL_U16(base, value) (base) = (uint16_t) ((base) - (value))

/**
 * Context of interpreter, related to a JS stack frame
 */
typedef struct vm_frame_ctx_t
{
  ecma_call_stack_t call_frame;                       /**< [[TODO]] */
  const ecma_compiled_code_t *bytecode_header_p;      /**< currently executed byte-code data */
  const uint8_t *byte_code_p;                         /**< current byte code pointer */
  const uint8_t *byte_code_start_p;                   /**< byte code start pointer */
  ecma_value_t *stack_top_p;                          /**< stack top pointer */
  ecma_value_t *literal_start_p;                      /**< literal list start pointer */
  ecma_object_t *lex_env_p;                           /**< current lexical environment */
  ecma_value_t block_result;                          /**< block result */
#if ENABLED (JERRY_LINE_INFO)
  uint32_t current_line;                              /**< currently executed line */
#endif /* ENABLED (JERRY_LINE_INFO) */
  uint16_t context_depth;                             /**< current context depth */
  uint8_t is_eval_code;                               /**< eval mode flag */
  uint8_t call_operation;                             /**< perform a call or construct operation */
  /* Registers start immediately after the frame context. */
} vm_frame_ctx_t;

/**
 * Get register list corresponding to the frame context.
 */
#define VM_GET_REGISTERS(frame_ctx_p) ((ecma_value_t *) ((frame_ctx_p) + 1))

/**
 * Read or write a specific register.
 */
#define VM_GET_REGISTER(frame_ctx_p, i) (((ecma_value_t *) ((frame_ctx_p) + 1))[i])

/**
 * Get the executable object.
 */
#define VM_GET_EXECUTABLE_OBJECT(frame_ctx_p) \
  ((ecma_extended_object_t *) ((uintptr_t) (frame_ctx_p) - (uintptr_t) offsetof (vm_executable_object_t, frame_ctx)))

/**
 * Generator frame context.
 */
typedef struct
{
  ecma_extended_object_t extended_object; /**< extended object part */
  vm_frame_ctx_t frame_ctx; /**< frame context part */
} vm_executable_object_t;

/**
 * @}
 * @}
 */

#endif /* !VM_DEFINES_H */
