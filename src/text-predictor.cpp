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

#include <glib.h>
#include <string>
#include <iostream>
#include "presage.h"
#ifdef __cplusplus
extern "C" 
{
#include "text-predictor.h"
}
#endif

static gchar *string_to_gchar (std::string str);
GList *vector_string_to_glist (std::vector<std::string> string_vector);


class PredictCallback : public PresageCallback
{
 public:
  PredictCallback(gchar* new_contents, gint cursor_pos)
  { 
    contents = std::string(new_contents);
    cursor_position = cursor_pos;
  }

  void set_contents (gchar *new_contents, gint cursor_pos)
  {
    cursor_position = cursor_pos;
    contents = std::string(new_contents);
  }
  std::string get_past_stream () const
  {
    gint length = 0;
    if (cursor_position > 0)
    {
      length = cursor_position;
    }
    return contents.substr(0, length);
  }
  std::string get_future_stream () const
  {
    gint length = contents.length();
    gint position = 0;
    if (cursor_position > 0)
    {
      position = cursor_position;
    }
    return contents.substr(position, length);
  }

 private:
  std::string contents;
  gint cursor_position;
};

struct _TextPredictor
{
  PredictCallback *callback;
  Presage *presage;
};

void
text_predictor_free (TextPredictor *text_predictor)
{
  delete text_predictor->presage;
  delete text_predictor->callback;
  g_free (text_predictor);
}

gchar *
text_predictor_get_single_candidate_for_surrounding (TextPredictor *tp,
						     gchar *surrounding,
						     guint cursor_pos)
{
  tp->callback->set_contents (surrounding, cursor_pos);
  std::vector<std::string> predictions = tp->presage->predict();

  if (predictions.size() > 0)
  {
    return string_to_gchar (predictions[0]);
  }
  return NULL;
}

GList *
text_predictor_get_candidates_for_surrounding (TextPredictor *tp,
					       gchar *surrounding,
					       guint cursor_pos)
{
  tp->callback->set_contents (surrounding, cursor_pos);
  std::vector<std::string> predictions = tp->presage->predict();

  return vector_string_to_glist (predictions);
}

TextPredictor *
text_predictor_new (void)
{
  TextPredictor *tp = g_new0 (TextPredictor, 1);
  tp->callback = new PredictCallback((gchar *) "", -1);
  tp->presage = new Presage(tp->callback);
}

static gchar *
string_to_gchar (std::string str)
{
  const char *c_str = str.c_str();
  return g_strdup (c_str);
}

GList *
vector_string_to_glist (std::vector<std::string> string_vector)
{
  GList *list = NULL;
  size_t i;

  for (size_t i = 0; i < string_vector.size(); i++)
  {
    if (string_vector[i].length())
    {
      list = g_list_append (list, g_strdup ((gchar *) string_vector[i].c_str()));
    }
  }

  return list;
}
