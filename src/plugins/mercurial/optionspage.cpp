/**************************************************************************
**
** Copyright (c) 2012 Brian McGillion
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "optionspage.h"
#include "mercurialsettings.h"
#include "mercurialplugin.h"

#include <coreplugin/icore.h>
#include <utils/pathchooser.h>
#include <vcsbase/vcsbaseconstants.h>

#include <QTextStream>

using namespace Mercurial::Internal;
using namespace Mercurial;

OptionsPageWidget::OptionsPageWidget(QWidget *parent) :
        QWidget(parent)
{
    m_ui.setupUi(this);
    m_ui.commandChooser->setExpectedKind(Utils::PathChooser::ExistingCommand);
    m_ui.commandChooser->setPromptDialogTitle(tr("Mercurial Command"));
}

MercurialSettings OptionsPageWidget::settings() const
{
    MercurialSettings s = MercurialPlugin::instance()->settings();
    s.setValue(MercurialSettings::binaryPathKey, m_ui.commandChooser->rawPath());
    s.setValue(MercurialSettings::userNameKey, m_ui.defaultUsernameLineEdit->text().trimmed());
    s.setValue(MercurialSettings::userEmailKey, m_ui.defaultEmailLineEdit->text().trimmed());
    s.setValue(MercurialSettings::logCountKey, m_ui.logEntriesCount->value());
    s.setValue(MercurialSettings::timeoutKey, m_ui.timeout->value());
    s.setValue(MercurialSettings::promptOnSubmitKey, m_ui.promptOnSubmitCheckBox->isChecked());
    return s;
}

void OptionsPageWidget::setSettings(const MercurialSettings &s)
{
    m_ui.commandChooser->setPath(s.stringValue(MercurialSettings::binaryPathKey));
    m_ui.defaultUsernameLineEdit->setText(s.stringValue(MercurialSettings::userNameKey));
    m_ui.defaultEmailLineEdit->setText(s.stringValue(MercurialSettings::userEmailKey));
    m_ui.logEntriesCount->setValue(s.intValue(MercurialSettings::logCountKey));
    m_ui.timeout->setValue(s.intValue(MercurialSettings::timeoutKey));
    m_ui.promptOnSubmitCheckBox->setChecked(s.boolValue(MercurialSettings::promptOnSubmitKey));
}

QString OptionsPageWidget::searchKeywords() const
{
    QString rc;
    QLatin1Char sep(' ');
    QTextStream(&rc)
            << sep << m_ui.configGroupBox->title()
            << sep << m_ui.mercurialCommandLabel->text()
            << sep << m_ui.userGroupBox->title()
            << sep << m_ui.defaultUsernameLabel->text()
            << sep << m_ui.defaultEmailLabel->text()
            << sep << m_ui.miscGroupBox->title()
            << sep << m_ui.showLogEntriesLabel->text()
            << sep << m_ui.timeoutSecondsLabel->text()
            << sep << m_ui.promptOnSubmitCheckBox->text()
               ;
    rc.remove(QLatin1Char('&'));
    return rc;
}

OptionsPage::OptionsPage()
{
    setId(QLatin1String(VcsBase::Constants::VCS_ID_MERCURIAL));
    setDisplayName(tr("Mercurial"));
}

QWidget *OptionsPage::createPage(QWidget *parent)
{
    if (!optionsPageWidget)
        optionsPageWidget = new OptionsPageWidget(parent);
    optionsPageWidget->setSettings(MercurialPlugin::instance()->settings());
    if (m_searchKeywords.isEmpty())
        m_searchKeywords = optionsPageWidget->searchKeywords();
    return optionsPageWidget;
}

void OptionsPage::apply()
{
    if (!optionsPageWidget)
        return;
    MercurialPlugin *plugin = MercurialPlugin::instance();
    const MercurialSettings newSettings = optionsPageWidget->settings();
    if (newSettings != plugin->settings()) {
        //assume success and emit signal that settings are changed;
        plugin->setSettings(newSettings);
        newSettings.writeSettings(Core::ICore::settings());
        emit settingsChanged();
    }
}

bool OptionsPage::matches(const QString &s) const
{
    return m_searchKeywords.contains(s, Qt::CaseInsensitive);
}
