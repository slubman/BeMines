#include "NewScoreWindow.h"
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <Screen.h>
#include <StringView.h>
#include "Globals.h"

enum
{
	M_CHANGE_NAME = 'chgn'
};

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "NewScoreWindow"

NewScoreWindow::NewScoreWindow(uint16 time, int32 difficulty)
	:	DWindow(BRect(0,0,300,300), B_TRANSLATE("Best Score"), B_TITLED_WINDOW,
				B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE
				| B_AUTO_UPDATE_SIZE_LIMITS),
		fDifficulty(difficulty),
		fSeconds(time)
{
	BView *top = GetBackgroundView();

	fTextBox = new BTextControl("scorename","",gBestTimes[difficulty].name.String(), NULL);
	fTextBox->MakeFocus(true);
	fTextBox->TextView()->SelectAll();
	fTextBox->SetModificationMessage(new BMessage(M_CHANGE_NAME));

	BLayoutBuilder::Group<>(top, B_VERTICAL, B_USE_SMALL_INSETS)
		.SetInsets(B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS,
			B_USE_WINDOW_INSETS, B_USE_WINDOW_INSETS)
		.Add(new BStringView("label1", B_TRANSLATE_COMMENT("You have made a new best time!", "First line text new record")))
		.Add(new BStringView("lable2", B_TRANSLATE_COMMENT("Please enter your name.", "Second line text new record")))
		.Add(fTextBox)
		.AddGroup(B_HORIZONTAL)
			.AddGlue()
			.Add(fClose = new BButton("close", B_TRANSLATE("Close"),
				new BMessage(B_QUIT_REQUESTED)));

	fClose->MakeDefault(true);

	MakeCenteredOnShow(true);
}


bool
NewScoreWindow::QuitRequested(void)
{
	BString name = fTextBox->Text();

	gBestTimes[fDifficulty].time = fSeconds;
	gBestTimes[fDifficulty].name = name;

	return true;
}


void
NewScoreWindow::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case M_CHANGE_NAME:
		{
			BString name = fTextBox->Text();
			if (name.CountChars() < 1)
				fClose->SetEnabled(false);
			else
				fClose->SetEnabled(true);
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

