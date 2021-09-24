/*
 * Licensed to Elasticsearch B.V. under one or more contributor
 * license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright
 * ownership. Elasticsearch B.V. licenses this file to you under
 * the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#pragma once

#include <php.h>
#include <zend.h>
#include <zend_API.h>
#include <zend_modules.h>

#include "Tracer.h"
#include "elastic_apm_version.h"

extern zend_module_entry elastic_apm_module_entry;

#if defined( ZTS ) && defined( COMPILE_DL_ELASTIC_APM )
ZEND_TSRMLS_CACHE_EXTERN()
#endif

ZEND_BEGIN_MODULE_GLOBALS( elastic_apm )
Tracer globalTracer;
ZEND_END_MODULE_GLOBALS( elastic_apm )

ZEND_EXTERN_MODULE_GLOBALS( elastic_apm )

#define ZEND_PARSE_PARAMETERS_START_EX( flags, min_num_args, max_num_args )                           \
    do {                                                                                              \
        const int _flags = ( flags );                                                                 \
        uint32_t _min_num_args = ( min_num_args );                                                    \
        uint32_t _max_num_args = (uint32_t) ( max_num_args );                                         \
        uint32_t _num_args = EX_NUM_ARGS();                                                           \
        uint32_t _i = 0;                                                                              \
        zval *_real_arg, *_arg = NULL;                                                                \
        zend_expected_type _expected_type = Z_EXPECTED_LONG;                                          \
        char* _error = NULL;                                                                          \
        bool _dummy = 0;                                                                              \
        bool _optional = 0;                                                                           \
        int _error_code = ZPP_ERROR_OK;                                                               \
        ( (void) _i );                                                                                \
        ( (void) _real_arg );                                                                         \
        ( (void) _arg );                                                                              \
        ( (void) _expected_type );                                                                    \
        ( (void) _error );                                                                            \
        ( (void) _optional );                                                                         \
        ( (void) _dummy );                                                                            \
                                                                                                      \
        do {                                                                                          \
            if ( UNEXPECTED( _num_args < _min_num_args ) || UNEXPECTED( _num_args > _max_num_args ) ) \
            {                                                                                         \
                if ( ! ( _flags & ZEND_PARSE_PARAMS_QUIET ) )                                         \
                {                                                                                     \
                    zend_wrong_parameters_count_error( _min_num_args, _max_num_args );                \
                }                                                                                     \
                _error_code = ZPP_ERROR_FAILURE;                                                      \
                break;                                                                                \
            }                                                                                         \
            _real_arg = ZEND_CALL_ARG( execute_data, 0 );

#define ZEND_PARSE_PARAMETERS_START( min_num_args, max_num_args ) \
    ZEND_PARSE_PARAMETERS_START_EX( 0, min_num_args, max_num_args )


#define ZEND_PARSE_PARAMETERS_END_EX( failure )                                          \
    ZEND_ASSERT( _i == _max_num_args || _max_num_args == (uint32_t) -1 );                \
    }                                                                                    \
    while ( 0 )                                                                          \
        ;                                                                                \
    if ( UNEXPECTED( _error_code != ZPP_ERROR_OK ) )                                     \
    {                                                                                    \
        if ( ! ( _flags & ZEND_PARSE_PARAMS_QUIET ) )                                    \
        {                                                                                \
            zend_wrong_parameter_error( _error_code, _i, _error, _expected_type, _arg ); \
        }                                                                                \
        failure;                                                                         \
    }                                                                                    \
    }                                                                                    \
    while ( 0 )

#define ZEND_PARSE_PARAMETERS_END() \
    ZEND_PARSE_PARAMETERS_END_EX( return )


#define Z_PARAM_STRING_EX( dest, dest_len, check_null, deref )                             \
    Z_PARAM_PROLOGUE( deref, 0 );                                                          \
    if ( UNEXPECTED( ! zend_parse_arg_string( _arg, &dest, &dest_len, check_null, _i ) ) ) \
    {                                                                                      \
        _expected_type = check_null ? Z_EXPECTED_STRING_OR_NULL : Z_EXPECTED_STRING;       \
        _error_code = ZPP_ERROR_WRONG_ARG;                                                 \
        break;                                                                             \
    }

#define Z_PARAM_STRING( dest, dest_len ) \
    Z_PARAM_STRING_EX( dest, dest_len, 0, 0 )

#define Z_PARAM_STRING_OR_NULL( dest, dest_len ) \
    Z_PARAM_STRING_EX( dest, dest_len, 1, 0 )

#define Z_PARAM_PROLOGUE( deref, separate )               \
    _i++;                                                 \
    ZEND_ASSERT( _i <= _min_num_args || _optional == 1 ); \
    ZEND_ASSERT( _i > _min_num_args || _optional == 0 );  \
    if ( _optional )                                      \
    {                                                     \
        if ( UNEXPECTED( _i > _num_args ) ) break;        \
    }                                                     \
    _real_arg++;                                          \
    _arg = _real_arg;                                     \
    if ( deref )                                          \
    {                                                     \
        if ( EXPECTED( Z_ISREF_P( _arg ) ) )              \
        {                                                 \
            _arg = Z_REFVAL_P( _arg );                    \
        }                                                 \
    }                                                     \
    if ( separate )                                       \
    {                                                     \
        SEPARATE_ZVAL_NOREF( _arg );                      \
    }

#define ZPP_ERROR_OK 0
#define ZPP_ERROR_FAILURE 1
#define ZPP_ERROR_WRONG_CALLBACK 2
#define ZPP_ERROR_WRONG_CLASS 3
#define ZPP_ERROR_WRONG_CLASS_OR_NULL 4
#define ZPP_ERROR_WRONG_CLASS_OR_STRING 5
#define ZPP_ERROR_WRONG_CLASS_OR_STRING_OR_NULL 6
#define ZPP_ERROR_WRONG_CLASS_OR_LONG 7
#define ZPP_ERROR_WRONG_CLASS_OR_LONG_OR_NULL 8
#define ZPP_ERROR_WRONG_ARG 9
#define ZPP_ERROR_WRONG_COUNT 10
#define ZPP_ERROR_UNEXPECTED_EXTRA_NAMED 11
#define ZPP_ERROR_WRONG_CALLBACK_OR_NULL 12

#define Z_EXPECTED_TYPES( _ )                                                              \
    _( Z_EXPECTED_LONG, "of type int" )                                                    \
    _( Z_EXPECTED_LONG_OR_NULL, "of type ?int" )                                           \
    _( Z_EXPECTED_BOOL, "of type bool" )                                                   \
    _( Z_EXPECTED_BOOL_OR_NULL, "of type ?bool" )                                          \
    _( Z_EXPECTED_STRING, "of type string" )                                               \
    _( Z_EXPECTED_STRING_OR_NULL, "of type ?string" )                                      \
    _( Z_EXPECTED_ARRAY, "of type array" )                                                 \
    _( Z_EXPECTED_ARRAY_OR_NULL, "of type ?array" )                                        \
    _( Z_EXPECTED_ARRAY_OR_LONG, "of type array|int" )                                     \
    _( Z_EXPECTED_ARRAY_OR_LONG_OR_NULL, "of type array|int|null" )                        \
    _( Z_EXPECTED_ITERABLE, "of type iterable" )                                           \
    _( Z_EXPECTED_ITERABLE_OR_NULL, "of type ?iterable" )                                  \
    _( Z_EXPECTED_FUNC, "a valid callback" )                                               \
    _( Z_EXPECTED_FUNC_OR_NULL, "a valid callback or null" )                               \
    _( Z_EXPECTED_RESOURCE, "of type resource" )                                           \
    _( Z_EXPECTED_RESOURCE_OR_NULL, "of type resource or null" )                           \
    _( Z_EXPECTED_PATH, "of type string" )                                                 \
    _( Z_EXPECTED_PATH_OR_NULL, "of type ?string" )                                        \
    _( Z_EXPECTED_OBJECT, "of type object" )                                               \
    _( Z_EXPECTED_OBJECT_OR_NULL, "of type ?object" )                                      \
    _( Z_EXPECTED_DOUBLE, "of type float" )                                                \
    _( Z_EXPECTED_DOUBLE_OR_NULL, "of type ?float" )                                       \
    _( Z_EXPECTED_NUMBER, "of type int|float" )                                            \
    _( Z_EXPECTED_NUMBER_OR_NULL, "of type int|float|null" )                               \
    _( Z_EXPECTED_ARRAY_OR_STRING, "of type array|string" )                                \
    _( Z_EXPECTED_ARRAY_OR_STRING_OR_NULL, "of type array|string|null" )                   \
    _( Z_EXPECTED_STRING_OR_LONG, "of type string|int" )                                   \
    _( Z_EXPECTED_STRING_OR_LONG_OR_NULL, "of type string|int|null" )                      \
    _( Z_EXPECTED_OBJECT_OR_CLASS_NAME, "an object or a valid class name" )                \
    _( Z_EXPECTED_OBJECT_OR_CLASS_NAME_OR_NULL, "an object, a valid class name, or null" ) \
    _( Z_EXPECTED_OBJECT_OR_STRING, "of type object|string" )                              \
    _( Z_EXPECTED_OBJECT_OR_STRING_OR_NULL, "of type object|string|null" )


typedef int64_t zend_long;
ResultCode registerElasticApmIniEntries( int module_number, IniEntriesRegistrationState* iniEntriesRegistrationState );
void unregisterElasticApmIniEntries( int module_number, IniEntriesRegistrationState* iniEntriesRegistrationState );
