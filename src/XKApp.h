#ifndef _XKAPP_H_
#define _XKAPP_H_

#include <Application.h>
#include <Resources.h>
#include "mainWindow.h"

//#define MAINWINDOW_POSITION	1001

class XKApp : public BApplication {
	public:
		XKApp();
		~XKApp(void);
	
	virtual void ReadyToRun(void);
	virtual bool QuitRequested(void);
	virtual void MessageReceived(BMessage *message);
	virtual void RefsReceived(BMessage *message);
	virtual void AboutRequested(void);
	
	mainWindow	*mainW;
//	BResources	*appRES;
};


#endif