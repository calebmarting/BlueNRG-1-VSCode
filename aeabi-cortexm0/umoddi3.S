/* Runtime ABI for the ARM Cortex-M0  
 * uldivmod.S: unsigned 64 bit division
 *
 * Copyright (c) 2012 Jörg Mische <bobbl@gmx.de>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */



	.syntax unified
	.text
	.thumb
	.cpu cortex-m0


@ unsigned long long __umoddi3(unsigned long long num, unsigned long long denom)
@
@ libgcc wrapper: use __aeabi_uldivmod() and return remainder
@
	.thumb_func
        .global __umoddi3
__umoddi3:
	push	{lr}
	bl	__aeabi_uldivmod
	mov	r0, r2
	mov	r1, r3
	pop	{pc}
