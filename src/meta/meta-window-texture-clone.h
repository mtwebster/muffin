/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/*
 * Copyright (C) 2008 Matthew Allum
 * Copyright (C) 2007 Iain Holmes
 * Based on xcompmgr - (c) 2003 Keith Packard
 *          xfwm4    - (c) 2005-2007 Olivier Fourdan
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Suite 500, Boston, MA
 * 02110-1335, USA.
 */

#ifndef META_WINDOW_TEXTURE_CLONE_H_
#define META_WINDOW_TEXTURE_CLONE_H_

#include <clutter/clutter.h>
#include "meta-window-actor.h"

G_BEGIN_DECLS

#define META_TYPE_WINDOW_TEXTURE_CLONE (meta_window_texture_clone_get_type())

G_DECLARE_FINAL_TYPE (MetaWindowTextureClone, meta_window_texture_clone, META, WINDOW_TEXTURE_CLONE, ClutterClone)

ClutterActor    *meta_window_texture_clone_new                   (MetaWindowActor *window_actor);
void             meta_window_texture_clone_set_meta_window_actor (MetaWindowTextureClone *clone,
                                                                  MetaWindowActor        *meta_window_actor);
MetaWindowActor *meta_window_texture_clone_get_meta_window_actor (MetaWindowTextureClone *clone);

G_END_DECLS

#endif /* META_WINDOW_TEXTURE_CLONE_H_ */
