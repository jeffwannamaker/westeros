/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __WESTEROS_SINK_SOC_H__
#define __WESTEROS_SINK_SOC_H__

#include <stdlib.h>

#include "simplebuffer-client-protocol.h"

#define WESTEROS_SINK_CAPS \
           "video/x-tbd;"

struct _GstWesterosSinkSoc
{
   struct wl_sb *sb;
   int activeBuffers;
};

gboolean gst_westeros_sink_soc_init( GstWesterosSink *sink );
void gst_westeros_sink_soc_term( GstWesterosSink *sink );
gboolean gst_westeros_sink_soc_null_to_ready( GstWesterosSink *sink, gboolean *passToDefault );
gboolean gst_westeros_sink_soc_ready_to_paused( GstWesterosSink *sink, gboolean *passToDefault );
gboolean gst_westeros_sink_soc_paused_to_playing( GstWesterosSink *sink, gboolean *passToDefault );
gboolean gst_westeros_sink_soc_playing_to_paused( GstWesterosSink *sink, gboolean *passToDefault );
gboolean gst_westeros_sink_soc_paused_to_ready( GstWesterosSink *sink, gboolean *passToDefault );
gboolean gst_westeros_sink_soc_ready_to_null( GstWesterosSink *sink, gboolean *passToDefault );
void gst_westeros_sink_soc_registryHandleGlobal( GstWesterosSink *sink, 
                                 struct wl_registry *registry, uint32_t id,
		                           const char *interface, uint32_t version);
void gst_westeros_sink_soc_registryHandleGlobalRemove(GstWesterosSink *sink,
                                 struct wl_registry *registry,
			                        uint32_t name);
gboolean gst_westeros_sink_soc_accept_caps( GstWesterosSink *sink, GstCaps *caps );
void gst_westeros_sink_soc_set_startPTS( GstWesterosSink *sink, gint64 pts );
void gst_westeros_sink_soc_render( GstWesterosSink *sink, GstBuffer *buffer );
void gst_westeros_sink_soc_flush( GstWesterosSink *sink );
gboolean gst_westeros_sink_soc_start_video( GstWesterosSink *sink );

#endif
