#import <UIKit/UIKit.h>

void FsOpenURLC(const char urlStr[])
{
	printf("Open Web Site> %s\n",urlStr);

	NSURL *url;
	NSString *urlString;

	urlString=[NSString stringWithUTF8String:urlStr];
	url=[NSURL URLWithString:urlString];

    [[UIApplication sharedApplication] openURL:url];
}

