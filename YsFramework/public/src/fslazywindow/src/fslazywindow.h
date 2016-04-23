#ifndef FSLAZYWINDOW_IS_INCLUDED
#define FSLAZYWINDOW_IS_INCLUDED
/* { */

#include <fssimplewindow.h>


#ifdef __cplusplus

/*! 
*/
class FsLazyWindowApplicationBase
{
protected:
	bool terminateFlag;

public:
	/*! This function return a pointer to a sub-class of FsLazyWindowApplicationBase.

	    This function will be called from the lazy-window framework's event handler many times.
	    Therefore, do not re-allocate FsLazyWindowApplicationBase every time.
	*/
	static FsLazyWindowApplicationBase *GetApplication(void);


	/*! This function sets must-terminate flag. 
	*/
	void SetMustTerminate(bool MustTerminate);

	/*! Default constructor.  It sets terminateFlag=false.
	*/
	FsLazyWindowApplicationBase();

	/*! This function is called before the window is created.  If something needs to be done before opening a window
	    (for example, deciding window size from the command parameters) it is the right place to do.

	    Argc and argv are taken from the command parameters.
	*/
	virtual void BeforeEverything(int argc,char *argv[])=0;

	/*! This function must be implemented in the lazy-window application.
	    It should return preferred window location and size, title, etc., in OPT,
	    which may or may not be respected by the lazy-window framework.
	*/
	virtual void GetOpenWindowOption(FsOpenWindowOption &OPT) const=0;

	/*! This function must be implemented in the lazy-window application.
        This function will be called once when the window and OpenGL context is ready.

	    Argc and argv are taken from the command parameters.
	*/
	virtual void Initialize(int argc,char *argv[])=0;

	/*! This function is called regularly on interval.
	*/
	virtual void Interval(void)=0;

	/*! This function is called before the program terminates.
	*/
	virtual void BeforeTerminate(void)=0;

	/*! This function is called when the window needs to be re-drawn. */
	virtual void Draw(void)=0;

	/*! This function is called when the user wants to close the program.
	    For example, when the user clicks on the close button of the window, or
	    pressed Alt+F4 in Windows or Command+Q in Mac OSX. 

	    The default implementation just closes the program by returning true.

	    Return true if the window can be closed.

	    If the window must not be closed right away (for example, need to confirm with the user if the window can really be closed),
	    set up whatever user-inquirey and return false in this function.
	*/
	virtual bool UserWantToCloseProgram(void);

	/*! This function must return true when the program should terminate.
	    The default behavior is to return terminateFlag.
	*/
	virtual bool MustTerminate(void) const;

	/*! This function must return minimum sleep per interval in milli seconds. 
	    It returns 10 by default. */
	virtual long long int GetMinimumSleepPerInterval(void) const;

	/*! This function must return true when the window must be re-drawn.
	*/
	virtual bool NeedRedraw(void) const=0;
};

#endif

/* } */
#endif
