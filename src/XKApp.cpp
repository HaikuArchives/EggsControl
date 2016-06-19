#include "XKApp.h"
#include <Roster.h>
#include <Path.h>
#include <Entry.h>
#include "AboutWindow.h"
#include <Alert.h>
#include <TextView.h>

XKApp::XKApp() : BApplication("application/x-EggsControl"){
//	app_info	ai;
//	GetAppInfo(&ai);
//	appRES=new BResources(new BFile(&ai.ref, B_READ_WRITE));

	mainW=new mainWindow();
	if (mainW->Lock()) {
//		if (appRES->HasResource(B_POINT_TYPE,MAINWINDOW_POSITION)) {
//			BPoint	*point=(BPoint *)appRES->LoadResource(B_POINT_TYPE, MAINWINDOW_POSITION, NULL);
//			//if (point!=NULL)
//			mainW->MoveTo(*point);
//		}
		mainW->Show();
		mainW->Unlock();
	}
}

XKApp::~XKApp(void) {
	delete mainW;
}

void XKApp::ReadyToRun(void) {
}

bool XKApp::QuitRequested(void) {
//	BPoint	point=(mainW->Frame()).LeftTop();
//	// save main window position in resources of application file
//	if (appRES->HasResource(B_POINT_TYPE,MAINWINDOW_POSITION))
//		appRES->RemoveResource(B_POINT_TYPE,MAINWINDOW_POSITION);
//	appRES->AddResource(B_POINT_TYPE,MAINWINDOW_POSITION,(void *) (&point), sizeof(point) );
	
	while (CountWindows()>0) {WindowAt(0)->Lock();WindowAt(0)->Quit();}
	return true;
}

void XKApp::MessageReceived(BMessage *msg) {
	switch(msg->what) {
	
		// file dropped on mainWaindow
		case B_SIMPLE_DATA: {
		uint32 		type;
		int32 		count;
		BEntry		*entry=new BEntry();
		entry_ref	ref;
		BPath		path;


		msg->GetInfo("refs", &type, &count);
		if (type != B_REF_TYPE) {delete entry; break;}

		for (int32 i = count-1; i >=0; i--) {
			if (msg->FindRef("refs", i, &ref) == B_OK)
				entry->SetTo(&ref,true);

	   		   if (entry->GetPath(&path)==B_OK)
	   		   	if (mainW->Lock()) {
	   		   		mainW->AddSoundFile(new BEntry(path.Path(),true));
	   		   		mainW->Unlock();
	   		   	}
		}
		   path.Unset();
		   entry->Unset();
		delete entry;
		break;
		}



		default: {
			BApplication::MessageReceived(msg);
			break;
		}
	}
}

void XKApp::RefsReceived(BMessage *msg) {
}

void XKApp::AboutRequested(void) {
	AboutWindow	*aw=new AboutWindow("About");
	aw->SetApplicationName("EggsControl");
	aw->SetVersionNumber("1.3");
	aw->SetCopyrightString(B_UTF8_COPYRIGHT"2002 by Maurice Michalski");
	aw->SetText("Plays many samples at the same time and loops them seamlessly.");
	aw->Show();

}