/*
 * FPU helper for the DJGPP (DOS) port.
 *
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MPLAYER_DJGPP_FPU_H
#define MPLAYER_DJGPP_FPU_H

#ifdef __DJGPP__
/**
 * Reinitialize the x87 FPU and mask all FPU exceptions (control word 0x037F).
 * Under DPMI an unmasked FPU exception (e.g. left over by MMX code without
 * emms or by a denormal/overflow in a codec) raises IRQ 13 / SIGFPE and
 * kills the player, so this is called at critical points.
 */
static inline void djgpp_fpu_reset(void)
{
    unsigned short cw = 0x037F;
    __asm__ __volatile__(
        "fninit\n\t"
        "fldcw %0\n\t"
        :
        : "m" (cw)
    );
}

static inline void djgpp_fpu_clear(void)
{
    __asm__ __volatile__(
        "emms\n\t"
        "fnclex\n\t"
    );
}
#else
#define djgpp_fpu_reset() do {} while (0)
#define djgpp_fpu_clear() do {} while (0)
#endif

#endif /* MPLAYER_DJGPP_FPU_H */
