/*
 *	This file is part of H4KvT.
 *
 *	Copyright (c) 2015 Christian Schmitz <tynn.dev@gmail.com>
 *
 *	H4KvT is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	H4KvT is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with H4KvT. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QFileInfo>
#include <QMessageBox>
#include <QMimeData>
#include <QString>
#include <QUrl>
#include <QWidget>

#include "window.hpp"


#define _MSTRING(tag) #tag
#define _TAG_EXT(tag) "/releases/tag/" _MSTRING(tag)
#define GITHUB_BASE "https://github.com/tynn/H4KvT"
#ifdef RTAG
#define GITHUB_URL GITHUB_BASE _TAG_EXT(RTAG)
#define QT_VERSION_FORMAT "<a href='https://download.qt.io/official_releases/qt/'>%1</a>"
#else
#define GITHUB_URL GITHUB_BASE
#define QT_VERSION_FORMAT "%1"
#endif


Window::Window(QWidget *parent) : QMainWindow(parent)
{
	resize(631, 157);
	setCentralWidget(new QWidget);
	setAcceptDrops(true);
	dropable = true;
	connect(this, &Window::idle, this, &Window::setDropable);
}

void Window::setDropable(bool isDropable)
{
	dropable = isDropable;
}

static bool _accept_drag(const QMimeData *data)
{
	int i = 0;
	if (data->hasUrls())
		for (QUrl &url: data->urls())
			if (url.isLocalFile()) {
				QFileInfo info(url.toLocalFile());
				if (info.isFile() && info.isReadable())
					i++;
			}
	return i == 1;
}

bool Window::eventFilter(QObject *, QEvent *event)
{
	if (event->type() != QEvent::DragEnter)
		return false;
	return _accept_drag(static_cast<QDragEnterEvent *>(event)->mimeData());
}

void Window::dragEnterEvent(QDragEnterEvent *event)
{
	if (dropable && _accept_drag(event->mimeData()))
		event->acceptProposedAction();
}

void Window::dropEvent(QDropEvent *event)
{
	for (QUrl &url: event->mimeData()->urls())
		if (url.isLocalFile() && QFileInfo(url.toLocalFile()).isFile()) {
			emit fileDroped(url.fileName(), url.toLocalFile());
			break;
		}
}

void Window::about()
{
	QMessageBox::about(this, tr("About %1").arg(APP_NAME),
			QString::fromLatin1("<p><i>" APP_NAME "</i> %1.</p><p>Copyright (C) %2 Christian Schmitz</p>")
				.arg(tr("is a simple application to calculate hashes of files")).arg(2015)
			+ tr("<p>This program is free software: you can redistribute it and/or modify "
					"it under the terms of the GNU General Public License as published by "
					"the Free Software Foundation, either version 3 of the License, or "
					"(at your option) any later version.</p>"
				 "<p>This program is distributed in the hope that it will be useful, "
					"but WITHOUT ANY WARRANTY; without even the implied warranty of "
					"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
					"GNU General Public License for more details.</p>"
				 "<p>You should have received a copy of the GNU General Public License "
					"along with this program.  If not, see <a href='http://%1'>%1</p>").arg("www.gnu.org/licenses/")
			+ tr("<p>You can download the source code via <a href='%1'>GitHub</a>.</p>").arg(GITHUB_URL)
			+ "<hr>" + tr("<p>This program uses %1 version %2.</p>"
						"<p>Copyright (C) %3 Digia Plc and/or its subsidiary(-ies) and other contributors.</p>"
						"<p>Qt is a trademark of Digia Plc and/or its subsidiary(-ies).</p>").arg("<a href='https://qt.io'>Qt</a>")
				.arg(QString::fromLatin1(QT_VERSION_FORMAT).arg(qVersion())).arg(2015));
}

