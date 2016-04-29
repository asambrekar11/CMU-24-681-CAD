#import "fslazywindow_viewcontroller.h"
#import <OpenGLES/ES2/glext.h>


#include <fssimplewindowios.h>
#include "fslazywindowios.h"


@interface FsLazyWindowViewController () {
}
@property (strong, nonatomic) EAGLContext *context;

- (void) reportToucheEvent:(NSSet<UITouch *> *)touches;

@end

@implementation FsLazyWindowViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    float scale=[self.view contentScaleFactor];
    
    FsIOSBeforeEverything();
    {
        int x0,y0,wid,hei;
        FsIOSGetWindowSizeAndPreference(&x0,&y0,&wid,&hei);
    }

    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create OpenGL ES 2.0 context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    view.multipleTouchEnabled=true;
    
    [EAGLContext setCurrentContext:self.context];

    {
        CGRect rect=[view frame];
        int wid=(int)(rect.size.width*scale);
        int hei=(int)(rect.size.height*scale);
        FsIOSReportWindowSize(wid,hei);
    }

    glClearColor(1,1,1,0);
    FsIOSInitialize();
}

- (void)dealloc
{
    FsIOSBeforeTerminate();

    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)update
{
    // 2015/12/01
    // In iOS Simulator, [self.view frame] gives correct dimension after rotation
    // In actual device, [self.view frame] does not change after rotation.  [self.view bounds] give correct dimension.
    
    float scale=[self.view contentScaleFactor];
    // CGRect rect=[self.view frame];
    CGRect bound=[self.view bounds];
    int wid=(int)(bound.size.width*scale);
    int hei=(int)(bound.size.height*scale);
    FsIOSReportWindowSize(wid,hei);
    FsIOSCallInterval();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    FsIOSDraw();
}

- (void) reportToucheEvent:(NSSet<UITouch *> *)touches
{
    const int maxNumTouch=16;
    int nTouch=0;
    int touchCoord[maxNumTouch*2];
    float scale=[self.view contentScaleFactor];

    printf("%lu\n",(unsigned long)touches.count);
    for(UITouch *t in touches)
    {
        CGPoint p=[t locationInView:self.view];
        bool touchIsActive=false;
       
        printf("%f %f ",p.x*scale,p.y*scale);
        switch([t phase])
        {
            case UITouchPhaseBegan:
                printf("Began\n");
                touchIsActive=true;
                break;
            case UITouchPhaseMoved:
                printf("Moved\n");
                touchIsActive=true;
                break;
            case UITouchPhaseStationary:
                printf("Stationary\n");
                touchIsActive=true;
                break;
            case UITouchPhaseEnded:
                printf("Ended\n");
                touchIsActive=false;
                break;
            case UITouchPhaseCancelled:
                printf("Cancelled\n");
                touchIsActive=false;
                break;
        }

        if(nTouch<maxNumTouch && true==touchIsActive)
        {
            touchCoord[nTouch*2  ]=(int)(p.x*scale);
            touchCoord[nTouch*2+1]=(int)(p.y*scale);
            ++nTouch;
        }
    }

    FsIOSReportCurrentTouch(nTouch,touchCoord);
}

- (void) touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    printf("%s\n",__FUNCTION__);
    [self reportToucheEvent:[event allTouches]];
}

- (void) touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    printf("%s\n",__FUNCTION__);
    [self reportToucheEvent:[event allTouches]];
}

- (void) touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    printf("%s\n",__FUNCTION__);
    [self reportToucheEvent:[event allTouches]];
}

- (void) touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    printf("%s\n",__FUNCTION__);
    [self reportToucheEvent:[event allTouches]];
}

@end
