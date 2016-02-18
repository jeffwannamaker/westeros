#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <pthread.h>
#include <unistd.h>

#include <dlfcn.h>

#include "westeros-nested.h"
#include "westeros-render.h"

#define DEFAULT_OUTPUT_WIDTH (1280)
#define DEFAULT_OUTPUT_HEIGHT (720)

#define WESTEROS_UNUSED(x) ((void)(x))


WstRenderer* WstRendererCreate( const char *moduleName, int argc, char **argv, WstNestedConnection *nc )
{
   bool error= false;
   void *module= 0, *init;
   
   WstRenderer *renderer= (WstRenderer*)calloc( 1, sizeof(WstRenderer) );
   if ( renderer )
   {
      int rc;
      const char *displayName= 0;
      int i= 0;
      int len, value;
      int width= DEFAULT_OUTPUT_WIDTH;
      int height= DEFAULT_OUTPUT_HEIGHT;
      
      while ( i < argc )
      {
         len= strlen(argv[i]);
         if ( (len == 7) && (strncmp( argv[i], "--width", len) == 0) )
         {
            if ( i+1 < argc )
            {
               ++i;
               value= atoi(argv[i]);
               if ( value > 0 )
               {
                  width= value;
               }
            }
         }
         else
         if ( (len == 8) && (strncmp( argv[i], "--height", len) == 0) )
         {
            if ( i+1 < argc )
            {
               ++i;
               value= atoi(argv[i]);
               if ( value > 0 )
               {
                  height= value;
               }
            }
         }
         ++i;
      }
      
      renderer->nc= nc;
      if ( nc )
      {
         renderer->display= WstNestedConnectionGetDisplay( nc );
         renderer->surface= WstNestedConnectionGetSurface( nc );
      }
            
      module= dlopen( moduleName, RTLD_NOW );
      if ( !module )
      {
         printf("WstRendererCreate: failed to load module (%s)\n", moduleName);
         printf("  detail: %s\n", dlerror() );
         error= true;
         goto exit;
      }
      
      init= dlsym( module, RENDERER_MODULE_INIT );
      if ( !init )
      {
         printf("WstRendererCreate: failed to find module (%s) method (%s)\n", moduleName, RENDERER_MODULE_INIT );
         printf("  detail: %s\n", dlerror() );
         error= true;
         goto exit;
      }

      renderer->outputWidth= width;
      renderer->outputHeight= height;
      
      rc= ((WSTMethodRenderInit)init)( renderer, argc, argv );
      if ( rc )
      {
         printf("WstRendererCreate: module (%s) init failed: %d\n", moduleName, rc );
         error= true;
         goto exit;
      }
      
      printf("WstRendererCreate: module (%s) loaded and intialized\n", moduleName );
   }
   
exit:

   if ( error )
   {
      if ( renderer )
      {
         WstRendererDestroy( renderer );
      }
      if ( module )
      {
         dlclose( module );
      }
   }
   
   return renderer;
}

void WstRendererDestroy( WstRenderer *renderer )
{
   if ( renderer )
   {
      if ( renderer->renderer )
      {
         renderer->renderTerm( renderer );
         renderer->renderer= 0;
      }
      free( renderer );      
   }
}

void WstRendererUpdateScene( WstRenderer *renderer )
{
   renderer->updateScene( renderer );
}

WstRenderSurface* WstRendererSurfaceCreate( WstRenderer *renderer )
{
   return renderer->surfaceCreate( renderer );
}

void WstRendererSurfaceDestroy( WstRenderer *renderer, WstRenderSurface *surface )
{
   renderer->surfaceDestroy( renderer, surface );
}

void WstRendererSurfaceCommit( WstRenderer *renderer, WstRenderSurface *surface, void *buffer )
{
   renderer->surfaceCommit( renderer, surface, buffer );
}

void WstRendererSurfaceCommitMemory( WstRenderer *renderer, WstRenderSurface *surface,
                                     void *data, int width, int height, int format, int stride )
{
   renderer->surfaceCommitMemory( renderer, surface, data, width, height, format, stride );
}

void WstRendererSurfaceSetVisible( WstRenderer *renderer, WstRenderSurface *surface, bool visible )
{
   renderer->surfaceSetVisible( renderer, surface, visible );
}

bool WstRendererSurfaceGetVisible( WstRenderer *renderer, WstRenderSurface *surface, bool *visible )
{
   return renderer->surfaceGetVisible( renderer, surface, visible );
}

void WstRendererSurfaceSetGeometry( WstRenderer *renderer, WstRenderSurface *surface, int x, int y, int width, int height )
{
   renderer->surfaceSetGeometry( renderer, surface, x, y, width, height );
}

void WstRendererSurfaceGetGeometry( WstRenderer *renderer, WstRenderSurface *surface, int *x, int *y, int *width, int *height )
{
   renderer->surfaceGetGeometry( renderer, surface, x, y, width, height );
}

void WstRendererSurfaceSetOpacity( WstRenderer *renderer, WstRenderSurface *surface, float opacity )
{
   renderer->surfaceSetOpacity( renderer, surface, opacity );
}

float WstRendererSurfaceGetOpacity( WstRenderer *renderer, WstRenderSurface *surface, float *opacity )
{
   return renderer->surfaceGetOpacity( renderer, surface, opacity );
}

void WstRendererSurfaceSetZOrder( WstRenderer *renderer, WstRenderSurface *surface, float z )
{
   renderer->surfaceSetZOrder( renderer, surface, z );
}

float WstRendererSurfaceGetZOrder( WstRenderer *renderer, WstRenderSurface *surface, float *z )
{
   return renderer->surfaceGetZOrder( renderer, surface, z );
}


