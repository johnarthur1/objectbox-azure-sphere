#ifndef COMPRESSIONDEMOENTITY_BUILDER_H
#define COMPRESSIONDEMOENTITY_BUILDER_H

/* Generated by flatcc 0.5.3-pre FlatBuffers schema compiler for C by dvide.com */

#ifndef COMPRESSIONDEMOENTITY_READER_H
#include "CompressionDemoEntity_reader.h"
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

static const flatbuffers_voffset_t __CompressionDemoEntity_required[] = { 0 };
typedef flatbuffers_ref_t CompressionDemoEntity_ref_t;
static CompressionDemoEntity_ref_t CompressionDemoEntity_clone(flatbuffers_builder_t *B, CompressionDemoEntity_table_t t);
__flatbuffers_build_table(flatbuffers_, CompressionDemoEntity, 3)

#define __CompressionDemoEntity_formal_args , uint64_t v0, uint64_t v1, flatbuffers_int8_vec_ref_t v2
#define __CompressionDemoEntity_call_args , v0, v1, v2
static inline CompressionDemoEntity_ref_t CompressionDemoEntity_create(flatbuffers_builder_t *B __CompressionDemoEntity_formal_args);
__flatbuffers_build_table_prolog(flatbuffers_, CompressionDemoEntity, CompressionDemoEntity_identifier, CompressionDemoEntity_type_identifier)

__flatbuffers_build_scalar_field(0, flatbuffers_, CompressionDemoEntity_id, flatbuffers_uint64, uint64_t, 8, 8, UINT64_C(0), CompressionDemoEntity)
__flatbuffers_build_scalar_field(1, flatbuffers_, CompressionDemoEntity_timestamp, flatbuffers_uint64, uint64_t, 8, 8, UINT64_C(0), CompressionDemoEntity)
__flatbuffers_build_vector_field(2, flatbuffers_, CompressionDemoEntity_userData, flatbuffers_int8, int8_t, CompressionDemoEntity)

static inline CompressionDemoEntity_ref_t CompressionDemoEntity_create(flatbuffers_builder_t *B __CompressionDemoEntity_formal_args)
{
    if (CompressionDemoEntity_start(B)
        || CompressionDemoEntity_id_add(B, v0)
        || CompressionDemoEntity_timestamp_add(B, v1)
        || CompressionDemoEntity_userData_add(B, v2)) {
        return 0;
    }
    return CompressionDemoEntity_end(B);
}

static CompressionDemoEntity_ref_t CompressionDemoEntity_clone(flatbuffers_builder_t *B, CompressionDemoEntity_table_t t)
{
    __flatbuffers_memoize_begin(B, t);
    if (CompressionDemoEntity_start(B)
        || CompressionDemoEntity_id_pick(B, t)
        || CompressionDemoEntity_timestamp_pick(B, t)
        || CompressionDemoEntity_userData_pick(B, t)) {
        return 0;
    }
    __flatbuffers_memoize_end(B, t, CompressionDemoEntity_end(B));
}

#include "flatcc/flatcc_epilogue.h"
#endif /* COMPRESSIONDEMOENTITY_BUILDER_H */
