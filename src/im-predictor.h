/*
 * Copyright (C) 2010 Igalia, S.L.
 * 
 * Author: Joaquim Rocha <jrocha@igalia.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __GTK_IM_PREDICTOR_H
#define __GTK_IM_PREDICTOR_H

#include <gtk/gtkimmodule.h>

G_BEGIN_DECLS

void im_module_init (GTypeModule *module);
void im_module_exit (void);
void im_module_list (const GtkIMContextInfo ***contexts,
		     int *n_contexts);
GtkIMContext *im_module_create (const gchar *context_id);

G_END_DECLS


#endif
