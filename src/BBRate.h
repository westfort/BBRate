/*
 * BBRate.h
 * Version 1.0
 * Created by West Fort on 5/26/2014
 */

#ifndef BBRATE_H_
#define BBRATE_H_

#include <QObject>
#include <bb/system/SystemDialog>
#include <QDate>

class BBRate : public QObject
{
Q_OBJECT

    Q_PROPERTY(int appWorldID READ appWorldID WRITE setAppWorldID)
    Q_PROPERTY(QString appName READ appName WRITE setAppName)
    Q_PROPERTY(QString appVersion READ appVersion WRITE setAppVersion)
    Q_PROPERTY(int usesUntilPrompt READ usesUntilPrompt WRITE setUsesUntilPrompt)
    Q_PROPERTY(int daysUntilPrompt READ daysUntilPrompt WRITE setDaysUntilPrompt)
    Q_PROPERTY(int remindAfterUses READ remindAfterUses WRITE setRemindAfterUses)
    Q_PROPERTY(int remindAfterDays READ remindAfterDays WRITE setRemindAfterDays)
    Q_PROPERTY(bool promptForNewVersionIfRated READ promptForNewVersionIfRated WRITE setPromptForNewVersionIfRated)
    Q_PROPERTY(bool promptForNewVersionIfDeclined READ promptForNewVersionIfDeclined WRITE setPromptForNewVersionIfDeclined)
    Q_PROPERTY(bool test READ test WRITE setTest)
    Q_PROPERTY(QString promptTitle READ promptTitle WRITE setPromptTitle)
    Q_PROPERTY(QString promptMessage READ promptMessage WRITE setPromptMessage)
    Q_PROPERTY(QString cancelButtonText READ cancelButtonText WRITE setCancelButtonText)
    Q_PROPERTY(QString rateButtonText READ rateButtonText WRITE setRateButtonText)
    Q_PROPERTY(QString remindButtonText READ remindButtonText WRITE setRemindButtonText)


public:
    BBRate();
    virtual ~BBRate();

    int appWorldID();
    QString appName();
    QString appVersion();
    int usesUntilPrompt();
    int daysUntilPrompt();
    int remindAfterUses();
    int remindAfterDays();
    bool promptForNewVersionIfRated();
    bool promptForNewVersionIfDeclined();
    bool test();
    QString promptTitle();
    QString promptMessage();
    QString cancelButtonText();
    QString rateButtonText();
    QString remindButtonText();

public slots:
    void setAppWorldID(int id);
    void setAppName(QString appName);
    void setAppVersion(QString appVersion);
    void setUsesUntilPrompt(int usesUntilPrompt);
    void setDaysUntilPrompt(int daysUntilPrompt);
    void setRemindAfterUses(int remindAfterUses);
    void setRemindAfterDays(int remindAfterDays);
    void setPromptForNewVersionIfRated(bool promptForNewVersionIfRated);
    void setPromptForNewVersionIfDeclined(bool promptForNewVersionIfDeclined);
    void setTest(bool test);
    void setPromptTitle(QString promptTitle);
    void setPromptMessage(QString promptMessage);
    void setCancelButtonText(QString cancelButtonText);
    void setRateButtonText(QString rateButtonText);
    void setRemindButtonText(QString remindButtonText);

   void onDialogFinished(bb::system::SystemUiResult::Type type);
   void onAwake();
   void onAsleep();

signals:
    void rated();
    void declined();
    void remind();

private:
    // Required
    int m_AppWorldID;

    // All variables below are optional
    // App details - Automatically set
    QString m_AppName;
    QString m_AppVersion;

    // Usage settings
    int m_UsesUntilPrompt;
    int m_DaysUntilPrompt;
    int m_RemindAfterUses;
    int m_RemindAfterDays;
    bool m_PromptForNewVersionIfRated;
    bool m_PromptForNewVersionIfDeclined;
    bool m_Test;

    // Prompt settings
    QString m_PromptTitle;
    QString m_PromptMessage;
    QString m_CancelButtonText;
    QString m_RateButtonText;
    QString m_RemindMeButtonText;
    // End of user settings

    QString m_VersionRated;
    QString m_VersionDeclined;

    QDate m_LaunchDate;
    QDate m_LastRemindDate;

    bool m_Declined;
    bool m_Rated;
    bool m_RemindLater;

    int m_LaunchCount;

    bool m_DisplayingPrompt;

    bb::system::SystemDialog *m_Dialog;

    QVariant getValueFor(const QString &objectName, const QVariant &defaultValue);
    void saveValueFor(const QString &objectName, const QVariant &inputValue);

    bool shouldShowDialog();
    void showDialog();

    void rate();
    void remindLater();
    void declineVersion();
};

#endif /* BBRATE_H_ */
