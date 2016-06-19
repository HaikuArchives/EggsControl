#include <Application.h>
#include "mainWindow.h"
#include "SoundPanel.h"
#include <StringView.h>
#include <stdio.h>

mainWindow::mainWindow() : BWindow(BRect(0,0,150,70), "EggsControl 1.3",B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS) {
	MoveTo(200,100);

	mainMB=new BMenuBar(Bounds(), "mainMB");
	mainMB->SetTargetForItems(be_app_messenger);
	float x1;
	mainMB->GetPreferredSize(&x1,&menuheight);
	AddChild(mainMB);
	fileM=new BMenu("File");
	fileM->SetTargetForItems(be_app_messenger);
	mainMB->AddItem(fileM);
	controlM=new BMenu("Control");
	controlM->SetTargetForItems(be_app_messenger);
	stop_allMI=new BMenuItem("Stop All", new BMessage(SP_STOPALL), 'S');
	stop_allMI->SetEnabled(false);
	controlM->AddItem(stop_allMI);
	start_allMI=new BMenuItem("Play All", new BMessage(SP_STARTALL), 'P');
	start_allMI->SetEnabled(false);
	controlM->AddItem(start_allMI);
	mainMB->AddItem(controlM);
	aboutMI=new BMenuItem("About EggsControl...", new BMessage(B_ABOUT_REQUESTED));
	aboutMI->SetTarget(be_app_messenger);
	fileM->AddItem(aboutMI);
	fileM->AddSeparatorItem();
	exitMI=new BMenuItem("Exit", new BMessage(B_QUIT_REQUESTED));
	exitMI->SetTarget(be_app_messenger);
	fileM->AddItem(exitMI);
	
	ResizeTo(152, 72+menuheight+1);

	panelView=new BView(BRect(Bounds().left, Bounds().top+menuheight+1, Bounds().right, Bounds().bottom+menuheight+1), "SoundSampleView", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	panelView->SetViewColor((rgb_color){216,216,216,255});
	dropSV=new BStringView(BRect(0,0,150,42),"dropSV", "drop some sound files here!");
	dropSV->SetAlignment(B_ALIGN_CENTER);
	panelView->AddChild(dropSV);
	AddChild(panelView);
	// number of sound panels
	iPanels=0;
}

mainWindow::~mainWindow() {
	panelView->RemoveSelf();
	dropSV->RemoveSelf();
	mainMB->RemoveSelf();
	delete panelView;
	delete mainMB;
	delete dropSV;
}

bool mainWindow::QuitRequested(void) {
	be_app->PostMessage(B_QUIT_REQUESTED);
	return false;
}


void mainWindow::MessageReceived(BMessage *msg) {
	switch (msg->what) {
	
		case SP_PLAY: {
			void *ptr;
			msg->FindPointer("from", &ptr);
			((SoundPanel *)ptr)->Play();
			break;
		}

		case SP_STOP: {
			void *ptr;
			msg->FindPointer("from", &ptr);
			((SoundPanel *)ptr)->Stop();
			break;
		}

		case SP_STOPALL: {
			SoundPanel *ptr;

			for (int32 i=0; i<panelView->CountChildren(); i++) {
				ptr=(SoundPanel *)panelView->ChildAt(i);
				((SoundPanel *)ptr)->StopAll();
			}
			break;
		}

		case SP_STARTALL: {
			SoundPanel *ptr;

			for (int32 i=0; i<panelView->CountChildren(); i++) {
				ptr=(SoundPanel *)panelView->ChildAt(i);
				((SoundPanel *)ptr)->Play();
			}
			break;
		}

		case SP_UPDATEVOLUME: {
			void *ptr;
			msg->FindPointer("from", &ptr);
			((SoundPanel *)ptr)->UpdateVolume();
			break;
		}

		case SP_PAUSE: {
			void *ptr;
			msg->FindPointer("from", &ptr);
			((SoundPanel *)ptr)->Pause();
			break;
		}
		
		case SP_LOOP: {
			void *ptr;
			msg->FindPointer("from", &ptr);
			((SoundPanel *)ptr)->ToggleLoop();
			break;
		}

		case SP_DELETE: {
			void *ptr;
			msg->FindPointer("from", &ptr);
			((SoundPanel *)ptr)->StopAll();
			RemoveSoundFile((SoundPanel *)ptr);
			break;
		}

		case B_SIMPLE_DATA: {		
			be_app->PostMessage(msg);
			break;
		}

		default: {
			BWindow::MessageReceived(msg);
			break;
		}	
	}
}



bool mainWindow::AddSoundFile(BEntry *entry) {
	SoundPanel	*sp=new SoundPanel(entry);
	float		width=(sp->Bounds()).Width()+2;
	float		height=(sp->Bounds()).Height()+2;
	float		x,y;
	
	if (iPanels==0)
		panelView->RemoveChild(dropSV);
	stop_allMI->SetEnabled(true);
	start_allMI->SetEnabled(true);
	y=(iPanels)/5;
	x=(iPanels)%5;
	sp->MoveTo(x*width+1,y*height+1);
	panelView->AddChild(sp);
	iPanels++;
	if (y>0) x=4;
	ResizeTo(x*width+width, y*height+height+menuheight+1);
	panelView->ResizeTo(x*width+width, y*height+height);
	panelView->MoveTo(0,menuheight+1);

	return true;
}

bool mainWindow::RemoveSoundFile(SoundPanel *panel) {
	iPanels--;
	panelView->RemoveChild(panel);
	delete panel;
	ReorderPanels();
	return true;
}

void mainWindow::ReorderPanels() {
	float		width=0;
	float		height=0;
	float		x=0,y=0;

	if (iPanels>0) {
		for (int32 i=0; i<panelView->CountChildren(); i++) {
			SoundPanel	*sp=(SoundPanel *)panelView->ChildAt(i);

			width=(sp->Bounds()).Width()+2;
			height=(sp->Bounds()).Height()+2;
			
			y=(i)/5;
			x=(i)%5;
			sp->MoveTo(x*width+1,y*height+1);
		}
		if (y>0) x=4;
		ResizeTo(x*width+width, y*height+height+menuheight+1);
		panelView->ResizeTo(x*width+width, y*height+height);
		panelView->MoveTo(0,menuheight+1);
	} else
		if (iPanels==0) {
			panelView->AddChild(dropSV);
			stop_allMI->SetEnabled(false);
			start_allMI->SetEnabled(true);
		}


}

