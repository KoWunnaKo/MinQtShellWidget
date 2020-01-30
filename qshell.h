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

#ifndef QSHELL_H
#define QSHELL_H

#include <QPlainTextEdit>
#include <QProcess>

class QShell : public QPlainTextEdit
{
	Q_OBJECT
public:
	explicit QShell(QWidget *parent = nullptr);
	virtual ~QShell();

	void putData(const QByteArray &data);

signals:
	void getRawData(const QByteArray &data);

protected:
	void keyPressEvent(QKeyEvent *e) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseDoubleClickEvent(QMouseEvent *e) override;
	void contextMenuEvent(QContextMenuEvent *e) override;

private slots:
	void shellFinished(int exitCode);
	void shellError();
	void shellOutput();

private:
	QString m_last_command;
	QString m_command_buffer;
	QProcess *m_process;
	bool m_shutdown;
};

#endif // QSHELL_H
