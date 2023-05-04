// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) 2017-2020 The Raven Core developers
// Copyright (c) 2022-2023 The Gamepass Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef GAMEPASS_QT_GAMEPASSADDRESSVALIDATOR_H
#define GAMEPASS_QT_GAMEPASSADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class GamepassAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit GamepassAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Gamepass address widget validator, checks for a valid Gamepass address.
 */
class GamepassAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit GamepassAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // GAMEPASS_QT_GAMEPASSADDRESSVALIDATOR_H
