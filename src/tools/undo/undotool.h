// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2017-2019 Alejandro Sirgo Rica & Contributors

#pragma once

#include "src/tools/abstractactiontool.h"

class UndoTool : public AbstractActionTool
{
    Q_OBJECT
public:
    explicit UndoTool(QObject* parent = nullptr);

    bool closeOnButtonPressed() const override;

    QIcon icon(const QColor& background, bool inEditor) const override;
    QString name() const override;
    QString description() const override;

    CaptureTool* copy(QObject* parent = nullptr) override;

protected:
    ToolType type() const override;

public slots:
    void pressed(const CaptureContext& context) override;
};
