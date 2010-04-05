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
#include <gtk/gtkimcontext.h>
#include <gtk/gtkimmodule.h>
#include <gdk/gdkkeysyms.h>
#include "im-predictor-context.h"

#define UPDATE_TIMEOUT_INTERVAL 250
#define ACCEPT_CANDIDATE_KEY GDK_Return
#define ACCEPT_CANDIDATE_STATE GDK_CONTROL_MASK
#define NEXT_CANDIDATE_KEY GDK_Up
#define PREVIOUS_CANDIDATE_KEY GDK_Down
#define CHANGE_CANDIDATE_STATE GDK_CONTROL_MASK

static void     gtk_predictor_im_context_class_init        (GtkIMPredictorContextClass *context_class);
static void     gtk_predictor_im_context_init              (GtkIMPredictorContext *self);
static void     gtk_predictor_im_context_finalize          (GObject *obj);
static gboolean gtk_predictor_im_filter_keypress           (GtkIMContext *context,
							    GdkEventKey *event);
static void     gtk_predictor_im_get_preedit_string        (GtkIMContext *context,
							    gchar **str,
							    PangoAttrList **attrs,
							    gint *cursor_pos);
static void     gtk_predictor_im_context_set_client_window (GtkIMContext *context,
							    GdkWindow *window);

static GObjectClass *gtk_predictor_im_context_parent_class = NULL;
static GType gtk_predictor_im_context_type = 0;

GType gtk_predictor_im_context_get_type (void)
{
  g_assert (gtk_predictor_im_context_type != 0);
  return gtk_predictor_im_context_type;
}

void
gtk_predictor_im_context_register_type (GTypeModule *module)
{
  if (!gtk_predictor_im_context_type)
  {
    static const GTypeInfo predictor_im_context_info =
    {
      sizeof(GtkIMPredictorContextClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) gtk_predictor_im_context_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof(GtkIMPredictorContext),
      0, /* n_preallocs */
      (GInstanceInitFunc) gtk_predictor_im_context_init,
    };

    gtk_predictor_im_context_type =
      g_type_module_register_type (module,
				   GTK_TYPE_IM_CONTEXT,
				   "GtkIMPredictorContext",
				   &predictor_im_context_info,
				   0);
  }
}

static void
gtk_predictor_im_context_class_init (GtkIMPredictorContextClass *context_class)
{
  GtkIMContextClass *im_context_class = GTK_IM_CONTEXT_CLASS (context_class);
  GObjectClass *gobject_class = G_OBJECT_CLASS (context_class);

  gobject_class->finalize = gtk_predictor_im_context_finalize;

  gtk_predictor_im_context_parent_class = 
    G_OBJECT_CLASS (g_type_class_peek_parent (context_class));

  im_context_class->filter_keypress = gtk_predictor_im_filter_keypress;
  im_context_class->set_client_window =
    gtk_predictor_im_context_set_client_window;
  im_context_class->get_preedit_string = gtk_predictor_im_get_preedit_string;
}

static void
gtk_predictor_im_context_init (GtkIMPredictorContext *self)
{
  self->text_predictor = text_predictor_new ();
  self->widget = NULL;
  self->candidate_list = NULL;
  self->current_candidate = NULL;
  self->current_candidate_index = 0;
  self->prefix_offset = 0;
  self->update_timeout_id = 0;
}

static void
gtk_predictor_im_context_finalize (GObject *obj)
{
  GtkIMPredictorContext *self = GTK_PREDICTOR_IM_CONTEXT (obj);

  if (self->update_timeout_id)
  {
    g_source_remove (self->update_timeout_id);
  }

  g_free (self->current_candidate);

  g_object_unref (self->text_predictor);
}

static void
gtk_predictor_im_context_set_client_window (GtkIMContext *context,
					    GdkWindow *window)
{
  GtkIMPredictorContext *self = GTK_PREDICTOR_IM_CONTEXT (context);

  if (window)
  {
    gpointer window_widget;
    gdk_window_get_user_data (window, &window_widget);
    if (window_widget != NULL &&
	GTK_IS_WIDGET (window_widget))
    {
      self->widget = GTK_WIDGET (window_widget);
    }
  }
}

static GtkStyle *
get_gtk_style (GtkIMPredictorContext *self)
{
  GtkStyle *style = NULL;

  if (self->widget)
  {
    style = gtk_widget_get_style (self->widget);
  }

  if (style == NULL)
  {
    style = gtk_widget_get_default_style ();
  }

  return style;
}

static void
gtk_predictor_im_get_preedit_string (GtkIMContext *context,
				     gchar **str,
				     PangoAttrList **attrs,
				     gint *cursor_pos)
{
  GtkIMPredictorContext *self = GTK_PREDICTOR_IM_CONTEXT (context);
  gsize len_bytes = 0;
  gsize len_utf8_chars = 0;

  if (str)
  {
    if (self->current_candidate)
    {
      gchar *substr = g_utf8_offset_to_pointer (self->current_candidate,
						self->prefix_offset);
      *str = g_strdup (substr);
    }
    else
    {
      *str = g_strdup ("");
    }

    if(*str)
    {
      len_utf8_chars = g_utf8_strlen (*str, -1);
      len_bytes = strlen (*str);
    }
  }

  if (attrs)
  {
    *attrs = pango_attr_list_new ();

    if (len_bytes)
    {
      GtkStyle *style = get_gtk_style (self);
      GtkStateType state = GTK_STATE_SELECTED;
      PangoAttribute *attr = pango_attr_foreground_new (style->bg[state].red,
							style->bg[state].green,
							style->bg[state].blue);
      attr->start_index = 0;
      attr->end_index = len_bytes;
      pango_attr_list_insert (*attrs, attr);
      
      attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
      attr->start_index = 0;
      attr->end_index = len_bytes;
      pango_attr_list_insert (*attrs, attr);
    }
  }

  if (cursor_pos)
  {
    *cursor_pos = 0;
  }
}

static void
clean_candidate(GtkIMPredictorContext *self)
{
  if (self->update_timeout_id)
  {
    g_source_remove (self->update_timeout_id);
    self->update_timeout_id = 0;
  }

  if (self->candidate_list)
  {
    GList *list = self->candidate_list;
    for (; list; list = g_list_next (list))
    {
      gchar *candidate = (gchar *) list->data;
      g_free (candidate);
    }
    g_list_free (self->candidate_list);
    self->candidate_list = NULL;
  }

  g_free (self->current_candidate);
  self->current_candidate = NULL;
  self->current_candidate_index = 0;
  g_signal_emit_by_name (self, "preedit-changed");
}

static void
accept_candidate (GtkIMPredictorContext *self)
{
  gchar *substr;

  if (!self->current_candidate)
  {
    return;
  }

  substr = g_utf8_offset_to_pointer (self->current_candidate,
				     self->prefix_offset);
  g_signal_emit_by_name (self, "commit", substr);
  clean_candidate (self);  
}

static guint
get_last_space_char_back_offset (const gchar *text, gint position)
{
  guint length, i;

  if (!text)
  {
    return 0;
  }
  
  length = g_utf8_strlen (text, -1);
  if (length == 0)
  {
    return 0;
  }

  i = position - 1;
  while (i > 0)
  {
    gchar *substr = g_utf8_offset_to_pointer (text, i);
    gunichar current_char = g_utf8_get_char (substr);
    if (g_unichar_isspace (current_char) ||
	g_unichar_ispunct (current_char))
    {
      /* When the space is found, we want the
	 index but do not wanna count the space */
      i++;
      break;
    }
    i--;
  }
  CLAMP (i, 0, length);

  return position - i;
}

static void
update_candidate_from_list (GtkIMPredictorContext *self)
{
  gchar *candidate = NULL;
  if (!self->candidate_list)
  {
    return;
  }
  g_free (self->current_candidate);
  candidate = (gchar *) g_list_nth_data (self->candidate_list,
					 self->current_candidate_index);
  self->current_candidate = g_strdup (candidate);
  g_signal_emit_by_name (self, "preedit-changed");
}

static void
update_candidates (GtkIMPredictorContext *self)
{
  GList *list = NULL;
  gchar *surrounding = NULL;
  gint cursor_position = 0;

  gtk_im_context_get_surrounding (GTK_IM_CONTEXT (self),
				  &surrounding,
				  &cursor_position);

  if (!self->candidate_list)
  {
    list = text_predictor_get_candidates_for_surrounding (self->text_predictor,
							  surrounding,
							  cursor_position);
  }
  else
  {
    list = self->candidate_list;
  }

  if (list)
  {
    g_free (self->current_candidate);
    gchar *candidate = (gchar *) g_list_nth_data (list,
					  self->current_candidate_index);
    self->current_candidate = g_strdup (candidate);
    self->candidate_list = list;
    self->prefix_offset = get_last_space_char_back_offset (surrounding, g_utf8_pointer_to_offset (surrounding, surrounding + cursor_position));
  }

  g_signal_emit_by_name (self, "preedit-changed");
  g_free (surrounding);
}

static gboolean
update_candidate_for_timeout (gpointer data)
{
  GtkIMPredictorContext *self = GTK_PREDICTOR_IM_CONTEXT (data);
  update_candidates (self);

  return FALSE;
}

static void
update_candidate_delayed (GtkIMPredictorContext *self)
{
  if (self->update_timeout_id)
  {
    g_source_remove (self->update_timeout_id);
  }
  self->update_timeout_id = g_timeout_add (UPDATE_TIMEOUT_INTERVAL,
					   update_candidate_for_timeout,
					   self);
}

static void
next_candidate (GtkIMPredictorContext *self)
{
  if (!self->candidate_list)
  {
    update_candidates (self);
    return;
  }

  self->current_candidate_index++;
  if (self->current_candidate_index > g_list_length (self->candidate_list))
  {
    self->current_candidate_index = 0;
  }
  update_candidate_from_list (self);
}

static void
previous_candidate (GtkIMPredictorContext *self)
{
  if (!self->candidate_list)
  {
    update_candidates (self);
    return;
  }

  self->current_candidate_index--;
  if (self->current_candidate_index < 0)
  {
    self->current_candidate_index = g_list_length (self->candidate_list) - 1;
  }
  update_candidate_from_list (self);
}

static gboolean
gtk_predictor_im_filter_keypress (GtkIMContext *context,
				  GdkEventKey *event)
{
  GtkIMPredictorContext *self = GTK_PREDICTOR_IM_CONTEXT (context);

  if (event->type == GDK_KEY_RELEASE)
  {
    gunichar character = gdk_keyval_to_unicode (event->keyval);

    if (g_unichar_isprint (character) &&
	event->keyval != GDK_BackSpace)
    {
      clean_candidate (self);
      update_candidate_delayed (self);
    }
    else if (event->keyval == GDK_BackSpace)
    {
      clean_candidate (self);
      update_candidate_delayed (self);
    }
  }
  else if (event->type == GDK_KEY_PRESS)
  {
    gboolean may_change_candidate = (event->state &
				     CHANGE_CANDIDATE_STATE) != 0 ||
                                    CHANGE_CANDIDATE_STATE == 0;

    if (event->keyval == ACCEPT_CANDIDATE_KEY &&
	((event->state & ACCEPT_CANDIDATE_STATE) != 0 ||
	 ACCEPT_CANDIDATE_STATE == 0))
    {
      accept_candidate (self);
      return TRUE;
    }
    else if (event->keyval == NEXT_CANDIDATE_KEY &&
	     may_change_candidate)
    {
      next_candidate (self);
      return TRUE;
    }
    else if (event->keyval == PREVIOUS_CANDIDATE_KEY &&
	     may_change_candidate)
    {
      previous_candidate (self);
      return TRUE;
    }
    else if (self->current_candidate &&
	     (event->keyval == GDK_Left ||
	      event->keyval == GDK_Up ||
	      event->keyval == GDK_Down ||
	      event->keyval == GDK_Right))
    {
      clean_candidate (self);
      return TRUE;
    }

    guint32 keyval_char = gdk_keyval_to_unicode (event->keyval);
    if (g_unichar_isprint (keyval_char))
    {
      gchar text[6];
      gint length = g_unichar_to_utf8 (keyval_char, text);
      text[length] = '\0';
      g_signal_emit_by_name (self, "commit", text);
    }
  }

  /* To honor normal, not handled, keys' functions
     we call the regular filter_keypress handler */
  GtkIMContextClass *parent = GTK_IM_CONTEXT_CLASS (
				  gtk_predictor_im_context_parent_class);
  if(parent)
  {
    return parent->filter_keypress(context, event);
  }
  else
  {
    return FALSE;
  }
}
