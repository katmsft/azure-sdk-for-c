// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#ifndef _az_SPAN_H
#define _az_SPAN_H

#include <az_action.h>
#include <az_result.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <_az_cfg_prefix.h>

/**
 * An immutable span of bytes (octets).
 */
typedef struct {
  uint8_t const * begin;
  size_t size;
} az_span;

AZ_NODISCARD AZ_INLINE az_span az_span_empty() { return (az_span){ 0 }; }

AZ_NODISCARD AZ_INLINE bool az_span_is_empty(az_span const span) { return span.size == 0; }

/**
 * @brief returns a span with the left @var n bytes of the given @var span.
 *
 * If the @var n is greater than the @span.size than the whole @var span is returned.
 */
AZ_NODISCARD AZ_INLINE az_span az_span_take(az_span const span, size_t const n) {
  if (span.size <= n) {
    return span;
  }
  return (az_span){ .begin = span.begin, .size = n };
}

/**
 * @brief returns a span with @var n positions are dropped.
 *
 * If the @var n is greater than @span.size than an empty span is returned
 */
AZ_NODISCARD AZ_INLINE az_span az_span_drop(az_span const span, size_t const n) {
  if (span.size <= n) {
    return az_span_empty();
  }
  return (az_span){ .begin = span.begin + n, .size = span.size - n };
}

/**
 * Returns a sub span of the given span.
 */
AZ_NODISCARD AZ_INLINE az_span
az_span_sub(az_span const span, size_t const begin, size_t const end) {
  az_span const t = az_span_take(span, end);
  return az_span_drop(t, begin);
}

/**
 * Returns `true` if a content of the @a span is equal to a content of the @b
 * span.
 */
AZ_NODISCARD AZ_INLINE bool az_span_is_equal(az_span const a, az_span const b) {
  return a.size == b.size && memcmp(a.begin, b.begin, a.size) == 0;
}

/**
 * Returns `true` if a content of the @a span is equal to a content of the @b
 * span using case-insensetive compare.
 */
AZ_NODISCARD bool az_span_is_equal_ignoring_case(az_span const a, az_span const b);

AZ_NODISCARD AZ_INLINE bool az_span_is_overlap(az_span const a, az_span const b) {
  return (!az_span_is_empty(a) && !az_span_is_empty(b))
      && ((a.begin < b.begin && (a.begin + a.size - 1) >= b.begin)
          || (b.begin < a.begin && (b.begin + b.size - 1) >= a.begin) || (a.begin == b.begin));
}

AZ_NODISCARD az_result az_span_to_uint64(az_span const self, uint64_t * const out);

#define AZ_SPAN_FROM_ARRAY(ARRAY) \
  { .begin = ARRAY, .size = (sizeof(ARRAY) / sizeof(*ARRAY)) }

/**
 * @brief Use this function when size for one span is given in runtime
 * Don't use this function for arrays. Use @var AZ_SPAN_FROM_ARRAY instead.
 *
 */
AZ_NODISCARD AZ_INLINE az_span az_span_from_runtime_array(uint8_t const * ptr, size_t size) {
  return (az_span){ .begin = ptr, .size = size };
}

/**
 * @brief Use this only to create a span from uint8_t object.
 * The size of the returned span is always one.
 * Don't use this function for arrays. Use @var AZ_SPAN_FROM_ARRAY instead.
 * Don't us
 */
AZ_NODISCARD AZ_INLINE az_span az_span_from_single_item(uint8_t const * ptr) {
  return az_span_from_runtime_array(ptr, 1);
}

AZ_NODISCARD AZ_INLINE az_span az_str_to_span(char const * str) {
  return (az_span){ .begin = (uint8_t const *)str, .size = strlen(str) };
}

#include <_az_cfg_suffix.h>

#endif
