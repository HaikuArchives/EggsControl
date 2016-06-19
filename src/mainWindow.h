#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <Window.h>
#include <View.h>
#include "SoundPanel.h"
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <StringView.h>

class mainWindow : public BWindow {
	public:
		mainWindow();
		~mainWindow();

		virtual bool QuitRequested(void);
		virtual void MessageReceived(BMessage *msg);
		bool AddSoundFile(BEntry *entry);
		bool RemoveSoundFile(SoundPanel *panel);
		void ReorderPanels();

	private:
		BView	*panelView;
		int32	iPanels;
		BMenuBar	*mainMB;
		float	menuheight;
		BStringView	*dropSV;
		BMenu		*fileM, *controlM;
		BMenuItem	*aboutMI, *exitMI,
				*stop_allMI, *start_allMI;
};

#endif
