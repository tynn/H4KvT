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

#include "hashbuf.hpp"
#include "md5/md5buf.hpp"
#include "sha1/sha1buf.hpp"
#include "sha2/sha224buf.hpp"
#include "sha2/sha256buf.hpp"
#include "sha2/sha384buf.hpp"
#include "sha2/sha512buf.hpp"


struct Vals {
	Vals() : Vals("", "") { _f = false; }
	Vals(const QString &qname, const QString &qpath)
		: md5(""), sha1(""), sha224(""), sha256(""), sha384(""), sha512(""), _f(true)
	{
		name = qname.toStdString();
		path = qpath.toStdString();
	}
	std::string name, path;
	std::string md5, sha1, sha224, sha256, sha384, sha512;
	bool _f;
};
#define _EQ_LO(vals,EQ,LO) (vals.md5 EQ LO vals.sha1 EQ LO vals.sha224 EQ LO vals.sha256 EQ LO vals.sha384 EQ LO vals.sha512 EQ)
#define ANY(vals,val) _EQ_LO(vals,==val,||)
#define ALL(vals,val) _EQ_LO(vals,==val,&&)

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
	if (right.sha224 != "")
		left.sha224 = right.sha224;
	if (right.sha256 != "")
		left.sha256 = right.sha256;
	if (right.sha384 != "")
		left.sha384 = right.sha384;
	if (right.sha512 != "")
		left.sha512 = right.sha512;
	return left;
}

static std::string hash(const std::string filename, hashbuf &&buf)
{
	std::ifstream in(filename);
	in.exceptions(std::ifstream::failbit|std::ifstream::badbit);
	std::ostream out(&buf);
	out << in.rdbuf();
	return buf.hex();
}

static Vals update(const QString which, Vals vals)
{
	try {
		if (which == "md5") {
			if (vals.md5 == "")
				vals.md5 = hash(vals.path, md5buf());
		} else if (which == "sha1") {
			if (vals.sha1 == "")
				vals.sha1 = hash(vals.path, sha1buf());
		} else if (which == "sha224") {
			if (vals.sha224 == "")
				vals.sha224 = hash(vals.path, sha224buf());
		} else if (which == "sha256") {
			if (vals.sha256 == "")
				vals.sha256 = hash(vals.path, sha256buf());
		} else if (which == "sha384") {
			if (vals.sha384 == "")
				vals.sha384 = hash(vals.path, sha384buf());
		} else if (which == "sha512") {
			if (vals.sha512 == "")
				vals.sha512 = hash(vals.path, sha512buf());
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
		explicit HexVal(QObject *parent=0)
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


static bool moreOrLess()
{
	const QStringList &arg = QCoreApplication::arguments();
	int more = arg.lastIndexOf("-more");
	int less = arg.lastIndexOf("-less");

	if (more == -1 && less == -1)
		throw 0;

	return more > less;
}


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
	if (qtr.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
		app.installTranslator(&qtr);

	QTranslator htr;
	if (htr.load("H4KvT_" + QLocale::system().name(), ":/"))
		app.installTranslator(&htr);

	/* display information */
	QTextEdit *text = new QTextEdit;
	text->setReadOnly(true);
	text->setLineWrapMode(QTextEdit::NoWrap);
	text->setToolTip(Window::tr("Drop any file for hashing"));
	QObject::connect(&window, &Window::idle, text, &QWidget::setEnabled);

	/* compare hash */
	QLineEdit *test = new QLineEdit;
	HexVal hexval;
	test->setValidator(&hexval);
	test->installEventFilter(&window);
	test->setToolTip(Window::tr("Compare hash"));

	QObject::connect(test, &QLineEdit::textChanged,
		[&](const QString &newValue) { if (vals.name != "") {
			std::string hashVal = newValue.toStdString();
			std::transform(hashVal.begin(), hashVal.end(), hashVal.begin(), ::tolower);
			std::stringstream html;
			if (hashVal != "")
				html << "<style>.h" << hashVal << "{color:green}</style>";
			html << "<div style='margin-bottom:2; font-size:27px'><i><b>" << vals.name << "</b></i></div>";
			html << "<div style='margin-bottom:7; margin-left:23; font-size:13px'>" << vals.path << "</div>";
			if (!ALL(vals,"")) {
				html << "<div style='font-size:13px'><table>";
				if (vals.md5 != "")
					html << "<tr><td>md5: </td><td class='h" << vals.md5 << "'>" << vals.md5 << "</td</tr>";
				if (vals.sha1 != "")
					html << "<tr><td>sha1: </td><td class='h" << vals.sha1 << "'>" << vals.sha1 << "</td</tr>";
				if (vals.sha224 != "")
					html << "<tr><td>sha224: </td><td class='h" << vals.sha224 << "'>" << vals.sha224 << "</td</tr>";
				if (vals.sha256 != "")
					html << "<tr><td>sha256: </td><td class='h" << vals.sha256 << "'>" << vals.sha256 << "</td</tr>";
				if (vals.sha384 != "")
					html << "<tr><td>sha384: </td><td class='h" << vals.sha384 << "'>" << vals.sha384 << "</td</tr>";
				if (vals.sha512 != "")
					html << "<tr><td>sha512: </td><td class='h" << vals.sha512 << "'>" << vals.sha512 << "</td</tr>";
				html << "</table></div>";
			}
			int horizontal = text->horizontalScrollBar()->value();
			int vertical = text->verticalScrollBar()->value();
			text->setHtml(QString::fromStdString(html.str()));
			text->horizontalScrollBar()->setValue(horizontal);
			text->verticalScrollBar()->setValue(vertical);
			test->setStyleSheet(ANY(vals,hashVal) ? "color:green" : "");
		}});

	/* error messages */
	QLabel *error = new QLabel;
	error->setStyleSheet("color:red");

	/* test or error */
	QStackedWidget *stack = new QStackedWidget;
	delete stack->layout();
	stack->setLayout(new Stack);
	stack->addWidget(error);
	stack->addWidget(test);
	stack->setCurrentIndex(1);

	/* toggle test or error */
	QPushButton *hash = new QPushButton(QIcon(":/icon"), "");
	hash->setCheckable(true);
	hash->setChecked(true);
	hash->setContextMenuPolicy(Qt::CustomContextMenu);
	hash->setToolTip(Window::tr("Compare hash"));
	QObject::connect(hash, &QPushButton::toggled, stack, &QStackedWidget::setCurrentIndex);

	/* store method */
	QSettings settings("H4KvT", "H4KvT");

	/* more methods */
	bool more;
	try {
		settings.setValue("MoreHashingMethods", more = moreOrLess());
	} catch (...) {
		more = settings.value("MoreHashingMethods", false).toBool();
	}

	/* hashing method */
	QComboBox *meth = new QComboBox;
	meth->addItem("md5");
	meth->addItem("sha1");
	if (more) meth->addItem("sha224");
	meth->addItem("sha256");
	if (more) meth->addItem("sha384");
	meth->addItem("sha512");
	meth->setToolTip(Window::tr("Hashing method"));
	meth->setCurrentText(settings.value("HashingMethod", "md5").toString());
	QObject::connect(&window, &Window::idle, meth, &QWidget::setEnabled);

	QObject::connect(meth, &QComboBox::currentTextChanged,
		[&](const QString &text) { settings.setValue("HashingMethod", text); });

	/* toolbar */
	QHBoxLayout *pane = new QHBoxLayout;
	pane->addWidget(hash, 0, Qt::AlignLeft);
	pane->addWidget(stack);
	pane->addWidget(meth, 0, Qt::AlignRight);

	/* main layout */
	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(text);
	layout->addLayout(pane);

	/* the window */
	window.centralWidget()->setLayout(layout);
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

	QObject::connect(meth, &QComboBox::currentTextChanged,
		[&](const QString &text) { if (vals.name != "") {
			window.idle(false);
			zu.setFuture(QtConcurrent::run(&update, text, vals));
		}});

	QObject::connect(&window, &Window::fileDroped,
		[&](const QString &name, const QString &path) {
			window.idle(false);
			zu.setFuture(QtConcurrent::run(&update, meth->currentText(), Vals(name, path)));
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
	QMenu about;
	QObject::connect(hash, &QWidget::customContextMenuRequested,
		[&about, &hash](const QPoint &pos) { about.exec(hash->mapToGlobal(pos)); });

	QAction *action = about.addAction(QIcon(":/icon"), Window::tr("About %1").arg(APP_NAME));
	action->setMenuRole(QAction::AboutRole);
	QObject::connect(action, &QAction::triggered, &window, &Window::about);

	return app.exec();
}

