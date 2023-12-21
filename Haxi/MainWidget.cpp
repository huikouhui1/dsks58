#include "MainWidget.h"

MainWidget::MainWidget(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::MainWidgetClass())
{
	ui->setupUi(this);
	connect(ui->pushButton_2, &QPushButton::clicked, this, [=]() {
		int num = QRandomGenerator::global()->bounded(256);
		ui->graphicsView->insert(num, num);
		int allfindLen = 0;
		for (auto len : ui->graphicsView->hashTable->auxInfo)
		{
			allfindLen += (1 + len) * len / 2;
		}
		int allErrLen = 0;
		for (auto len : ui->graphicsView->hashTable->auxInfo)
		{
			len += 1;
			allErrLen += len;
		}

		double success = allfindLen / (double)ui->graphicsView->hashTable->size;
		double fail = (allErrLen) / (double)13;
		ui->labelASL->setText(QString("success:%1 fail:%2").arg(QString::asprintf("%f", success)).arg(QString::asprintf("%f", fail)));


		});
	connect(ui->pushButton, &QPushButton::clicked, this, [=]() {
		int v = ui->lineEdit->text().toInt();
		ui->graphicsView->insert(v, v);
		int allfindLen = 0;
		for (auto len : ui->graphicsView->hashTable->auxInfo)
		{
			allfindLen += (1 + len) * len / 2;
		}
		int allErrLen = 0;
		for (auto len : ui->graphicsView->hashTable->auxInfo)
		{
			len += 1;
			allErrLen += len;

		}

		double success = allfindLen / (double)ui->graphicsView->hashTable->size;
		double fail = (allErrLen) / (double)13;
		ui->labelASL->setText(QString("success:%1 fail:%2").arg(QString::asprintf("%f", success)).arg(QString::asprintf("%f", fail)));
		});
}

MainWidget::~MainWidget()
{
	delete ui;
}
