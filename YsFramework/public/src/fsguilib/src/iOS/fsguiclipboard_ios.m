#import <UIKit/UIKit.h>

int FsGuiCopyStringToClipBoardC(const char cstr[])
{
	UIPasteboard *pasteboard=[UIPasteboard generalPasteboard];
    pasteboard.string=[[NSString alloc] initWithUTF8String:cstr];
	return 1;
}


int FsGuiGetStringLengthInClipBoardC(void)
{
	UIPasteboard *pasteboard=[UIPasteboard generalPasteboard];

    NSString *str=pasteboard.string;
    if(nil!=str)
    {
        return (int)strlen([str UTF8String]);
    }
    return 0;
}

int FsGuiCopyStringFromClipBoardC(char cstr[])
{
    UIPasteboard *pasteboard=[UIPasteboard generalPasteboard];
    
    NSString *str=pasteboard.string;
    if(nil!=str)
    {
        strcpy(cstr,[str UTF8String]);
        return 1;
    }
    return 0;
}
