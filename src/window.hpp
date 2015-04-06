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

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QLayout>
#include <QMainWindow>
#include <QObject>
#include <QString>

#define APP_NAME "H4KvT"

class Window : public QMainWindow
{
	Q_OBJECT

	public:
		explicit Window(QWidget *parent=0);
		~Window() { }

		bool eventFilter(QObject *, QEvent *);

	public slots:
		void about();

	protected:
		void dragEnterEvent(QDragEnterEvent *);
		void dropEvent(QDropEvent *);

	Q_SIGNALS:
		void fileDroped(const QString &, const QString &);
		void idle(bool);

	private:
		void setDropable(bool);
		bool dropable;
};

