// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2017-2020 The Raven Core developers
// Copyright (c) 2022-2023 The Gamepass Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GAMEPASS_QT_CSVMODELWRITER_H
#define GAMEPASS_QT_CSVMODELWRITER_H

#include <QList>
#include <QObject>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
QT_END_NAMESPACE

/** Export a Qt table model to a CSV file. This is useful for analyzing or post-processing the data in
    a spreadsheet.
 */
class CSVModelWriter : public QObject
{
    Q_OBJECT

public:
    explicit CSVModelWriter(const QString &filename, QObject *parent = 0);

    void setModel(const QAbstractItemModel *model);
    void addColumn(const QString &title, int column, int role=Qt::EditRole);

    /** Perform export of the model to CSV.
        @returns true on success, false otherwise
    */
    bool write();

private:
    QString filename;
    const QAbstractItemModel *model;

    struct Column
    {
        QString title;
        int column;
        int role;
    };
    QList<Column> columns;
};

#endif // GAMEPASS_QT_CSVMODELWRITER_H
