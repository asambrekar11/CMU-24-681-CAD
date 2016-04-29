
From Game Application

1.  Create a Game Application.  Select "Objective-C" as Language, and "OpenGL ES" as Game Technology.
    (When copied from a template, *M*A*K*E* *S*U*R*E* to change the project name.  Otherwise it may be mixed up with previous build.)

2.  Add all XCode Projects (*.xcodeproj) under public/iOS
3.  In "Build Phase" of the new project, select "Link Libraries" and add all .a files.
4.  In GameViewController.h, add:
      #import "fslazywindow_viewcontroller.h"
    You may see an error in the source editor, but it is until you build once.  
    After you build once, this header file is copied to the location where XCode can recognize.

5.  It's a good idea to check that the build succeeds at this point.
    CAUTION:  If you skip or fail to do step 3, header files won't be copied, and you will get a compiler error.

6.  In GameViewController.h, replace:
      @interface GameViewController : GLKViewController
    with:
      @interface GameViewController : FsLazyWindowViewController
7.  Add FsLazyWindow application source files.  (Like main.cpp in the template)
    Source file does not have to be in the same source directory as the XCode project.
    It's a good idea to first play with a file that is supposed to work, then work on your own.
8.  In GameViewController.m, make it empty.  Because the view controller included in the FsLazyWindow library, all you need in here is:
		#import "GameViewController.h"
		@interface GameViewController () {
		}
		@end
		@implementation GameViewController
		@end



From Single-View Application

1.  Create Single-View Application
2.  Add all XCode Projects (*.xcodeproj) under public/iOS
3.  In "Build Phase" of the new project, select "Link Libraries" and add all .a files.
4.  In ViewController.h, add
      #import "fslazywindow_viewcontroller.h"
    You may see an error in the source editor, but it is until you build once.  
    After you build once, this header file is copied to the location where XCode can recognize.
5.  It's a good idea to check that the build succeeds at this point.
6.  In ViewController.h, replace:
      @interface ViewController : UIViewController
    with:
      @interface ViewController : FsLazyWindowViewController
7.  Add FsLazyWindow application source files.  (Like main.cpp in the template)
    Source file does not have to be in the same source directory as the XCode project.
    It's a good idea to first play with a file that is supposed to work, then work on your own.
8.  If you run at this time, you'll see a runtime error saying
     [GLKViewController loadView] loaded the "something" nib but didn't get a GLKView
    The error is because in .xib file the view is supposed to use UIView not GLKView.

9.  Select Main.storyboard, open "View Controller", and select "View"
10. On the right-hand side pane, find the rectangular icon enclosing a smaller rect angle and horizontal lines.
    In my default setting in XCode 7, it is third from the left.
    By the way, Apple's user interface design is definitely rotting and coming close to Microsoft.  I cannot make sense of these icons.)

    In the tab, find Custom Class.  By default, "UIView" is selected.  Change it to "GLKView".



Alternatively (Steps 7-10) you can:
7.  In the storyboard, select "View Controller", and click on the rectangular icon in the top-right tab.
8.  Select "FsLazyWindowViewController" as Class.
9.  Click on "View" in the storyboard.
10. Select "GLKView"

Step 6 is anyway needed to make sure FsLazyWindowViewController is linked.

This alternative steps unnecessitate ViewController.h and ViewController.m in the default application template.





Pitfalls

- fsguifiledialog.cpp was accidentally dropped to fsguilib.xcodeproj, then it caused compile error (obviously).  Foolishly xcode didn't stop building and went on to linking, and caused massive link error, which is extremely confusing.  How stupid?  I needed to look through all linker errors before finding one compile error.  The problem was that compiling stopped in the middle.  If I see unreasonable link errors, I should be aware that it may be compile error somewhere.


- If, libraries of the depending xcode projects do not appear in the Build Phase->Link Libraries, I just may have to quit (not just close project. really Quit) XCode and start again.  Xcode is very crappy about handling this kind of depending projects.


- Some textbooks were assuming GL_RGBA is defined as 4.  Looks like I was using that definition from somewhere.  But, it is not the case in OpenGL ES.  Be careful about the 3rd parameter to glTexImage2D.
