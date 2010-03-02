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

#ifndef __GTK_PREDICTOR_IM_CONTEXT_H
#define __GTK_PREDICTOR_IM_CONTEXT_H

#include <gtk/gtkimcontext.h>
#include "text-predictor.h"

G_BEGIN_DECLS

#define GTK_TYPE_PREDICTOR_IM_CONTEXT              (gtk_predictor_im_context_get_type())
#define GTK_PREDICTOR_IM_CONTEXT(obj)              (GTK_CHECK_CAST ((obj), GTK_TYPE_PREDICTOR_IM_CONTEXT, GtkIMPredictorContext))
#define GTK_PREDICTOR_IM_CONTEXT_CLASS(klass)      (GTK_CHECK_CLASS_CAST ((klass), GTK_TYPE_PREDICTOR_IM_CONTEXT, GtkIMPredictorContextClass))
#define GTK_IS_PREDICTOR_IM_CONTEXT(obj)           (GTK_CHECK_TYPE ((obj), GTK_TYPE_PREDICTOR_IM_CONTEXT))
#define GTK_IS_PREDICTOR_IM_CONTEXT_CLASS(klass)   (GTK_CHECK_CLASS_TYPE ((klass), GTK_TYPE_PREDICTOR_IM_CONTEXT))
#define GTK_PREDICTOR_IM_CONTEXT_GET_CLASS(obj)    (GTK_CHECK_GET_CLASS ((obj), GTK_TYPE_PREDICTOR_IM_CONTEXT, GtkIMPredictorContextClass))


typedef struct _GtkIMPredictorContext GtkIMPredictorContext;

struct _GtkIMPredictorContext
{
  GtkIMContext parent;

  TextPredictor *text_predictor;
  GtkWidget *widget;
  gchar *current_candidate;
  guint prefix_offset;
  gint update_timeout_id;
  GList *candidate_list;
  gint current_candidate_index;
};


typedef struct _GtkIMPredictorContextClass  GtkIMPredictorContextClass;

struct _GtkIMPredictorContextClass
{
  GtkIMContextClass parent_class;
};

void gtk_predictor_im_context_register_type(GTypeModule* type_module);
GType gtk_predictor_im_context_get_type (void) G_GNUC_CONST;
GtkIMContext *gtk_predictor_im_context_new (void);

G_END_DECLS

#endif
