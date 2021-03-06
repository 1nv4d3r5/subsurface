#include "divecomputermanagementdialog.h"
#include "models.h"
#include "ui_divecomputermanagementdialog.h"
#include "mainwindow.h"
#include <QMessageBox>
#include "../qthelper.h"
#include "../helpers.h"

DiveComputerManagementDialog::DiveComputerManagementDialog(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f),
	ui( new Ui::DiveComputerManagementDialog()),
	model(0)
{
	ui->setupUi(this);
	init();
	connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(tryRemove(QModelIndex)));
}

void DiveComputerManagementDialog::init()
{
	if (model)
		delete model;
	model = new DiveComputerModel(dcList.dcMap);
	ui->tableView->setModel(model);
}

DiveComputerManagementDialog* DiveComputerManagementDialog::instance()
{
	static DiveComputerManagementDialog *self = new DiveComputerManagementDialog();
	return self;
}

void DiveComputerManagementDialog::update()
{
	model->update();
	ui->tableView->resizeColumnsToContents();
	ui->tableView->setColumnWidth(DiveComputerModel::REMOVE, 22);
	layout()->activate();
}

void DiveComputerManagementDialog::tryRemove(const QModelIndex& index)
{
	if (index.column() != DiveComputerModel::REMOVE){
		return;
	}

	QMessageBox::StandardButton response = QMessageBox::question(
		this,
		tr("Remove the selected Dive Computer?"),
		tr("Are you sure that you want to \n remove the selected dive computer?"),
		QMessageBox::Ok | QMessageBox::Cancel
	);

	if (response == QMessageBox::Ok){
		model->remove(index);
	}
}

void DiveComputerManagementDialog::accept()
{
	model->keepWorkingList();
	hide();
	close();
}

void DiveComputerManagementDialog::reject()
{
	model->dropWorkingList();
	hide();
	close();
}
