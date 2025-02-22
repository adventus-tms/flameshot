// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2017-2019 Alejandro Sirgo Rica & Contributors

#include "configwindow.h"
#include "src/config/filenameeditor.h"
#include "src/config/generalconf.h"
#include "src/config/shortcutswidget.h"
#include "src/config/strftimechooserwidget.h"
#include "src/config/visualseditor.h"
#include "src/utils/colorutils.h"
#include "src/utils/confighandler.h"
#include "src/utils/globalvalues.h"
#include "src/utils/pathinfo.h"
#include <QFileSystemWatcher>
#include <QIcon>
#include <QKeyEvent>
#include <QTabBar>
#include <QVBoxLayout>

// ConfigWindow contains the menus where you can configure the application

ConfigWindow::ConfigWindow(QWidget* parent)
  : QWidget(parent)
{
    // We wrap QTabWidget in a QWidget because of a Qt bug
    auto layout = new QVBoxLayout(this);
    m_tabs = new QTabWidget(this);
    m_tabs->tabBar()->setUsesScrollButtons(false);
    layout->addWidget(m_tabs);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":img/app/flameshot.svg"));
    setWindowTitle(tr("Configuration"));

    auto changedSlot = [this](QString s) {
        QStringList files = m_configWatcher->files();
        if (!files.contains(s)) {
            this->m_configWatcher->addPath(s);
        }
        emit updateChildren();
    };
    m_configWatcher = new QFileSystemWatcher(this);
    m_configWatcher->addPath(ConfigHandler().configFilePath());
    connect(
      m_configWatcher, &QFileSystemWatcher::fileChanged, this, changedSlot);

    QColor background = this->palette().window().color();
    bool isDark = ColorUtils::colorIsDark(background);
    QString modifier =
      isDark ? PathInfo::whiteIconPath() : PathInfo::blackIconPath();

    // visuals
    m_visuals = new VisualsEditor();
    m_tabs->addTab(
      m_visuals, QIcon(modifier + "graphics.svg"), tr("Interface"));

    // filename
    m_filenameEditor = new FileNameEditor();
    m_tabs->addTab(m_filenameEditor,
                   QIcon(modifier + "name_edition.svg"),
                   tr("Filename Editor"));

    // general
    m_generalConfig = new GeneralConf();
    m_tabs->addTab(
      m_generalConfig, QIcon(modifier + "config.svg"), tr("General"));

    // shortcuts
    m_shortcuts = new ShortcutsWidget();
    m_tabs->addTab(
      m_shortcuts, QIcon(modifier + "shortcut.svg"), tr("Shortcuts"));

    // connect update sigslots
    connect(this,
            &ConfigWindow::updateChildren,
            m_filenameEditor,
            &FileNameEditor::updateComponents);
    connect(this,
            &ConfigWindow::updateChildren,
            m_visuals,
            &VisualsEditor::updateComponents);
    connect(this,
            &ConfigWindow::updateChildren,
            m_generalConfig,
            &GeneralConf::updateComponents);
}

void ConfigWindow::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Escape) {
        close();
    }
}
