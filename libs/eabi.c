/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <stddef.h>
#include <string.h>

/* Temporary hack: this variable should not be part of the C library
 * itself, but placed in the .bss section of each executable or
 * shared library instead.
 *
 * We keep it here temporarily until the build system has been
 * modified properly to use crtbegin_so.S and crtend_so.S when
 * generating shared libraries.
 *
 * It must be a 'weak' symbol to avoid conflicts with the definitions
 * that have been moved to crtbegin_static.S and crtbegin_dynamic.S
 *
 * For the record, it is used for static C++ object construction
 * and destruction. See http://www.codesourcery.com/public/cxx-abi/abi.html#dso-dtor
 */

/* The "C++ ABI for ARM" document states that static C++ constructors,
 * which are called from the .init_array, should manually call
 * __aeabi_atexit() to register static destructors explicitely.
 *
 * Note that 'dso_handle' is the address of a magic linker-generate
 * variable from the shared object that contains the constructor/destructor
 */

/* Make this a weak symbol to avoid a multiple definition error when linking
 * with libstdc++-v3.  */


void __aeabi_memmove4(void *dest, const void *src, size_t n) {
    memmove(dest, src, n);
}

