/*
 * Mesa 3-D graphics library
 *
 * Copyright (C) 1999-2008  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


/**
 * \mainpage Mesa GL API Module
 *
 * \section GLAPIIntroduction Introduction
 *
 * The Mesa GL API module is responsible for dispatching all the
 * gl*() functions.  All GL functions are dispatched by jumping through
 * the current dispatch table (basically a struct full of function
 * pointers.)
 *
 * A per-thread current dispatch table and per-thread current context
 * pointer are managed by this module too.
 *
 * This module is intended to be non-Mesa-specific so it can be used
 * with the X/DRI libGL also.
 */


#ifndef _GLAPI_H
#define _GLAPI_H

#include "util/macros.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef _GLAPI_NO_EXPORTS
#  define _GLAPI_EXPORT
#else /* _GLAPI_NO_EXPORTS */
#  ifdef _WIN32
#    ifdef _GLAPI_DLL_EXPORTS
#      define _GLAPI_EXPORT __declspec(dllexport)
#    else
#      define _GLAPI_EXPORT __declspec(dllimport)
#    endif
#  elif defined(__GNUC__)
#    define _GLAPI_EXPORT __attribute__((visibility("default")))
#  else
#    define _GLAPI_EXPORT
#  endif
#endif /* _GLAPI_NO_EXPORTS */

#include "GL/gl.h"
#include "GL/glext.h"


struct _glapi_table;

typedef void (*_glapi_proc)(void); /* generic function pointer */

typedef void (*_glapi_nop_handler_proc)(const char *name);

typedef void (*_glapi_warning_func)(void *ctx, const char *str, ...);


#if defined(USE_MGL_NAMESPACE)
#define _glapi_set_dispatch _mglapi_set_dispatch
#define _glapi_get_dispatch _mglapi_get_dispatch
#define _glapi_set_context _mglapi_set_context
#define _glapi_get_context _mglapi_get_context
#define _glapi_Dispatch _mglapi_Dispatch
#define _glapi_Context _mglapi_Context
#endif

/*
 * Number of extension functions which we can dynamically add at runtime.
 */
#define MAX_EXTENSION_FUNCS 300


/**
 ** Define the GET_CURRENT_CONTEXT() macro.
 ** \param C local variable which will hold the current context.
 **/
#if defined (GLX_USE_TLS)

_GLAPI_EXPORT extern __thread struct _glapi_table * _glapi_tls_Dispatch
  ;

_GLAPI_EXPORT extern __thread void * _glapi_tls_Context
  ;

_GLAPI_EXPORT extern const struct _glapi_table *_glapi_Dispatch;
_GLAPI_EXPORT extern const void *_glapi_Context;

# define GET_DISPATCH() _glapi_tls_Dispatch
# define GET_CURRENT_CONTEXT(C)  struct gl_context *C = (struct gl_context *) _glapi_tls_Context

#else

#ifdef INSERVER
#define SERVEXTERN _declspec(dllimport)
#else
#define SERVEXTERN _declspec(dllexport)
#endif

SERVEXTERN struct _glapi_table *_glapi_Dispatch;
SERVEXTERN void *_glapi_Context;

#define GET_DISPATCH() \
     (likely(_glapi_Dispatch) ? _glapi_Dispatch : _glapi_get_dispatch())

#define GET_CURRENT_CONTEXT(C)  struct gl_context *C = (struct gl_context *) \
     (likely(_glapi_Context) ? _glapi_Context : _glapi_get_context())

#endif /* defined (GLX_USE_TLS) */


/**
 ** GL API public functions
 **/

SERVEXTERN void
_glapi_check_multithread(void);


SERVEXTERN void
_glapi_set_context(void *context);


SERVEXTERN void *
_glapi_get_context(void);


SERVEXTERN void
_glapi_set_dispatch(struct _glapi_table *dispatch);


SERVEXTERN struct _glapi_table *
_glapi_get_dispatch(void);

SERVEXTERN int
_glapi_begin_dispatch_override(struct _glapi_table *override);

SERVEXTERN void
_glapi_end_dispatch_override(int layer);

struct _glapi_table *
_glapi_get_override_dispatch(int layer);

SERVEXTERN GLuint
_glapi_get_dispatch_table_size(void);


SERVEXTERN int
_glapi_add_dispatch( const char * const * function_names,
		     const char * parameter_signature );

_GLAPI_EXPORT _glapi_proc
_glapi_get_proc_address(const char *funcName);

_GLAPI_EXPORT const char *
_glapi_get_proc_name(unsigned int offset);

extern struct _glapi_table *
_glapi_create_table_from_handle(void *handle, const char *symbol_prefix);


void
_glapi_set_nop_handler(_glapi_nop_handler_proc func);

/** Return pointer to new dispatch table filled with no-op functions */
struct _glapi_table *
_glapi_new_nop_table(unsigned num_entries);



#ifdef __cplusplus
}
#endif

#endif /* _GLAPI_H */
