#include "BBRate.h"
#include <bb/ApplicationInfo>
#include <bb/system/InvokeManager>
#include <QSettings>

using namespace bb::system;

BBRate::BBRate()
{
    m_AppWorldID = -1;

    bb::ApplicationInfo appInfo;
    m_AppName = appInfo.title();
    m_AppVersion = appInfo.version();

    m_UsesUntilPrompt = 5;
    m_DaysUntilPrompt = 3;
    m_RemindAfterUses = 5;
    m_RemindAfterDays = 3;
    m_PromptForNewVersionIfRated = false;
    m_PromptForNewVersionIfDeclined = false;
    m_Test = false;

    m_PromptTitle = "Rate " + m_AppName;
    m_PromptMessage = "If you enjoy using " + m_AppName + ", would you mind taking a moment to rate it? It won't take more than a minute. Thanks for your support!";
    m_CancelButtonText = "No, Thanks";
    m_RateButtonText = "Rate It Now";
    m_RemindMeButtonText = "Remind Me Later";

    m_Rated = getValueFor("rated", false).toBool();
    m_Declined = getValueFor("declined", false).toBool();
    m_RemindLater = getValueFor("remindLater", false).toBool();
    m_VersionRated = getValueFor("versionRated", m_AppVersion).toString();

    m_LaunchDate = getValueFor("launchDate", QDate::currentDate()).toDate();
    if(m_LaunchDate == QDate::currentDate())
        saveValueFor("launchDate", m_LaunchDate);

    m_LaunchCount = getValueFor("launchCount", 0).toInt();

    m_DisplayingPrompt = false;
}

BBRate::~BBRate()
{

}

bool BBRate::shouldShowDialog() {
    if(m_Test == true) {
        return true;
    }
    else if(m_AppWorldID == -1) {
        qDebug()<<"[ERROR][BBRate]: Your AppWorld ID isn't set.";
        return false;
    }
    else if(m_Rated == true || m_Declined == true) {
        // already rated / declined
        return false;
    }
    else if(m_LaunchCount >= m_UsesUntilPrompt && m_RemindLater == false) {
        if(m_LaunchDate.daysTo(QDate::currentDate()) >= m_DaysUntilPrompt) {
            return true;
        }
    }
    else if(m_RemindLater == true) {
        QDate today = QDate::currentDate();

        int days = m_LastRemindDate.daysTo(today);

        if(days >= m_RemindAfterDays) {
            if(m_LaunchCount >= m_RemindAfterUses) {
                return true;
            }
        }
    }
    return false;
}

void BBRate::showDialog() {
    m_Dialog = new SystemDialog(m_RemindMeButtonText, m_RateButtonText, m_CancelButtonText);
    m_Dialog->setTitle(m_PromptTitle);
    m_Dialog->setBody(m_PromptMessage);
    m_Dialog->setButtonAreaLimit(1);
    bool success = connect(m_Dialog, SIGNAL(finished(bb::system::SystemUiResult::Type)), this, SLOT(onDialogFinished(bb::system::SystemUiResult::Type)));
    if(success) {
        m_DisplayingPrompt = true;
        m_Dialog->show();
    }
    else {
        qDebug()<<"[BBRate][Error]: Unable to connect to system dialog signal.";
        m_Dialog->deleteLater();
    }
}

void BBRate::onDialogFinished(bb::system::SystemUiResult::Type type) {
    switch(type) {
        case SystemUiResult::ConfirmButtonSelection:
            remindLater();
            break;
        case SystemUiResult::CancelButtonSelection:
            declineVersion();
            break;
        case SystemUiResult::CustomButtonSelection:
            rate();
            break;
        default:
            qDebug()<<"[BBRate][Error]: Unknown item selected";
    }

    m_DisplayingPrompt = false;

}

void BBRate::rate () {
    m_Rated = true;
    m_VersionRated = m_AppVersion;
    saveValueFor("rated", m_Rated);
    saveValueFor("versionRated", m_VersionRated);
    m_Dialog->deleteLater();
    emit rated();

    InvokeManager invokeManager;
    InvokeRequest request;
    request.setAction("bb.action.OPEN");
    request.setTarget("sys.appworld");
    request.setUri("appworld://content/" + QString::number(m_AppWorldID));
    invokeManager.invoke(request);
}

void BBRate::remindLater() {
    m_RemindLater = true;
    m_LaunchCount = 0;
    m_LastRemindDate = QDate::currentDate();
    saveValueFor("launchCount", 0);
    saveValueFor("remindLater", true);
    saveValueFor("remindDate", QDate::currentDate());
    m_Dialog->deleteLater();
    emit remind();
}

void BBRate::declineVersion() {
    m_Declined = true;
    m_VersionRated = m_AppVersion;
    saveValueFor("versionRated", m_VersionRated);
    saveValueFor("declined", m_Declined);
    saveValueFor("launchCount", 0);
    m_Dialog->deleteLater();
    emit declined();
}

void BBRate::onAwake() {
    if(m_VersionRated != m_AppVersion) {
        if(m_PromptForNewVersionIfRated == true) {
            m_Rated = false;
            saveValueFor("rated", m_Rated);
        }

        if(m_PromptForNewVersionIfDeclined == true) {
            m_Declined = false;
            saveValueFor("declined", m_Declined);
        }
        m_VersionRated = m_AppVersion;
        saveValueFor("versionRated", m_VersionRated);
        m_LaunchCount = 0;
    }

    m_LaunchCount = m_LaunchCount + 1;

    if(shouldShowDialog() && m_DisplayingPrompt == false)
        showDialog();
}

void BBRate::onAsleep() {
    if(m_Rated == false || m_Declined == false)
        saveValueFor("launchCount", m_LaunchCount);
}


int BBRate::appWorldID() {
    return m_AppWorldID;
}

void BBRate::setAppWorldID(int appWorldID) {
    m_AppWorldID = appWorldID;
}

QString BBRate::appName() {
    return m_AppName;
}

void BBRate::setAppName(QString appName) {
    m_PromptTitle.replace(m_AppName, appName);
    m_PromptMessage.replace(m_AppName, appName);
    m_AppName = appName;
}

QString BBRate::appVersion() {
    return m_AppVersion;
}

void BBRate::setAppVersion(QString appVersion) {
    m_AppVersion = appVersion;
}

int BBRate::usesUntilPrompt() {
    return m_UsesUntilPrompt;
}

void BBRate::setUsesUntilPrompt(int usesUntilPrompt) {
    m_UsesUntilPrompt = usesUntilPrompt;
}

int BBRate::daysUntilPrompt() {
    return m_DaysUntilPrompt;
}

void BBRate::setDaysUntilPrompt(int daysUntilPrompt) {
    m_DaysUntilPrompt = daysUntilPrompt;
}

int BBRate::remindAfterUses() {
    return m_RemindAfterUses;
}

void BBRate::setRemindAfterUses(int remindAfterUses) {
    m_RemindAfterUses = remindAfterUses;
}

int BBRate::remindAfterDays() {
    return m_RemindAfterDays;
}

void BBRate::setRemindAfterDays(int remindAfterDays) {
    m_RemindAfterDays = remindAfterDays;
}

bool BBRate::promptForNewVersionIfRated() {
    return m_PromptForNewVersionIfRated;
}

void BBRate::setPromptForNewVersionIfRated(bool promptForNewVersionIfRated) {
    m_PromptForNewVersionIfRated = promptForNewVersionIfRated;
}

bool BBRate::promptForNewVersionIfDeclined() {
    return m_PromptForNewVersionIfDeclined;
}

void BBRate::setPromptForNewVersionIfDeclined(bool promptForNewVersionIfDeclined) {
    m_PromptForNewVersionIfDeclined = promptForNewVersionIfDeclined;
}

bool BBRate::test() {
    return m_Test;
}

void BBRate::setTest(bool test) {
    m_Test = test;
}

QString BBRate::promptTitle() {
    return m_PromptTitle;
}

void BBRate::setPromptTitle(QString promptTitle) {
    m_PromptTitle = promptTitle;
}

QString BBRate::promptMessage() {
    return m_PromptMessage;
}

void BBRate::setPromptMessage(QString promptMessage) {
    m_PromptMessage = promptMessage;
}

QString BBRate::cancelButtonText() {
    return m_CancelButtonText;
}

void BBRate::setCancelButtonText(QString cancelButtonText) {
    m_CancelButtonText = cancelButtonText;
}

QString BBRate::rateButtonText() {
    return m_RateButtonText;
}

void BBRate::setRateButtonText(QString rateButtonText) {
    m_RateButtonText = rateButtonText;
}

QString BBRate::remindButtonText() {
    return m_RemindMeButtonText;
}

void BBRate::setRemindButtonText(QString remindButtonText) {
    m_RemindMeButtonText = remindButtonText;
}

QVariant BBRate::getValueFor(const QString &objectName, const QVariant &defaultValue) {
    QSettings settings;

    // If no value has been saved, return the default value.
    if (settings.value(objectName).isNull()) {
        return defaultValue;
    }

    // Otherwise, return the value stored in the settings object.
    return settings.value(objectName);
}

void BBRate::saveValueFor(const QString &objectName, const QVariant &inputValue) {
    QSettings settings;
    settings.setValue(objectName, QVariant(inputValue));
}
