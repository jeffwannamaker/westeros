#ifndef _WESTEROS_COMPOSITOR_H
#define _WESTEROS_COMPOSITOR_H

typedef struct _WstCompositor WstCompositor;

typedef enum _WstKeyboard_keyState
{
   WstKeyboard_keyState_released,
   WstKeyboard_keyState_depressed,
   WstKeyboard_keyState_none
} WstKeyboard_keyState;

typedef enum _WstKeyboad_modifiers
{
   WstKeyboard_shift= (1<<0),
   WstKeyboard_alt=   (1<<1),
   WstKeyboard_ctrl=  (1<<2),
   WstKeyboard_caps=  (1<<3)
} WstKeyboard_modifiers;

typedef enum _WstPointer_buttonState
{
   WstPointer_buttonState_released,
   WstPointer_buttonState_depressed
} WstPointer_buttonState;

typedef enum _WstClient_status
{
   WstClient_started,
   WstClient_stoppedNormal,
   WstClient_stoppedAbnormal,
   WstClient_connected,
   WstClient_disconnected
} WstClient_status;

typedef void (*WstInvalidateSceneCallback)( WstCompositor *ctx, void *userData );
typedef void (*WstHidePointerCallback)( WstCompositor *ctx, bool hidePointer, void *userData );
typedef void (*WstClientStatus)( WstCompositor *ctx, int status, int clientPID, int detail, void *userData );

/**
 * WestCompositorCreate
 *
 * Create a new compositor instance.  The caller should configure
 * the instance with WstCompositorSet* calls and then start the
 * compositor operation by calling WstCompositorStart.
 */
WstCompositor* WstCompositorCreate();

/** 
 * WstCompositorDestroy
 *
 * Destroy a compositor instance.  If the compositor is running
 * it will be stopped, and then all resources will be freed.
 */
void WstCompositorDestroy( WstCompositor *ctx );

/**
 * WstCompositorGetLastErrorDetail
 *
 * Returns a null terminated string giving information about the
 * last error that has occurred.
 */
const char *WstCompositorGetLastErrorDetail( WstCompositor *ctx );

/**
 * WstCompositorSetDisplayName
 *
 * Specify the name of the wayland display that this instance will
 * create.  This must be called prior to WstCompositorStart.  If not
 * called, the behaviour is as follows: for a nested compositor a
 * display name will be generated, for a non-nested compositor the
 * default display name of 'wayland-0' will be used.  The display
 * name of a compositor can be obtained using WstCompositorGetDisplayName.
 */
bool WstCompositorSetDisplayName( WstCompositor *ctx, const char *displayName );

/**
 * WstCompositorSetFrameRate
 *
 * Specity the rate in frames per second (fps) that the compositor should
 * generate each new composited output frame.  This can be called at any time.
 */
bool WstCompositorSetFrameRate( WstCompositor *ctx, unsigned int frameRate );

/**
 * WstCompositorSetRendererModule
 *
 * Specify the name of the module the compositor will use for rendering.  This
 * will be a shared library file name without path.  An example module 
 * name might be libwesteros_render_gl.so.0.  This must be called prior
 * to WstCompositorStart.
 */
bool WstCompositorSetRendererModule( WstCompositor *ctx, const char *rendererModule );

/**
 * WstCompositorSetIsNested
 *
 * Specify if the compositor is to act as a nested compositor.  When acting
 * as a nested compositor, the compositor will create a wayland display that
 * clients can connect and render to, but the compositor will act as a client
 * to another compositor and its output frames will be drawn to a surface
 * of the second compositor.
 */
bool WstCompositorSetIsNested( WstCompositor *ctx, bool isNested );

/**
 * WstCompositorSetIsEmbedded
 *
 * Specify if the compositor is to act as an embedded compositor.  When acting
 * as an embedded compositor, the compositor will create a wayland display that
 * clients can connect and render to, but the compositor will only compose
 * its scene when WstCompositorComposeEmbedded is called.  An embedded
 * compositor should use libwesteros_render_embedded.so.0 as its
 * renderer module (or some other module that supports embedded composition).
 */
bool WstCompositorSetIsEmbedded( WstCompositor *ctx, bool isEmbedded );

/**
 * WstCompositorSetNestedDisplayName
 *
 * Specify the wayland display name that this compositor instance should connect
 * and render to as a nested compositor.  This must be called prior to 
 * WstCompositorStart.
 */
bool WstCompositorSetNestedDisplayName( WstCompositor *ctx, const char *nestedDisplayName );

/**
 * WstCompositorSetNestedSize
 *
 * Specify the size of the surface which should be created on the display
 * specified with WstCompositorSetNestedDisplayName in which to display the
 * composited output.  This must be called prior to WstCompositorStart.
 */
bool WstCompositorSetNestedSize( WstCompositor *ctx, unsigned int width, unsigned int height );

/**
 * WstCompositorSetAllowCursorModification
 *
 * Specify whether compositor clients are permitted to modify the pointer cursor
 * image.  This must be called prior to WstCompositorStart.
 */
bool WstCompositorSetAllowCursorModification( WstCompositor *ctx, bool allow );

/**
 * WstCompositorGetOutputDimensions
 *
 * Obtain the width and height of the compositor output.
 */
void WstCompositorGetOutputDimensions( WstCompositor *ctx, unsigned int *width, unsigned int *height );
 
/**
 * WstCompositorGetDisplayName
 *
 * Obtain the display name used by this compositor instance.  This will 
 * be the name set prior to start via WstCompositorSetDisplayName or, for
 * a nested compositor for which no name was specified, the display name
 * that was automatically generated.  This can be called at any time.
 */
const char *WstCompositorGetDisplayName( WstCompositor *ctx );

/**
 * WstCompositorGetFrameRate
 *
 * Obtain the current output frame rate being used by the 
 * compositor instance.  The returned value will be in 
 * frames per second (fps).  This can be called at any time.
 */
unsigned int WstCompositorGetFrameRate( WstCompositor *ctx );

/**
 * WstCompositorGetRendererModule
 *
 * Obtain the name of the renderer module being used by
 * this compositor instance.  This can be called at any time.
 */
const char *WstCompositorGetRenderModule( WstCompositor *ctx );

/**
 * WstCompositorGetIsNested
 *
 * Determine if this compsitor instance is acting as a nested
 * compositor or not.  This may be called at any time.
 */
bool WstCompositorGetIsNested( WstCompositor *ctx );

/**
 * WstCompositorGetIsEmbedded
 *
 * Determine if this compsitor instance is acting as an embedded
 * compositor or not.  This may be called at any time.
 */
bool WstCompositorGetIsEmbedded( WstCompositor *ctx );

/**
 * WstCompositorGetNestedDisplayName
 *
 * Obtain the name of the wayland display that this compositor
 * instance will be, or is using to connect to as a nested
 * compositor.  This can be called at any time.
 */
const char *WstCompositorGetNestedDisplayName( WstCompositor *ctx );

/**
 * WstCompositorGetNestedSize
 *
 * Obtain the size of surface this compositor instance will create
 * or has created on another wayland display as a nested compositor.
 * This can be called at any time.
 */
void WstCompositorGetNestedSize( WstCompositor *ctx, unsigned int *width, unsigned int *height );

/**
 * WstCompositorGetAllowCursorModification
 *
 * Determine if this compsitor instance is configured to allow
 * compositor clients to modify the pointer cursor image.  
 * This may be called at any time.
 */
bool WstCompositorGetAllowCursorModification( WstCompositor *ctx );

/**
 * WstCompositorSetInvalidateCallback
 *
 * Specifies a callback for an embedded compositor to invoke to signal that its
 * scene has become invalid and that WstCompositorComposeEmbedded should be called.
 */
bool WstCompositorSetInvalidateCallback( WstCompositor *ctx, WstInvalidateSceneCallback cb, void *userData );

/**
 * WstCompositorSetHidePointerCallback
 *
 * Specifies a callback for an embedded compositor to invoke to signal that any
 * pointer image being displayed by the process embedding this compositor should be
 * hidden or shown.  The embedded compositor will request the host cursor be hidden
 * when a client requests a different pointer be used.
 */
bool WstCompositorSetHidePointerCallback( WstCompositor *ctx, WstHidePointerCallback cb, void *userData );

/**
 * WstCompositorSetClientStatusCallback
 *
 * Specifies a callback for an embedded compositor to invoke to signal the status of a
 * client process.  The callback will supply a status value from the WstClient_status
 * enum and the client pid.  If the status is WstClient_status_stoppedAbnormal the detail
 * value will be the signal that caused the client to terminate.
 * supply the pid of the client process.
 */
bool WstCompositorSetClientStatusCallback( WstCompositor *ctx, WstClientStatus cb, void *userData );

/**
 * WstCompositorComposeEmbedded
 *
 * Requests that the current scene be composed as part of the
 * configured embedded environment.  This should only be called
 * while the compositor is running.
 */
bool WstCompositorComposeEmbedded( WstCompositor *ctx, int width, int height, int resW, int resH, float *matrix, float alpha );

/**
 * WstCompositorStart
 *
 * Start the compositor operating.  This will cause the compositor to create its
 * wayland display, connect to its target wayland display if acting as a nested 
 * compositor, and start processing events.  The function is not blocking and will
 * return as soon as the compositor is operating.
 */
bool WstCompositorStart( WstCompositor *ctx );

/**
 * WstCompositorStop
 *
 * Stops the operation of a compositor.  The compositor will halt all operation
 * and release all resources.
 */
void WstCompositorStop( WstCompositor *ctx );

/**
 * WstCompositorKeyEvent
 *
 * Pass a key event to the compositor.  The compositor will route the event
 * to an appropriate compositor client.
 */
void WstCompositorKeyEvent( WstCompositor *ctx, int keyCode, unsigned int keyState, unsigned int modifiers );

/**
 * WstCompositorPointerEnter
 *
 * Notifiy compositor that the pointer has entered its bounds.
 */
void WstCompositorPointerEnter( WstCompositor *ctx );

/**
 * WstCompositorPointerLeave
 *
 * Notifiy compositor that the pointer has exited its bounds.
 */
void WstCompositorPointerLeave( WstCompositor *ctx );

/**
 * WstCompositorPointerMoveEvent
 *
 * Pass a pointer move event to the compositor.  Th compositor will route the event
 * to an appropriate compositor client.
 */
void WstCompositorPointerMoveEvent( WstCompositor *ctx, int x, int y );

/**
 * WstCompositorPointerButtonEvent
 *
 * Pass a pointer button event to the compositor.  The compositor will route the event
 * to an appropriate compositor client.
 */
void WstCompositorPointerButtonEvent( WstCompositor *ctx, unsigned int button, unsigned int buttonState );


/**
 * WstCompositorLaunchClient
 *
 * Launch a named process intended to connect to the compositor as a client.  This should only be called
 * while the compositor is running.  The function is blocking and will not return until the client
 * process terminates or fails to launch.
 */
bool WstCompositorLaunchClient( WstCompositor *ctx, const char *cmd );

#endif

