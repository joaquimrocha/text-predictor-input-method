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

#include <string.h>
#include <config.h>
#include <glib/gi18n.h>
#include "im-predictor.h"
#include "im-predictor-context.h"

#define CONTEXT_ID "predictor"
 
static const
GtkIMContextInfo info =
{ 
  CONTEXT_ID,
  N_("Predictor"),
  GETTEXT_PACKAGE,
  PREDICTOR_LOCALEDIR,
  ""
};

static const
GtkIMContextInfo *info_list[] = { &info };

void
im_module_init (GTypeModule *module)
{
  gtk_predictor_im_context_register_type (module);
}

GtkIMContext *
im_module_create (const gchar *context_id)
{
  if (strcmp (context_id, CONTEXT_ID) == 0)
  {
    GtkIMContext* imcontext =
      GTK_IM_CONTEXT(g_object_new (GTK_TYPE_PREDICTOR_IM_CONTEXT, NULL));

    return imcontext;
  }
  else
  {
    return NULL;
  }
}

void 
im_module_exit (void) {}

void 
im_module_list (const GtkIMContextInfo ***contexts,
		int *n_contexts)
{
  *contexts = info_list;
  *n_contexts = G_N_ELEMENTS (info_list);
}
