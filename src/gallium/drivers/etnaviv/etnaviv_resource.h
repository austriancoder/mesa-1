/*
 * Copyright (c) 2012-2015 Etnaviv Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Wladimir J. van der Laan <laanwj@gmail.com>
 */

#ifndef H_ETNAVIV_RESOURCE
#define H_ETNAVIV_RESOURCE

#include "etnaviv_internal.h"
#include "etnaviv_tiling.h"
#include "pipe/p_state.h"

struct pipe_screen;

struct etna_resource_level
{
   unsigned width, padded_width;
   unsigned height, padded_height;
   unsigned offset; /* offset into memory area */
   uint32_t stride; /* row stride */
   uint32_t layer_stride; /* layer stride */
   unsigned size; /* total size of memory area */

   uint32_t ts_offset;
   uint32_t ts_size;
   uint32_t clear_value; /* clear value of resource level (mainly for TS) */
};

struct etna_resource
{
    struct pipe_resource base;

    /* only lod 0 used for non-texture buffers */
    /* Layout for surface (tiled, multitiled, split tiled, ...) */
    enum etna_surface_layout layout;
    /* Horizontal alignment for texture unit (TEXTURE_HALIGN_*) */
    unsigned halign;
    struct etna_bo *bo; /* Surface video memory */
    struct etna_bo *ts_bo; /* Tile status video memory */

    struct etna_resource_level levels[ETNA_NUM_LOD];
};

static inline struct etna_resource *
etna_resource(struct pipe_resource *p)
{
    return (struct etna_resource *)p;
}

/* Allocate Tile Status for an etna resource.
 * Tile status is a cache of the clear status per tile. This means a smaller surface
 * has to be cleared which is faster. This is also called "fast clear".
 */
bool etna_screen_resource_alloc_ts(struct pipe_screen *pscreen, struct etna_resource *prsc);

void etna_resource_screen_init(struct pipe_screen *pscreen);

#endif