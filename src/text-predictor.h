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

#ifndef __TEXT_PREDICTOR__
#define __TEXT_PREDICTOR__

#ifdef __cplusplus
extern "C"
{
#endif

#include <glib.h>

G_BEGIN_DECLS

typedef struct _TextPredictor TextPredictor;

TextPredictor *text_predictor_new (void);
void text_predictor_free (TextPredictor *tp);
GList *text_predictor_get_candidates_for_surrounding (TextPredictor *tp,
						      gchar *surrounding,
						      guint cursor_pos);
gchar *text_predictor_get_single_candidate_for_surrounding (TextPredictor *tp,
							    gchar *surrounding,
							    guint cursor_pos);

#ifdef __cplusplus
}
#endif

G_END_DECLS

#endif
