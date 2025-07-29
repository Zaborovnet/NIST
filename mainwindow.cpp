#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patientdata.h"
#include "progresswidget.h"

#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , _ui_po(new Ui::MainWindow)
{
  _ui_po->setupUi(this);

  // Настройка таблицы
  _ui_po->dataTable->setColumnCount(4);
  _ui_po->dataTable->setHorizontalHeaderLabels({"Фамилия", "Диагноз", "Процедура", "Параметры"});
  _ui_po->dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // Соединение сигналов и слотов
  connect(_ui_po->detailsButton, &QPushButton::clicked,
           this, &MainWindow::onShowDetailsClicked);
  connect(_ui_po->loadButton, &QPushButton::clicked,
           this, &MainWindow::onLoadCSVClicked);
  connect(_ui_po->createDemoButton, &QPushButton::clicked,
           this, &MainWindow::onCreateDemoCSVClicked);
}

MainWindow::~MainWindow()
{
  delete _ui_po;
}

void MainWindow::onLoadCSVClicked()
{
  // Выбор файла через диалог
  QString filename = QFileDialog::getOpenFileName(
      this,
      "Выберите файл с данными",
      "",
      "CSV Files (*.csv);;All Files (*)"
      );

  if (!filename.isEmpty()) {
      // Попробуем сначала в UTF-8
      bool success = tryLoadWithEncoding(filename, "UTF-8");

      // Если не получилось, попробуем другие кодировки
      if (!success) {
          success = tryLoadWithEncoding(filename, "Windows-1251");
        }
      if (!success) {
          success = tryLoadWithEncoding(filename, "CP866");
        }

      if (!success) {
          QMessageBox::critical(this, "Ошибка",
                                 "Не удалось загрузить файл. Проверьте кодировку и формат данных.");
        }
    }
}

bool MainWindow::tryLoadWithEncoding(const QString& filename, const QString& encoding)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      return false;
    }

         // Очистка предыдущих данных
  _patientsData_vec.clear();
  _ui_po->patientsCombo->clear();
  _ui_po->dataTable->setRowCount(0);

  QTextStream in(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  in.setCodec(encoding.toUtf8());
#else
  if (encoding == "UTF-8") {
      in.setEncoding(QStringConverter::Utf8);
    } else if (encoding == "Windows-1251") {
      in.setEncoding(QStringConverter::Latin1);
    } else {
      in.setEncoding(QStringConverter::System);
    }
#endif

         // Пропускаем заголовок
  if (!in.atEnd()) in.readLine();

  // Чтение данных
  int validLines = 0;
  while (!in.atEnd())
    {
      QString line = in.readLine();
      QStringList fields = line.split(";");

      if (fields.size() < 6) continue;

      // Проверяем, что первое поле похоже на фамилию (содержит только буквы)
      QRegularExpression nameRegex("^[\\p{L} -]+$");
      if (!nameRegex.match(fields[0].trimmed()).hasMatch()) {
          continue;
        }

      // Если хотя бы одна строка прошла проверку - считаем кодировку подходящей
      validLines++;

      PatientData patient(
          fields[0].trimmed(),
          fields[1].trimmed(),
          fields[2].trimmed(),
          fields[3].trimmed(),
          fields[4].toInt(),
          fields[5].toInt()
          );

      _patientsData_vec.append(patient);
      _ui_po->patientsCombo->addItem(patient.surname());

      int row = _ui_po->dataTable->rowCount();
      _ui_po->dataTable->insertRow(row);
      _ui_po->dataTable->setItem(row, 0, new QTableWidgetItem(patient.surname()));
      _ui_po->dataTable->setItem(row, 1, new QTableWidgetItem(patient.diagnosis()));
      _ui_po->dataTable->setItem(row, 2, new QTableWidgetItem(patient.procedure()));
      _ui_po->dataTable->setItem(row, 3, new QTableWidgetItem(patient.parameters()));
    }

  file.close();

  return validLines > 0;
}

#include <QTextCodec>

void MainWindow::onCreateDemoCSVClicked()
{
  QString filename = "nist_cosmetology_demo.csv";

  // Создаем содержимое файла в кодировке UTF-8
  QString content;
  content += QString::fromUtf8("Фамилия;Диагноз;Процедура;Параметры;Назначено;Выполнено\n");
  content += QString::fromUtf8("Иванова;Пигментация;Лазерное омоложение;Длина волны: 532 нм, Мощность: 15 Дж/см²;6;3\n");
  content += QString::fromUtf8("Петрова;Морщины;RF-лифтинг;Частота: 1 МГц, Температура: 42°C;5;2\n");
  content += QString::fromUtf8("Сидорова;Акне;Фототерапия;Длина волны: 415 нм, Время: 15 мин;8;5\n");
  content += QString::fromUtf8("Кузнецова;Рубцы;Фракционный лазер;Глубина: 1.5 мм, Плотность: 15%;4;1\n");
  content += QString::fromUtf8("Николаева;Купероз;Лазерная коагуляция;Длина волны: 1064 нм, Диаметр пятна: 3 мм;7;4\n");
  content += QString::fromUtf8("Смирнова;Потеря тонуса;Ультразвуковой SMAS-лифтинг;Глубина: 4.5 мм, Температура: 60°C;5;3");

  // Сохраняем файл с правильной кодировкой
  QFile file(filename);
  if (file.open(QIODevice::WriteOnly))
    {
      // Добавляем BOM для UTF-8
      QByteArray bom;
      bom.append(0xEF);
      bom.append(0xBB);
      bom.append(0xBF);
      file.write(bom);

      // Преобразуем в UTF-8 и записываем
      QTextCodec *codec = QTextCodec::codecForName("UTF-8");
      file.write(codec->fromUnicode(content));
      file.close();

      QMessageBox::information(this, "Демо файл создан",
                                QString("Файл '%1' успешно создан в кодировке UTF-8. Теперь вы можете загрузить его.").arg(filename));

      // Загружаем созданный файл
      loadDataFromCSV(filename);
    }
  else
    {
      QMessageBox::critical(this, "Ошибка", "Не удалось создать демо файл!");
    }
}

void MainWindow::loadDataFromCSV(const QString& filename)
{
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly))
    {
      QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл данных!");
      return;
    }

         // Очистка предыдущих данных
  _patientsData_vec.clear();
  _ui_po->patientsCombo->clear();
  _ui_po->dataTable->setRowCount(0);

         // Читаем весь файл
  QByteArray data = file.readAll();
  file.close();

  // Автоматическое определение кодировки
  QTextCodec::ConverterState state;
  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  QString content = codec->toUnicode(data, data.length(), &state);

  // Если были ошибки декодирования, пробуем другие кодировки
  if (state.invalidChars > 0)
    {
      codec = QTextCodec::codecForName("Windows-1251");
      content = codec->toUnicode(data);
    }

  // Разбиваем на строки
  QStringList lines = content.split('\n', Qt::SkipEmptyParts);

  // Проверяем, что есть данные
  if (lines.isEmpty())
    {
      QMessageBox::information(this, "Информация", "Файл не содержит данных");
      return;
    }

  // Обрабатываем строки
  for (int i = 0; i < lines.size(); ++i)
    {
      QString line = lines[i].trimmed();
      if (line.isEmpty() || (i == 0 && line.startsWith("Фамилия")))
        continue; // Пропускаем пустые строки и заголовок

      QStringList fields = line.split(';');
      if (fields.size() < 6)
        {
          qDebug() << "Неверный формат строки:" << line;
          continue;
        }

      // Создаем объект пациента
      bool ok1, ok2;
      int prescribed = fields[4].toInt(&ok1);
      int completed = fields[5].toInt(&ok2);

      if (!ok1 || !ok2)
        {
          qDebug() << "Ошибка в числовых полях:" << fields[4] << fields[5];
          continue;
        }

      PatientData patient(
          fields[0].trimmed(),
          fields[1].trimmed(),
          fields[2].trimmed(),
          fields[3].trimmed(),
          prescribed,
          completed
          );

      // Добавляем данные в приложение
      _patientsData_vec.append(patient);
      _ui_po->patientsCombo->addItem(patient.surname());

      int row = _ui_po->dataTable->rowCount();
      _ui_po->dataTable->insertRow(row);
      _ui_po->dataTable->setItem(row, 0, new QTableWidgetItem(patient.surname()));
      _ui_po->dataTable->setItem(row, 1, new QTableWidgetItem(patient.diagnosis()));
      _ui_po->dataTable->setItem(row, 2, new QTableWidgetItem(patient.procedure()));
      _ui_po->dataTable->setItem(row, 3, new QTableWidgetItem(patient.parameters()));
    }

  if (_patientsData_vec.isEmpty())
    {
      QMessageBox::information(this, "Информация", "Файл не содержит корректных данных");
    }
  else
    {
      _ui_po->patientsCombo->setCurrentIndex(0);
      onShowDetailsClicked();
    }
}

// Вспомогательная функция для обработки строк CSV
void MainWindow::processCSVLine(const QString& line)
{
  QStringList fields = line.split(";");

  if (fields.size() < 6) return;

  PatientData patient(
      fields[0].trimmed(),
      fields[1].trimmed(),
      fields[2].trimmed(),
      fields[3].trimmed(),
      fields[4].toInt(),
      fields[5].toInt()
      );

  _patientsData_vec.append(patient);

  // Добавление в комбобокс
  _ui_po->patientsCombo->addItem(patient.surname());

  // Добавление в таблицу
  int row = _ui_po->dataTable->rowCount();
  _ui_po->dataTable->insertRow(row);
  _ui_po->dataTable->setItem(row, 0, new QTableWidgetItem(patient.surname()));
  _ui_po->dataTable->setItem(row, 1, new QTableWidgetItem(patient.diagnosis()));
  _ui_po->dataTable->setItem(row, 2, new QTableWidgetItem(patient.procedure()));
  _ui_po->dataTable->setItem(row, 3, new QTableWidgetItem(patient.parameters()));
}


void MainWindow::onShowDetailsClicked()
{
  int index = _ui_po->patientsCombo->currentIndex();
  if (index < 0 || index >= _patientsData_vec.size()) return;

  const PatientData& patient = _patientsData_vec[index];

  // Обновление метки
  _ui_po->detailsLabel->setText(
      QString("%1: %2 процедур из %3 выполнено")
          .arg(patient.procedure())
          .arg(patient.completedCount())
          .arg(patient.prescribedCount())
      );

  // Обновление виджета прогресса
  _ui_po->progressWidget->setData(
      patient.completedCount(),
      patient.prescribedCount()
      );
}
