BBRate
====
BBRate is a library designed to help you promote your BlackBerry 10 App by prompting users to rate your app.

Supported API Levels
====
BlackBerry 10.0.0+

Installation
====
To install BBRate, drag BBRate.h and BBRate.cpp files into your project. Inside your app.pro file include:

	LIBS += -lbb
	LIBS += -lbbsystem

###C++
----
Add the following code to your main.cpp:

	#include "BBRate.h"

	BBRate bbRate;
	QObject::connect(&app, SIGNAL(awake()), &bbRate, SLOT(onAwake()));
	QObject::connect(&app, SIGNAL(asleep()), &bbRate, SLOT(onAsleep()));

###QML
----
Include BBRate into your Application's UI class. By default BlackBerry names this ApplicationUI.cpp.

	#include "BBRate.h"

Inside your ApplicationUI constructor register BBRate so it is accessible through QML.

	qmlRegisterType<BBRate>("bb.rate", 1, 0, "BBRate");

Inside your main.qml, import BBRate and set it as an attached object and register the application's onAwake and onAsleep signals.

	import bb.cascades 1.0
	import bb.rate 1.0

	Page {
	    Container {
	        
	    }
	    attachedObjects: [
	        BBRate {
	            id: bbRate
	            appWorldID: 1234
	        }
	    }
	    onCreationCompleted: {
	        Application.asleep.connect(bbRate.onAsleep)
	        Application.awake.connect(bbRate.onAwake)
	    }
	}


		
Configuration
====
###Required
----
Setting your BlackBerry World AppID (Your AppID can be found in the BlackBerry App World Vendor Portal). This is the only required configuration.

	setAppWorldID(string);

###Optional
---
All the items below are set using a default setting or by grabbing your application's information from the bar-descriptor.

Application's name. 
Default: Application name from bar-descriptor.

	setAppName(string);

Application's version. 
Default: Application version from bar-descriptor.

	setAppVersion(string);

Uses until prompt is displayed. 
Default: 5.

	setUsesUntilPrompt(int);
	
Days until prompt is displayed. 
Default: 3.

	setDaysUntilPrompt(int);

Uses until prompt is displayed again once the user asks you to remind them later. 
Default: 5.

	setRemindAfterUses(int);

Days until prompt is displayed again once the user asks you to remind them later. 
Default: 3.

	setRemindAfterDays(int);
	
Prompt the user to rate your application again once they've updated to a new version and have already rated your application. 
Default: false.

	setPromptForNewVersionIfRated(bool);
	
Prompt the user to rate your application again once they've updated to a new version and have already declined to rate your application. 
Default: false.

	setPromptForNewVersionIfDeclined(bool);
	
Always show your prompt for testing purposes. 
Default: false.
	
	setTest(bool);

The prompt title.
Default: Rate {AppName}.

	setPromptTitle(string);
	
The prompt message. 
Default: If you enjoy using {AppName}, would you mind taking a moment to rate it? It won't take more than a minute. Thanks for your support!

	setPromptTitle(string);

The cancel button text. 
Default: No, Thanks.
	
	setCancelButtonText(string)
	
The rate button text. 
Default: Rate It Now.

	setRateButtonText(string)

The remind me button text. 
Default: Remind Me Later.

	setRemindButtonText(string)
	
Signals
---
If you wish do run additional code after the user selects an option, you can connect to BBRate's signals. 

###C++
----
In your main.cpp add the following lines.

	QObject::connect(&bbRate, SIGNAL(rated()), &appui, SLOT(onRated()));
	QObject::connect(&bbRate, SIGNAL(declined()), &appui, SLOT(onDeclined()));
	QObject::connect(&bbRate, SIGNAL(remind()), &appui, SLOT(onRemind()));

###QML
----
In your main.qml add the following signals to your BBRate object.

    BBRate {
        id: bbRate
        appWorldID: 1234
        onDeclined: {
            
        }
        onRated: {
            
        }
        onRemind: {
            
        }
    }