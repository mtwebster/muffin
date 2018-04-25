/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */

/**
 * SECTION:meta-window-texture-clone
 * @title: MetaWindowTextureClone
 * @short_description: An #ClutterClone that has been clipped to represent the useful/visible portion of a
 * #MetaWindowActor's texture.
 */

#include <config.h>

#include <math.h>

#include <gdk/gdk.h> /* for gdk_rectangle_union() */

#include <meta/errors.h>
#include <meta/window.h>
#include <meta/meta-window-texture-clone.h>

typedef struct
{
  MetaWindowActor *meta_window_actor;
} MetaWindowTextureClonePrivate;

struct _MetaWindowTextureClone
{
  ClutterClone parent_object;

  MetaWindowTextureClonePrivate *priv;
};

enum 
{
  PROP_0,
  PROP_META_WINDOW_ACTOR,
  PROP_LAST
};

G_DEFINE_TYPE_WITH_PRIVATE (MetaWindowTextureClone, meta_window_texture_clone, CLUTTER_TYPE_CLONE)

static GParamSpec *properties[PROP_LAST];

static void
adjust_clipping (MetaWindowTextureClone *clone)
{
  MetaWindow *window;

  if (clone->priv->meta_window_actor == NULL)
    {
      return;
    }

  window = meta_window_actor_get_meta_window (clone->priv->meta_window_actor);

  if (FALSE)
    {g_printerr ("allocate\n");
      ClutterActor *source;

      gint left_offs, right_offs, top_offs, bottom_offs;
      gfloat hscale, vscale, orig_width, orig_height, width, height;
      gfloat scaled_xoffs, scaled_yoffs;

      g_object_get (CLUTTER_CLONE (clone),
                    "source", &source,
                    NULL);

      clutter_actor_get_size (source, &orig_width, &orig_height);
      clutter_actor_get_size (CLUTTER_ACTOR (clone), &width, &height);

      hscale = width / orig_width;
      vscale = height / orig_height;

      meta_window_get_gtk_frame_extents (window, &left_offs, &right_offs, &top_offs, &bottom_offs);

      scaled_xoffs = (gfloat) left_offs * hscale;
      scaled_yoffs = (gfloat) top_offs * vscale;

      clutter_actor_set_clip (CLUTTER_ACTOR (clone),
                              scaled_xoffs,
                              scaled_yoffs,
                              width - ((gfloat) (left_offs + right_offs)) * hscale,
                              height - ((gfloat) (top_offs + bottom_offs)) * vscale);
g_printerr ("%f, %f, %f, %f \n", scaled_xoffs, scaled_yoffs,width - ((gfloat) (left_offs + right_offs)) * hscale,height - ((gfloat) (top_offs + bottom_offs)) * vscale);
      clutter_actor_set_anchor_point (CLUTTER_ACTOR (clone),
                                      scaled_xoffs,
                                      scaled_yoffs);
    }
}

static void
meta_window_texture_clone_allocate (ClutterActor           *actor,
                                    const ClutterActorBox  *box,
                                    ClutterAllocationFlags  flags)
{
  MetaWindowTextureClone *clone = META_WINDOW_TEXTURE_CLONE (actor);

  CLUTTER_ACTOR_CLASS (meta_window_texture_clone_parent_class)->allocate (actor, box, flags);

  adjust_clipping (clone);
}

static void
meta_window_texture_clone_set_property (GObject         *object,
                                        guint            prop_id,
                                        const GValue    *value,
                                        GParamSpec      *pspec)
{
  MetaWindowTextureClone *clone;

  clone = META_WINDOW_TEXTURE_CLONE (object);

  switch (prop_id)
    {
    case PROP_META_WINDOW_ACTOR:
      meta_window_texture_clone_set_meta_window_actor (clone, g_value_get_object (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
meta_window_texture_clone_get_property (GObject    *object,
                                        guint       prop_id,
                                        GValue     *value,
                                        GParamSpec *pspec)
{
  MetaWindowTextureClone *clone;

  clone = META_WINDOW_TEXTURE_CLONE (object);

  switch (prop_id)
    {
    case PROP_META_WINDOW_ACTOR:
      g_value_set_object (value, clone->priv->meta_window_actor);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
meta_window_texture_clone_dispose (GObject *object)
{
  MetaWindowTextureClone *clone = META_WINDOW_TEXTURE_CLONE (object);

  g_clear_object (&clone->priv->meta_window_actor);

  G_OBJECT_CLASS (meta_window_texture_clone_parent_class)->dispose (object);
}

static void
meta_window_texture_clone_init (MetaWindowTextureClone *clone)
{
  clone->priv = G_TYPE_INSTANCE_GET_PRIVATE (clone,
                                             META_TYPE_WINDOW_TEXTURE_CLONE,
                                             MetaWindowTextureClonePrivate);

  clone->priv->meta_window_actor = NULL;
}

static void
meta_window_texture_clone_class_init (MetaWindowTextureCloneClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS(klass);
  ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);

  object_class->set_property = meta_window_texture_clone_set_property;
  object_class->get_property = meta_window_texture_clone_get_property;
  object_class->dispose = meta_window_texture_clone_dispose;

  actor_class->allocate = meta_window_texture_clone_allocate;

  properties[PROP_META_WINDOW_ACTOR] = g_param_spec_object ("meta-window-actor",
                                                            "MetaWindowActor whose texture is to be cloned",
                                                            "MetaWindowActor whose texture is to be cloned",
                                                            META_TYPE_WINDOW_ACTOR,
                                                            G_PARAM_READWRITE | G_PARAM_CONSTRUCT);

  g_object_class_install_property (object_class,
                                   PROP_META_WINDOW_ACTOR,
                                   properties[PROP_META_WINDOW_ACTOR]);
}

ClutterActor *
meta_window_texture_clone_new (MetaWindowActor *meta_window_actor)
{
  return g_object_new (META_TYPE_WINDOW_TEXTURE_CLONE,
                       "meta-window-actor", meta_window_actor,
                       NULL);
}

/**
 * meta_window_texture_clone_set_meta_window_actor:
 * @clone: a #MetaWindowTextureClone
 * @meta_window_actor: (allow-none): a #MetaWindowActor, or %NULL
 *
 * Sets @meta_window_actor as the source actor whose texture will be
 * cloned by @clone.
 *
 */
void
meta_window_texture_clone_set_meta_window_actor (MetaWindowTextureClone *clone,
                                                 MetaWindowActor        *meta_window_actor)
{
  ClutterActor *texture;

  g_return_if_fail (META_IS_WINDOW_TEXTURE_CLONE (clone));

  if (clone->priv->meta_window_actor == meta_window_actor)
    {
      return;
    }

  if (clone->priv->meta_window_actor)
    {
      g_object_unref (clone->priv->meta_window_actor);
      clone->priv->meta_window_actor = NULL;
    }

  if (meta_window_actor == NULL)
    {
      clutter_clone_set_source (CLUTTER_CLONE (clone), NULL);
      return;
    }

  clone->priv->meta_window_actor = META_WINDOW_ACTOR (g_object_ref (meta_window_actor));
  texture = meta_window_actor_get_texture (clone->priv->meta_window_actor);

  clutter_clone_set_source (CLUTTER_CLONE (clone), texture);

  adjust_clipping (clone);

  g_object_notify_by_pspec (G_OBJECT (clone), properties[PROP_META_WINDOW_ACTOR]);
}

/**
 * meta_window_texture_clone_get_meta_window_actor:
 * @clone: a #MetaWindowTextureClone
 *
 * Gets the MetaWindowActor whose texture is backing this clone
 *
 * Return value: (transfer none): the #MetaWindowActor
 */
MetaWindowActor*
meta_window_texture_clone_get_meta_window_actor (MetaWindowTextureClone *clone)
{
  g_return_val_if_fail (META_IS_WINDOW_TEXTURE_CLONE (clone), NULL);

  return clone->priv->meta_window_actor;
}