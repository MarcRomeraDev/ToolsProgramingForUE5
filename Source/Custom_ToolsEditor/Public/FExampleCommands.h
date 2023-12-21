#pragma once

#include "Framework/Commands/Commands.h"
#include "Misc/MessageDialog.h"
#include "Styling/SlateStyle.h"

#define IMPLEMENT_COMMAND(CommandName) \
TSharedPtr<FUICommandInfo> CommandInfo_##CommandName; \
static void Command_##CommandName()

class FExampleCommands : public TCommands<FExampleCommands>
{
public:
	
	FExampleCommands() : TCommands(
	TEXT("CommandsExample"),
	NSLOCTEXT("ToolsNamespace","ToolDesc","This is the tool description"),
	NAME_None,
	NAME_None
	){}

	virtual void RegisterCommands() override;

	IMPLEMENT_COMMAND(ShowMsg)
	{
		FMessageDialog::Open(EAppMsgType::YesNo, NSLOCTEXT("ToolsNamespace","PopupMsg", "Hey world!"));
	};
	IMPLEMENT_COMMAND(DeleteAssets)
	{
		//DeleteAssets
	};
};
