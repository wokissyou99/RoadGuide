#include "luxianguihua.h"
#include <qpushbutton.h>
#include <qfiledialog.h>
#include <qcheckbox.h>
#include <qtextedit.h>

int GAtime = 1; //遗传算法次数
extern double W[4];
extern int car_kind;//车辆种类
extern double car_time;
extern double unload_time;
extern double car_speed;
extern double car_distance;
extern double P;//惩罚系数
extern vector<double> car_weight;//记录每种车的载重量
extern vector<int> car_num;//记录每种车的数量
extern vector<double>car_oil;//记录每种车的油费

luxianguihua::luxianguihua(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	weight = 0;
	connect(ui.pushButton, &QPushButton::clicked, this, &luxianguihua::loading);
	connect(ui.checkBox, &QCheckBox::clicked, this, &luxianguihua::weight_c);
	connect(ui.checkBox_2, &QCheckBox::clicked, this, &luxianguihua::weight_d);
	connect(ui.checkBox_3, &QCheckBox::clicked, this, &luxianguihua::weight_n);
	connect(ui.checkBox_4, &QCheckBox::clicked, this, &luxianguihua::weight_l);
	connect(ui.pushButton_2, &QPushButton::clicked, this, &luxianguihua::_show);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &luxianguihua::imformation);
	//connect(ui.textEdit,&QTextEdit::QTextEdit)
}

void luxianguihua::loading() {
	
	srand((int)(time(0)));
	p = new(graph);
	QString str_path = "";
	for (;;) {
		QStringList str_path_list = QFileDialog::getOpenFileNames(this, QString::fromLocal8Bit("选择文本文件"), QString::fromLocal8Bit("测试用例1.txt"), QString::fromLocal8Bit("文本文件(*.txt);;"));
		if (str_path_list.size() == 0)
			continue;
		for (int i = 0; i < str_path_list.size(); i++) {
			str_path = str_path_list[i];
		}	
		break;
	}
	std::string _str_path = (string)str_path.toLocal8Bit();

	p->Init(_str_path);
	floyd(*p);
	getCopyvertexArray(*p);

	ui.pushButton->setText(QString::fromLocal8Bit("加载成功"));
}

void luxianguihua::imformation() {
	QString str;
	QStringList str_list;
	car_kind = ui.lineEdit->text().toInt();
	car_weight.resize(car_kind);
	car_num.resize(car_kind);
	car_oil.resize(car_kind);

	str = ui.lineEdit_2->text();
	str_list = str.split(" ");
	for (int i = 0; i < car_kind; i++) {
		car_oil[i] = str_list[i].toDouble();
	}
	str.clear();
	str_list.clear();

	str = ui.lineEdit_3->text();
	str_list = str.split(" ");
	for (int i = 0; i < car_kind; i++) {
		car_num[i] = str_list[i].toInt();
	}
	str.clear();
	str_list.clear();

	str = ui.lineEdit_4->text();
	str_list = str.split(" ");
	for (int i = 0; i < car_kind; i++) {
		car_weight[i] = str_list[i].toDouble();
	}
	str.clear();
	str_list.clear();

	car_distance = ui.lineEdit_5->text().toDouble();
	car_speed = ui.lineEdit_6->text().toDouble();
	car_time = ui.lineEdit_7->text().toDouble();
	unload_time = ui.lineEdit_8->text().toDouble();
	P = ui.lineEdit_9->text().toDouble();
}

void luxianguihua::weight_c() {
	if (ui.checkBox->isChecked()) {
		weight += 8;
	}
}

void luxianguihua::weight_d() {
	if (ui.checkBox_2->isChecked()) {
		weight += 4;
	}
}

void luxianguihua::weight_n() {
	if (ui.checkBox_3->isChecked()) {
		weight += 2;
	}
}

void luxianguihua::weight_l() {
	if (ui.checkBox_4->isChecked()) {
		weight += 1;
	}
}

void luxianguihua::_show() {
	switch (weight) {
	case 0:
		for (int i = 0; i < 4; i++) {
			W[i] = 1;
		}
	case 2:
		for (int i = 0; i < 4; i++) {
			W[i] = 1;
		}
	case 4:
		for (int i = 0; i < 4; i++) {
			W[i] = 1;
		}
	case 8:
		for (int i = 0; i < 4; i++) {
			W[i] = 1;
		}
	default:
		for (int i = 0; i < 4; i++) {
			W[i] = 1;
		}
	}

	bestone = new(individuality);
	for (int i = 0; i < GAtime; i++)
	{
		population a;
		a.GA(a);
		if (bestone->Fitness < a.group[0].Fitness)
		{
			*bestone = a.group[0];
		}
	}

	QString str;
	vector<vector<int>> each_road = bestone->get_each_road();
	unsigned int roadnum = bestone->Chromosome2.size() - 1;
	for (unsigned i = 0; i < each_road.size(); i++) {
		for (unsigned j = 0; j < each_road[i].size(); j++) {
			each_road[i][j] = indexmap[(each_road[i][j])];
		}
	}
	vector<vector<string>> bestpath = find_path(each_road, roadnum);
	vector<double> bestcar = bestone->Chromosome2;
	ui.textBrowser->append(QString::fromLocal8Bit("最终分配方案为: "));
	for (unsigned int i = 0; i < bestpath.size(); i++) {
		str.sprintf("%d", i + 1);
		ui.textBrowser->append(QString::fromLocal8Bit("第") + str +QString::fromLocal8Bit("种分配方案为:"));
		for (unsigned int j = 0; j < bestpath[i].size(); j++) {
			str.clear();
			str = QString::fromStdString(bestpath[i][j]);
			ui.textBrowser->insertPlainText(str);
			if (j != bestpath[i].size() - 1) {
				cout << "->";
				ui.textBrowser->insertPlainText("->");
			}
		}
		str.clear();
		str.sprintf("%d", (int)bestone->Chromosome2[i]);
		ui.textBrowser->append(QString::fromLocal8Bit("车辆分配方案为") + str);
	}
}