/*
 * Copyright 2018 ObjectBox Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Single header file for the ObjectBox C API
//
// Naming conventions
// ------------------
// * methods: obx_thing_action()
// * structs: OBX_thing {}
// * error codes: OBX_ERROR_REASON
//

#ifndef OBJECTBOX_H
#define OBJECTBOX_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------
// ObjectBox version codes
//----------------------------------------------

// Note that you should use methods with prefix obx_version_ to check when linking against the dynamic library
#define OBX_VERSION_MAJOR 0
#define OBX_VERSION_MINOR 5
#define OBX_VERSION_PATCH 0  // values >= 100 are reserved for dev releases leading to the next minor/major increase

/// Returns the version of the library as ints. Pointers may be null
void obx_version(int* major, int* minor, int* patch);

/// Checks if the version of the library is equal to or higher than the given version ints.
bool obx_version_is_at_least(int major, int minor, int patch);

/// Returns the version of the library to be printed.
/// The format may change; to query for version use the int based methods instead.
const char* obx_version_string(void);

/// Returns the version of the ObjectBox core to be printed.
/// The format may change, do not rely on its current form.
const char* obx_version_core_string(void);

//----------------------------------------------
// Utilities
//----------------------------------------------

/// delete the store files from the given directory
int obx_remove_db_files(char const* directory);

/// checks whether functions returning OBX_bytes_array are fully supported (depends on build, invariant during runtime)
bool obx_supports_bytes_array();

//----------------------------------------------
// Return codes
//----------------------------------------------

/// Value returned when no error occurred (0)
#define OBX_SUCCESS 0

/// Returned by e.g. get operations if nothing was found for a specific ID.
/// This is NOT an error condition, and thus no last error info is set.
#define OBX_NOT_FOUND 404

// General errors
#define OBX_ERROR_ILLEGAL_STATE 10001
#define OBX_ERROR_ILLEGAL_ARGUMENT 10002
#define OBX_ERROR_ALLOCATION 10003
#define OBX_ERROR_NO_ERROR_INFO 10097
#define OBX_ERROR_GENERAL 10098
#define OBX_ERROR_UNKNOWN 10099

// Storage errors (often have a secondary error code)
#define OBX_ERROR_DB_FULL 10101
#define OBX_ERROR_MAX_READERS_EXCEEDED 10102
#define OBX_ERROR_STORE_MUST_SHUTDOWN 10103
#define OBX_ERROR_STORAGE_GENERAL 10199

// Data errors
#define OBX_ERROR_UNIQUE_VIOLATED 10201
#define OBX_ERROR_NON_UNIQUE_RESULT 10202
#define OBX_ERROR_PROPERTY_TYPE_MISMATCH 10203
#define OBX_ERROR_CONSTRAINT_VIOLATED 10299

// STD errors
#define OBX_ERROR_STD_ILLEGAL_ARGUMENT 10301
#define OBX_ERROR_STD_OUT_OF_RANGE 10302
#define OBX_ERROR_STD_LENGTH 10303
#define OBX_ERROR_STD_BAD_ALLOC 10304
#define OBX_ERROR_STD_RANGE 10305
#define OBX_ERROR_STD_OVERFLOW 10306
#define OBX_ERROR_STD_OTHER 10399

// Inconsistencies detected
#define OBX_ERROR_SCHEMA 10501
#define OBX_ERROR_FILE_CORRUPT 10502

//----------------------------------------------
// Common types
//----------------------------------------------
/// Schema entity & property identifiers
typedef uint32_t obx_schema_id;

/// Universal identifier used in schema for entities & properties
typedef uint64_t obx_uid;

/// ID of a single Object stored in the database
typedef uint64_t obx_id;

/// Error code returned by an obx_* function
typedef int obx_err;

/// The callback for reading data one-by-one
/// @param arg is a pass-through argument passed to the called API
/// @param data is the read data buffer
/// @param size specifies the length of the read data
typedef bool obx_data_visitor(void* arg, const void* data, size_t size);

//----------------------------------------------
// Error info
//----------------------------------------------

obx_err obx_last_error_code(void);

const char* obx_last_error_message(void);

obx_err obx_last_error_secondary(void);

void obx_last_error_clear(void);

//----------------------------------------------
// Model
//----------------------------------------------
typedef enum {
    OBXPropertyType_Bool = 1,
    OBXPropertyType_Byte = 2,
    OBXPropertyType_Short = 3,
    OBXPropertyType_Char = 4,
    OBXPropertyType_Int = 5,
    OBXPropertyType_Long = 6,
    OBXPropertyType_Float = 7,
    OBXPropertyType_Double = 8,
    OBXPropertyType_String = 9,
    OBXPropertyType_Date = 10,
    OBXPropertyType_Relation = 11,
    OBXPropertyType_ByteVector = 23,
    OBXPropertyType_StringVector = 30,
} OBXPropertyType;

/// Not really an enum, but binary flags to use across languages
typedef enum {
    /// One long property on an entity must be the ID
    OBXPropertyFlags_ID = 1,

    /// On languages like Java, a non-primitive type is used (aka wrapper types, allowing null)
    OBXPropertyFlags_NON_PRIMITIVE_TYPE = 2,

    /// Unused yet
    OBXPropertyFlags_NOT_NULL = 4,

    OBXPropertyFlags_INDEXED = 8,

    /// Unused yet
    OBXPropertyFlags_RESERVED = 16,

    /// Unique index
    OBXPropertyFlags_UNIQUE = 32,

    /// Unused yet: Use a persisted sequence to enforce ID to rise monotonic (no ID reuse)
    OBXPropertyFlags_ID_MONOTONIC_SEQUENCE = 64,

    /// Allow IDs to be assigned by the developer
    OBXPropertyFlags_ID_SELF_ASSIGNABLE = 128,

    /// Unused yet
    OBXPropertyFlags_INDEX_PARTIAL_SKIP_NULL = 256,

    /// used by References for 1) back-references and 2) to clear references to deleted objects (required for ID reuse)
    OBXPropertyFlags_INDEX_PARTIAL_SKIP_ZERO = 512,

    /// Virtual properties may not have a dedicated field in their entity class, e.g. target IDs of to-one relations
    OBXPropertyFlags_VIRTUAL = 1024,

    /// Index uses a 32 bit hash instead of the value
    /// 32 bits is shorter on disk, runs well on 32 bit systems, and should be OK even with a few collisions
    OBXPropertyFlags_INDEX_HASH = 2048,

    /// Index uses a 64 bit hash instead of the value
    /// recommended mostly for 64 bit machines with values longer >200 bytes; small values are faster with a 32 bit hash
    OBXPropertyFlags_INDEX_HASH64 = 4096,

    /// The actual type of the variable is unsigned (used in combination with numeric OBXPropertyType_*)
    OBXPropertyFlags_UNSIGNED = 8192,
} OBXPropertyFlags;

struct OBX_model;
typedef struct OBX_model OBX_model;

OBX_model* obx_model_create(void);

/// Only call when not calling obx_store_open (which will free it internally)
obx_err obx_model_free(OBX_model* model);

obx_err obx_model_error_code(OBX_model* model);
const char* obx_model_error_message(OBX_model* model);

obx_err obx_model_entity(OBX_model* model, const char* name, obx_schema_id entity_id, obx_uid entity_uid);

obx_err obx_model_property(OBX_model* model, const char* name, OBXPropertyType type, obx_schema_id property_id,
                           obx_uid property_uid);

obx_err obx_model_property_flags(OBX_model* model, OBXPropertyFlags flags);

obx_err obx_model_property_relation(OBX_model* model, const char* target_entity, obx_schema_id index_id,
                                    obx_uid index_uid);

obx_err obx_model_property_index_id(OBX_model* model, obx_schema_id index_id, obx_uid index_uid);

/// Add a standalone relation between the active entity and the target entity to the model
obx_err obx_model_relation(OBX_model* model, obx_schema_id relation_id, obx_uid relation_uid, obx_schema_id target_id,
                           obx_uid target_uid);

void obx_model_last_entity_id(OBX_model*, obx_schema_id entity_id, obx_uid entity_uid);

void obx_model_last_index_id(OBX_model* model, obx_schema_id index_id, obx_uid index_uid);

void obx_model_last_relation_id(OBX_model* model, obx_schema_id relation_id, obx_uid relation_uid);

obx_err obx_model_entity_last_property_id(OBX_model* model, obx_schema_id property_id, obx_uid property_uid);

//----------------------------------------------
// Store
//----------------------------------------------

struct OBX_store;
typedef struct OBX_store OBX_store;

typedef struct OBX_store_options {
    /// Use NULL for default value ("objectbox")
    const char* directory;

    /// Use zero for default value
    uint64_t maxDbSizeInKByte;

    /// Use zero for default value
    unsigned int fileMode;

    /// Use zero for default value
    unsigned int maxReaders;
} OBX_store_options;

typedef enum {
    OBXDebugFlags_LOG_TRANSACTIONS_READ = 1,
    OBXDebugFlags_LOG_TRANSACTIONS_WRITE = 2,
    OBXDebugFlags_LOG_QUERIES = 4,
    OBXDebugFlags_LOG_QUERY_PARAMETERS = 8,
    OBXDebugFlags_LOG_ASYNC_QUEUE = 16,
} OBXDebugFlags;

typedef struct OBX_bytes {
    void* data;
    size_t size;
} OBX_bytes;

typedef struct OBX_bytes_array {
    OBX_bytes* bytes;
    size_t count;
} OBX_bytes_array;

typedef struct OBX_id_array {
    obx_id* ids;
    size_t count;
} OBX_id_array;

OBX_store* obx_store_open_bytes(const void* model_bytes, size_t model_size, const OBX_store_options* options);

/// Note: the model is freed by calling this method
OBX_store* obx_store_open(OBX_model* model, const OBX_store_options* options);

obx_schema_id obx_store_entity_id(OBX_store* store, const char* entity_name);

obx_schema_id obx_store_entity_property_id(OBX_store* store, obx_schema_id entity_id, const char* property_name);

obx_err obx_store_await_async_completion(OBX_store* store);

obx_err obx_store_debug_flags(OBX_store* store, OBXDebugFlags flags);

obx_err obx_store_close(OBX_store* store);

//----------------------------------------------
// Transaction
//----------------------------------------------

struct OBX_txn;
typedef struct OBX_txn OBX_txn;

OBX_txn* obx_txn_begin(OBX_store* store);

OBX_txn* obx_txn_begin_read(OBX_store* store);

obx_err obx_txn_close(OBX_txn* txn);

obx_err obx_txn_abort(OBX_txn* txn);

obx_err obx_txn_commit(OBX_txn* txn);

//----------------------------------------------
// Cursor
//----------------------------------------------

struct OBX_cursor;
typedef struct OBX_cursor OBX_cursor;

OBX_cursor* obx_cursor_create(OBX_txn* txn, obx_schema_id entity_id);

OBX_cursor* obx_cursor_create2(OBX_txn* txn, const char* entity_name);

obx_err obx_cursor_close(OBX_cursor* cursor);

obx_id obx_cursor_id_for_put(OBX_cursor* cursor, obx_id id_or_zero);

/// ATTENTION: ensure that the given value memory is allocated to the next 4 bytes boundary.
/// ObjectBox needs to store bytes with sizes dividable by 4 for internal reasons.
/// Use obx_cursor_put_padded otherwise.
obx_err obx_cursor_put(OBX_cursor* cursor, obx_id id, const void* data, size_t size, bool checkForPreviousValue);

/// Prefer obx_cursor_put (non-padded) if possible, as this does a memcpy if the size is not dividable by 4.
obx_err obx_cursor_put_padded(OBX_cursor* cursor, uint64_t id, const void* data, size_t size, bool checkForPreviousValue);

obx_err obx_cursor_get(OBX_cursor* cursor, obx_id id, void** data, size_t* size);

/// Gets all objects as bytes.
/// For bigger quantities, it's recommended to iterate using obx_cursor_first and obx_cursor_next.
/// However, if the calling overhead is high (e.g. for language bindings), this method helps.
OBX_bytes_array* obx_cursor_get_all(OBX_cursor* cursor);

obx_err obx_cursor_first(OBX_cursor* cursor, void** data, size_t* size);

obx_err obx_cursor_next(OBX_cursor* cursor, void** data, size_t* size);

obx_err obx_cursor_seek(OBX_cursor* cursor, obx_id id);

obx_err obx_cursor_current(OBX_cursor* cursor, void** data, size_t* size);

obx_err obx_cursor_remove(OBX_cursor* cursor, obx_id id);

obx_err obx_cursor_remove_all(OBX_cursor* cursor);

/// Count the number of available objects
obx_err obx_cursor_count(OBX_cursor* cursor, uint64_t* count);

/// Count the number of available objects up to the specified maximum
obx_err obx_cursor_count_max(OBX_cursor* cursor, uint64_t max_count, uint64_t* out_count);

/// Results true if there is no object available (false if at least one object is available)
obx_err obx_cursor_is_empty(OBX_cursor* cursor, bool* out_is_empty);

OBX_bytes_array* obx_cursor_backlink_bytes(OBX_cursor* cursor, obx_schema_id entity_id, obx_schema_id property_id,
                                           obx_id id);

OBX_id_array* obx_cursor_backlink_ids(OBX_cursor* cursor, obx_schema_id entity_id, obx_schema_id property_id,
                                      obx_id id);

obx_err obx_cursor_rel_put(OBX_cursor* cursor, obx_schema_id relation_id, obx_id source_id, obx_id target_id);
obx_err obx_cursor_rel_remove(OBX_cursor* cursor, obx_schema_id relation_id, obx_id source_id, obx_id target_id);
OBX_id_array* obx_cursor_rel_ids(OBX_cursor* cursor, obx_schema_id relation_id, obx_id source_id);

//----------------------------------------------
// Box
//----------------------------------------------

/// A box may be used across threads
struct OBX_box;
typedef struct OBX_box OBX_box;

OBX_box* obx_box_create(OBX_store* store, obx_schema_id entity_id);

obx_err obx_box_close(OBX_box* box);

obx_id obx_box_id_for_put(OBX_box* box, obx_id id_or_zero);

obx_err obx_box_put_async(OBX_box* box, obx_id id, const void* data, size_t size, bool checkForPreviousValue,
                          uint64_t timeoutMillis);

//----------------------------------------------
// Query Builder
//----------------------------------------------

/// Not really an enum, but binary flags to use across languages
typedef enum {
    /// Reverts the order from ascending (default) to descending.
    OBXOrderFlags_DESCENDING = 1,

    /// Makes upper case letters (e.g. "Z") be sorted before lower case letters (e.g. "a").
    /// If not specified, the default is case insensitive for ASCII characters.
    OBXOrderFlags_CASE_SENSITIVE = 2,

    /// For scalars only: changes the comparison to unsigned (default is signed).
    OBXOrderFlags_UNSIGNED = 4,

    /// null values will be put last.
    /// If not specified, by default null values will be put first.
    OBXOrderFlags_NULLS_LAST = 8,

    /// null values should be treated equal to zero (scalars only).
    OBXOrderFlags_NULLS_ZERO = 16,
} OBXOrderFlags;

struct OBX_query_builder;
typedef struct OBX_query_builder OBX_query_builder;

/// Query Builder condition identifier
/// - returned by condition creating functions,
/// - used to combine conditions with any/all, thus building more complex conditions
typedef int obx_qb_cond;

OBX_query_builder* obx_qb_create(OBX_store* store, obx_schema_id entity_id);
obx_err obx_qb_close(OBX_query_builder* builder);
obx_err obx_qb_error_code(OBX_query_builder* builder);
const char* obx_qb_error_message(OBX_query_builder* builder);

obx_qb_cond obx_qb_null(OBX_query_builder* builder, obx_schema_id property_id);
obx_qb_cond obx_qb_not_null(OBX_query_builder* builder, obx_schema_id property_id);

obx_qb_cond obx_qb_string_equal(OBX_query_builder* builder, obx_schema_id property_id, const char* value,
                                bool case_sensitive);

obx_qb_cond obx_qb_string_not_equal(OBX_query_builder* builder, obx_schema_id property_id, const char* value,
                                    bool case_sensitive);
obx_qb_cond obx_qb_string_contains(OBX_query_builder* builder, obx_schema_id property_id, const char* value,
                                   bool case_sensitive);
obx_qb_cond obx_qb_string_starts_with(OBX_query_builder* builder, obx_schema_id property_id, const char* value,
                                      bool case_sensitive);
obx_qb_cond obx_qb_string_ends_with(OBX_query_builder* builder, obx_schema_id property_id, const char* value,
                                    bool case_sensitive);
obx_qb_cond obx_qb_string_greater(OBX_query_builder* builder, obx_schema_id property_id, const char* value,
                                  bool case_sensitive, bool with_equal);
obx_qb_cond obx_qb_string_less(OBX_query_builder* builder, obx_schema_id property_id, const char* value,
                               bool case_sensitive, bool with_equal);
obx_qb_cond obx_qb_string_in(OBX_query_builder* builder, obx_schema_id property_id, const char* values[], int count,
                             bool case_sensitive);

obx_qb_cond obx_qb_strings_contain(OBX_query_builder* builder, obx_schema_id property_id, const char* value,
                                   bool case_sensitive);

obx_qb_cond obx_qb_int_equal(OBX_query_builder* builder, obx_schema_id property_id, int64_t value);
obx_qb_cond obx_qb_int_not_equal(OBX_query_builder* builder, obx_schema_id property_id, int64_t value);
obx_qb_cond obx_qb_int_greater(OBX_query_builder* builder, obx_schema_id property_id, int64_t value);
obx_qb_cond obx_qb_int_less(OBX_query_builder* builder, obx_schema_id property_id, int64_t value);
obx_qb_cond obx_qb_int_between(OBX_query_builder* builder, obx_schema_id property_id, int64_t value_a, int64_t value_b);

obx_qb_cond obx_qb_int64_in(OBX_query_builder* builder, obx_schema_id property_id, const int64_t values[], int count);
obx_qb_cond obx_qb_int64_not_in(OBX_query_builder* builder, obx_schema_id property_id, const int64_t values[],
                                int count);

obx_qb_cond obx_qb_int32_in(OBX_query_builder* builder, obx_schema_id property_id, const int32_t values[], int count);
obx_qb_cond obx_qb_int32_not_in(OBX_query_builder* builder, obx_schema_id property_id, const int32_t values[],
                                int count);

obx_qb_cond obx_qb_double_greater(OBX_query_builder* builder, obx_schema_id property_id, double value);
obx_qb_cond obx_qb_double_less(OBX_query_builder* builder, obx_schema_id property_id, double value);
obx_qb_cond obx_qb_double_between(OBX_query_builder* builder, obx_schema_id property_id, double value_a,
                                  double value_b);

obx_qb_cond obx_qb_bytes_equal(OBX_query_builder* builder, obx_schema_id property_id, const void* value, size_t size);
obx_qb_cond obx_qb_bytes_greater(OBX_query_builder* builder, obx_schema_id property_id, const void* value, size_t size,
                                 bool with_equal);
obx_qb_cond obx_qb_bytes_less(OBX_query_builder* builder, obx_schema_id property_id, const void* value, size_t size,
                              bool with_equal);

/// Combines conditions[] to a new condition using operator AND (all) or OR (any)
/// Note that these functions remove original conditions from the condition list and thus affect indices of remaining
/// conditions in the list
obx_qb_cond obx_qb_all(OBX_query_builder* builder, const obx_qb_cond conditions[], int count);
obx_qb_cond obx_qb_any(OBX_query_builder* builder, const obx_qb_cond conditions[], int count);

obx_err obx_qb_param_alias(OBX_query_builder* builder, const char* alias);

obx_err obx_qb_order(OBX_query_builder* builder, obx_schema_id property_id, OBXOrderFlags flags);

/// Create a link based on a property-relation (many-to-one)
OBX_query_builder* obx_qb_link_property(OBX_query_builder* builder, obx_schema_id property_id);

/// Create a backlink based on a property-relation used in reverse (one-to-many)
OBX_query_builder* obx_qb_backlink_property(OBX_query_builder* builder, obx_schema_id source_entity_id,
                                            obx_schema_id source_property_id);

// Create a link based on a standalone relation (many-to-many)
OBX_query_builder* obx_qb_link_standalone(OBX_query_builder* builder, obx_schema_id relation_id);

// Create a backlink based on a standalone relation (many-to-many, reverse direction)
OBX_query_builder* obx_qb_backlink_standalone(OBX_query_builder* builder, obx_schema_id relation_id);

//----------------------------------------------
// Query
//----------------------------------------------
struct OBX_query;
typedef struct OBX_query OBX_query;

OBX_query* obx_query_create(OBX_query_builder* builder);
obx_err obx_query_close(OBX_query* query);

obx_err obx_query_visit(OBX_query* query, OBX_cursor* cursor, obx_data_visitor* visitor, void* visitor_arg,
                        uint64_t offset, uint64_t limit);
OBX_bytes_array* obx_query_find(OBX_query* query, OBX_cursor* cursor, uint64_t offset, uint64_t limit);
OBX_id_array* obx_query_find_ids(OBX_query* query, OBX_cursor* cursor, uint64_t offset, uint64_t limit);
obx_err obx_query_count(OBX_query* query, OBX_cursor* cursor, uint64_t* count);

/// Removes (deletes!) all matching entities.
obx_err obx_query_remove(OBX_query* query, OBX_cursor* cursor, uint64_t* count);

/// the resulting char* is valid until another call on to_string is made on the same query or until the query is freed
const char* obx_query_describe(OBX_query* query);

/// the resulting char* is valid until another call on describe_parameters is made on the same query or until the query
/// is freed
const char* obx_query_describe_params(OBX_query* query);

//----------------------------------------------
// Query parameters (obx_query_{type}_param(s))
//----------------------------------------------
obx_err obx_query_string_param(OBX_query* query, obx_schema_id entity_id, obx_schema_id property_id, const char* value);
obx_err obx_query_string_params_in(OBX_query* query, obx_schema_id entity_id, obx_schema_id property_id,
                                   const char* values[], int count);
obx_err obx_query_int_param(OBX_query* query, obx_schema_id entity_id, obx_schema_id property_id, int64_t value);
obx_err obx_query_int_params(OBX_query* query, obx_schema_id entity_id, obx_schema_id property_id, int64_t value_a,
                             int64_t value_b);
obx_err obx_query_int64_params_in(OBX_query* query, obx_schema_id entity_id, obx_schema_id property_id,
                                  const int64_t values[], int count);
obx_err obx_query_int32_params_in(OBX_query* query, obx_schema_id entity_id, obx_schema_id property_id,
                                  const int32_t values[], int count);
obx_err obx_query_double_param(OBX_query* query, obx_schema_id entity_id, obx_schema_id property_id, double value);
obx_err obx_query_double_params(OBX_query* query, obx_schema_id entity_id, obx_schema_id property_id, double value_a,
                                double value_b);
obx_err obx_query_bytes_param(OBX_query* query, obx_schema_id entity_id, obx_schema_id property_id, const void* value,
                              size_t size);

obx_err obx_query_string_param_alias(OBX_query* query, const char* alias, const char* value);
obx_err obx_query_string_params_in_alias(OBX_query* query, const char* alias, const char* values[], int count);
obx_err obx_query_int_param_alias(OBX_query* query, const char* alias, int64_t value);
obx_err obx_query_int_params_alias(OBX_query* query, const char* alias, int64_t value_a, int64_t value_b);
obx_err obx_query_int64_params_in_alias(OBX_query* query, const char* alias, const int64_t values[], int count);
obx_err obx_query_int32_params_in_alias(OBX_query* query, const char* alias, const int32_t values[], int count);
obx_err obx_query_double_param_alias(OBX_query* query, const char* alias, double value);
obx_err obx_query_double_params_alias(OBX_query* query, const char* alias, double value_a, double value_b);
obx_err obx_query_bytes_param_alias(OBX_query* query, const char* alias, const void* value, size_t size);

//----------------------------------------------
// Freeing bytes/ids/arrays
//----------------------------------------------
void obx_bytes_free(OBX_bytes* bytes);
void obx_bytes_array_free(OBX_bytes_array* bytes_array);
void obx_id_array_free(OBX_id_array* id_array);

#ifdef __cplusplus
}
#endif

#endif  // OBJECTBOX_H
