#ifndef TESTENTITY_BUILDER_H
#define TESTENTITY_BUILDER_H

/* Generated by flatcc 0.5.3-pre FlatBuffers schema compiler for C by dvide.com */

#ifndef TESTENTITY_READER_H
#include "TestEntity_reader.h"
#endif
#ifndef FLATBUFFERS_COMMON_BUILDER_H
#include "flatbuffers_common_builder.h"
#endif
#include "flatcc/flatcc_prologue.h"
#ifndef flatbuffers_identifier
#define flatbuffers_identifier 0
#endif
#ifndef flatbuffers_extension
#define flatbuffers_extension ".bin"
#endif

static const flatbuffers_voffset_t __TestEntity_required[] = { 0 };
typedef flatbuffers_ref_t TestEntity_ref_t;
static TestEntity_ref_t TestEntity_clone(flatbuffers_builder_t *B, TestEntity_table_t t);
__flatbuffers_build_table(flatbuffers_, TestEntity, 11)

#define __TestEntity_formal_args ,\
  uint64_t v0, flatbuffers_bool_t v1, int8_t v2, int16_t v3,\
  int32_t v4, int64_t v5, float v6, double v7,\
  flatbuffers_string_ref_t v8, flatbuffers_int8_vec_ref_t v9, uint64_t v10
#define __TestEntity_call_args ,\
  v0, v1, v2, v3,\
  v4, v5, v6, v7,\
  v8, v9, v10
static inline TestEntity_ref_t TestEntity_create(flatbuffers_builder_t *B __TestEntity_formal_args);
__flatbuffers_build_table_prolog(flatbuffers_, TestEntity, TestEntity_identifier, TestEntity_type_identifier)

__flatbuffers_build_scalar_field(0, flatbuffers_, TestEntity_id, flatbuffers_uint64, uint64_t, 8, 8, UINT64_C(0), TestEntity)
__flatbuffers_build_scalar_field(1, flatbuffers_, TestEntity_simpleBoolean, flatbuffers_bool, flatbuffers_bool_t, 1, 1, UINT8_C(0), TestEntity)
__flatbuffers_build_scalar_field(2, flatbuffers_, TestEntity_simpleByte, flatbuffers_int8, int8_t, 1, 1, INT8_C(0), TestEntity)
__flatbuffers_build_scalar_field(3, flatbuffers_, TestEntity_simpleShort, flatbuffers_int16, int16_t, 2, 2, INT16_C(0), TestEntity)
__flatbuffers_build_scalar_field(4, flatbuffers_, TestEntity_simpleInt, flatbuffers_int32, int32_t, 4, 4, INT32_C(0), TestEntity)
__flatbuffers_build_scalar_field(5, flatbuffers_, TestEntity_simpleLong, flatbuffers_int64, int64_t, 8, 8, INT64_C(0), TestEntity)
__flatbuffers_build_scalar_field(6, flatbuffers_, TestEntity_simpleFloat, flatbuffers_float, float, 4, 4, 0.000000f, TestEntity)
__flatbuffers_build_scalar_field(7, flatbuffers_, TestEntity_simpleDouble, flatbuffers_double, double, 8, 8, 0.000000, TestEntity)
__flatbuffers_build_string_field(8, flatbuffers_, TestEntity_simpleString, TestEntity)
__flatbuffers_build_vector_field(9, flatbuffers_, TestEntity_simpleByteArray, flatbuffers_int8, int8_t, TestEntity)
__flatbuffers_build_scalar_field(10, flatbuffers_, TestEntity_simpleDate, flatbuffers_uint64, uint64_t, 8, 8, UINT64_C(0), TestEntity)

static inline TestEntity_ref_t TestEntity_create(flatbuffers_builder_t *B __TestEntity_formal_args)
{
    if (TestEntity_start(B)
        || TestEntity_id_add(B, v0)
        || TestEntity_simpleLong_add(B, v5)
        || TestEntity_simpleDouble_add(B, v7)
        || TestEntity_simpleDate_add(B, v10)
        || TestEntity_simpleInt_add(B, v4)
        || TestEntity_simpleFloat_add(B, v6)
        || TestEntity_simpleString_add(B, v8)
        || TestEntity_simpleByteArray_add(B, v9)
        || TestEntity_simpleShort_add(B, v3)
        || TestEntity_simpleBoolean_add(B, v1)
        || TestEntity_simpleByte_add(B, v2)) {
        return 0;
    }
    return TestEntity_end(B);
}

static TestEntity_ref_t TestEntity_clone(flatbuffers_builder_t *B, TestEntity_table_t t)
{
    __flatbuffers_memoize_begin(B, t);
    if (TestEntity_start(B)
        || TestEntity_id_pick(B, t)
        || TestEntity_simpleLong_pick(B, t)
        || TestEntity_simpleDouble_pick(B, t)
        || TestEntity_simpleDate_pick(B, t)
        || TestEntity_simpleInt_pick(B, t)
        || TestEntity_simpleFloat_pick(B, t)
        || TestEntity_simpleString_pick(B, t)
        || TestEntity_simpleByteArray_pick(B, t)
        || TestEntity_simpleShort_pick(B, t)
        || TestEntity_simpleBoolean_pick(B, t)
        || TestEntity_simpleByte_pick(B, t)) {
        return 0;
    }
    __flatbuffers_memoize_end(B, t, TestEntity_end(B));
}

#include "flatcc/flatcc_epilogue.h"
#endif /* TESTENTITY_BUILDER_H */
