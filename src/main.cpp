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

#include <streambuf>
#include <fstream>
#include <sstream>

#include <cerrno>
#include <cstring>

#include <QtConcurrent>
#include <QtWidgets>

#include "window.hpp"

#include "md5/md5buf.hpp"
#include "sha1/sha1buf.hpp"


struct Vals {
	Vals() : Vals("", "") { _f = false; }
	Vals(const QString &qname, const QString &qpath)
		: md5(""), sha1(""), _f(true)
	{
		name = qname.toStdString();
		path = qpath.toStdString();
	}
	std::string name, path;
	std::string md5, sha1;
	bool _f;
};
#define ANY(vals,val) (vals.md5==val||vals.sha1==val)
#define ALL(vals,val) (vals.md5==val&&vals.sha1==val)

static Vals & operator<<=(Vals &left, const Vals &right)
{
	left = right;
	left._f = false;
	return left;
}

static Vals & operator+=(Vals &left, const Vals &right)
{
	if (right._f || left.path != right.path)
		return left <<= right;
	if (right.md5 != "")
		left.md5 = right.md5;
	if (right.sha1 != "")
		left.sha1 = right.sha1;
	return left;
}

static void hash(const std::string filename, std::streambuf &buf)
{
	std::ifstream in(filename);
	in.exceptions(std::ifstream::failbit|std::ifstream::badbit);
	std::ostream out(&buf);
	out << in.rdbuf();
}

static std::string md5hash(const std::string filename)
{
	md5buf md5;
	hash(filename, md5);
	return md5.hexdigest();
}

static std::string sha1hash(const std::string filename)
{
	sha1buf sha1;
	hash(filename, sha1);
	return sha1.hexdigest();
}

static Vals update(int which, Vals vals)
{
	try {
		switch (which) {
			case 0:
				if (vals.md5 == "")
					vals.md5 = md5hash(vals.path);
				break;
			case 1:
				if (vals.sha1 == "")
					vals.sha1 = sha1hash(vals.path);
				break;
		}
	} catch (std::ifstream::failure &) {
		vals.name = strerror(errno);
		vals.path = "";
	}
	return vals;
}


class HexVal : public QValidator
{
	public:
		explicit HexVal(QObject * parent=0)
			: QValidator(parent), nonhex("[^0-9a-fA-F]") { }

		QValidator::State validate(QString &input, int &pos) const
		{
			/* fixup :*/
			pos -= input.left(pos).count(nonhex);
			input.replace(nonhex, "");
			return Acceptable;
		}

	private:
		QRegExp nonhex;
};

class Stack : public QStackedLayout
{
	public:
		explicit Stack() : QStackedLayout() { }

		Qt::Orientations expandingDirections() const { return Qt::Horizontal; }
};


int main(int argc, char **argv)
{
	Vals vals;

	/* the application */
	QApplication app(argc, argv);
	app.setApplicationName(APP_NAME);
	app.setWindowIcon(QIcon(":/icon"));
	Window window;

	/* translations */
	QTranslator qtr;
	if (qtr.load("default_" + QLocale::system().name(), ":/"))
		app.installTranslator(&qtr);

	/* display information */
	QTextEdit *text = new QTextEdit;
	text->setReadOnly(true);
	text->setLineWrapMode(QTextEdit::NoWrap);
	text->setToolTip(Window::tr("Drop any file for hashing"));
	QObject::connect(&window, &Window::idle, text, &QWidget::setEnabled);

	/* compare hash */
	QLineEdit *test = new QLineEdit;
	test->setValidator(new HexVal);
	test->setToolTip(Window::tr("Compare hash"));

	QObject::connect(test, &QLineEdit::textChanged,
		[&](const QString &newValue) { if (vals.name != "") {
			std::string hashVal = newValue.toStdString();
			std::transform(hashVal.begin(), hashVal.end(), hashVal.begin(), ::tolower);
			std::stringstream html;
			if (hashVal != "")
				html << "<style>.h" << hashVal << "{color:green}</style>";
			html << "<div style='margin-bottom:2; font-size:31px'><i><b>" << vals.name << "</b></i></div>";
			html << "<div style='margin-bottom:7; margin-left:23; font-size:13px'>" << vals.path << "</div>";
			if (!ALL(vals,"")) {
				html << "<div style='font-size:13px'><table>";
				if (vals.md5 != "")
					html << "<tr><td>md5: </td><td class='h" << vals.md5 << "'>" << vals.md5 << "</td</tr>";
				if (vals.sha1 != "")
					html << "<tr><td>sha1: </td><td class='h" << vals.sha1 << "'>" << vals.sha1 << "</td</tr>";
				html << "</table></div>";
			}
			text->setHtml(QString::fromStdString(html.str()));
			test->setStyleSheet(ANY(vals,hashVal) ? "color:green" : "");
		}});

	/* error messages */
	QLabel *error = new QLabel;
	error->setStyleSheet("color:red");

	/* test or error */
	QStackedLayout *stack = new Stack;
	stack->addWidget(error);
	stack->addWidget(test);
	stack->setCurrentIndex(1);

	/* toggle test or error */
	QPushButton *hash = new QPushButton(QIcon(":/icon"), "");
	hash->setCheckable(true);
	hash->setChecked(true);
	hash->setContextMenuPolicy(Qt::CustomContextMenu);
	hash->setToolTip(Window::tr("Compare hash"));
	QObject::connect(hash, &QPushButton::toggled, stack, &QStackedLayout::setCurrentIndex);

	/* store meth */
	QSettings settings("H4KvT", "H4KvT");

	/* hashing method */
	QComboBox *meth = new QComboBox;
	meth->addItem("md5");
	meth->addItem("sha1");
	//meth->addItem("sha256");
	//meth->addItem("sha512");
	meth->setToolTip(Window::tr("Hashing method"));
	meth->setCurrentIndex(settings.value("HashingMethod", 0).toInt());
	QObject::connect(&window, &Window::idle, meth, &QWidget::setEnabled);

	QObject::connect(meth, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		[&settings](int index) { settings.setValue("HashingMethod", index); });

	/* toolbar */
	QHBoxLayout *pane = new QHBoxLayout;
	pane->addWidget(hash, 0, Qt::AlignLeft);
	pane->addLayout(stack);
	pane->addWidget(meth, 0, Qt::AlignRight);

	/* main layout */
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(text);
	layout->addLayout(pane);

	/* the window */
	window.centralWidget()->setLayout(layout);
	test->installEventFilter(&window);
	window.show();

	/* future hashing */
	QFutureWatcher<Vals> zu;
	QObject::connect(&zu, &QFutureWatcher<Vals>::finished,
		[&]() {
			Vals valsi = zu.future().result();
			window.idle(true);
			if (valsi.path == "") {
				error->setText(QString::fromStdString(valsi.name));
				hash->setChecked(false);
			} else {
				error->clear();
				vals += valsi;
				test->textChanged(test->text());
			}
		});

	QObject::connect(meth, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
		[&](int index) { if (vals.name != "") {
			window.idle(false);
			zu.setFuture(QtConcurrent::run(&update, index, vals));
		}});

	QObject::connect(&window, &Window::fileDroped,
		[&](const QString &name, const QString &path) {
			window.idle(false);
			zu.setFuture(QtConcurrent::run(&update, meth->currentIndex(), Vals(name, path)));
		});

	/* hashing info */
	QGraphicsBlurEffect blur;
	blur.setBlurHints(QGraphicsBlurEffect::AnimationHint);

	QPropertyAnimation anim(&blur, "blurRadius");
	anim.setDuration(2000);
	anim.setLoopCount(-1);
	anim.setKeyValueAt(0, 0);
	anim.setKeyValueAt(0.5, 3);
	anim.setKeyValueAt(1, 0);

	QLabel *hashing = new QLabel;
	hashing->setPixmap(QPixmap(":/icon"));
	hashing->setAttribute(Qt::WA_TransparentForMouseEvents);
	hashing->setGraphicsEffect(&blur);
	hashing->hide();
	(new QHBoxLayout(text))->addWidget(hashing, 0, Qt::AlignCenter);

	QObject::connect(&blur, &QGraphicsBlurEffect::blurRadiusChanged,
		hashing, static_cast<void(QWidget::*)()>(&QWidget::update));

	QObject::connect(&window, &Window::idle,
		[&](bool idle) {
			if (idle) {
				hashing->hide();
				anim.stop();
			} else {
				hashing->show();
				anim.start();
			}
		});

	/* about app */
	QMenu *about = new QMenu;
	QObject::connect(hash, &QWidget::customContextMenuRequested,
		[&about, &hash](const QPoint &pos) { about->exec(hash->mapToGlobal(pos)); });

	QAction *action = about->addAction(QIcon(":/icon"), Window::tr("About %1").arg(APP_NAME));
	action->setMenuRole(QAction::AboutRole);
	QObject::connect(action, &QAction::triggered, &window, &Window::about);

	return app.exec();
}

