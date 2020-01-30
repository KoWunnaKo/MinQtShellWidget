/***************************************************************************
 *   copyright       : (C) 2020 by Transporter                             *
 *   https://github.com/OgreTransporter/MinQtShellWidget                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qshell.h"
#include <QScrollBar>

QShell::QShell(QWidget *parent)
	: QPlainTextEdit(parent)
	, m_process(NULL)
	, m_shutdown(false)
{
	document()->setMaximumBlockCount(100);
	QPalette p = palette();
	p.setColor(QPalette::Base, Qt::black);
	p.setColor(QPalette::Text, Qt::green);
	setPalette(p);
	QFont font = QFont("Courier");
	font.setStyleHint(QFont::Monospace);
	font.setPointSize(10);
	font.setFixedPitch(true);
	setFont(font);
	m_process = new QProcess(this);
	connect(m_process, SIGNAL(finished(int)), this, SLOT(shellFinished(int)));
	connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(shellError()));
	connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(shellOutput()));
#ifdef Q_OS_WINDOWS
	m_process->setProgram("cmd");
	m_process->setArguments({ "/K" });
#else
	m_process->setProgram("/bin/bash");
#endif
	m_process->start();
}

QShell::~QShell()
{
	if (m_process != NULL)
	{
		m_shutdown = true;
		m_process->write("exit\n");
		m_process->waitForFinished(1000);
		m_process->kill();
		delete m_process;
		m_process = NULL;
	}
}

void QShell::putData(const QByteArray &data)
{
	insertPlainText(data);
	QScrollBar *bar = verticalScrollBar();
	bar->setValue(bar->maximum());
}

void QShell::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_Backspace:
	case Qt::Key_Left:
	case Qt::Key_Right:
	case Qt::Key_Up:
	case Qt::Key_Down:
		break;
	default:
		QPlainTextEdit::keyPressEvent(e);
		if (e->key() != Qt::Key_Return)
			m_command_buffer += e->text().toLocal8Bit();
		else
		{
			m_last_command = m_command_buffer + "\n";
			m_command_buffer = "";
			for (int i = 0; i < m_last_command.size(); i++) textCursor().deletePreviousChar();
			m_process->write(m_last_command.toLocal8Bit());
		}
		emit getRawData(e->text().toLocal8Bit());
		break;
	}
}

void QShell::mousePressEvent(QMouseEvent *e)
{
	Q_UNUSED(e)
	setFocus();
}

void QShell::mouseDoubleClickEvent(QMouseEvent *e)
{
	Q_UNUSED(e)
}

void QShell::contextMenuEvent(QContextMenuEvent *e)
{
	Q_UNUSED(e)
}

void QShell::shellFinished(int exitCode)
{
	Q_UNUSED(exitCode)
	if (!m_shutdown) m_process->start();
}

void QShell::shellError()
{
	insertPlainText(m_process->readAllStandardOutput());
	QScrollBar *bar = verticalScrollBar();
	bar->setValue(bar->maximum());
}

void QShell::shellOutput()
{
	insertPlainText(m_process->readAllStandardOutput());
	QScrollBar *bar = verticalScrollBar();
	bar->setValue(bar->maximum());
}
