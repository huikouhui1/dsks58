#pragma once

#include <QWidget>
#include "ui_MainWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidgetClass; };
QT_END_NAMESPACE

class MainWidget : public QWidget
{
public:
	MainWidget(QWidget *parent = nullptr);
	~MainWidget();

private:
	Ui::MainWidgetClass *ui;
};
