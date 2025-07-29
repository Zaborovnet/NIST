#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Предварительные объявления классов
class QComboBox;
class QPushButton;
class QLabel;
class QTableWidget;
class QVBoxLayout;
class QHBoxLayout;
class PatientData;

namespace Ui {
class MainWindow;
}

/**
 * @brief Главное окно приложения
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

      public:
               /**
                * @brief Конструктор главного окна
                * @param parent Родительский виджет
                */
               explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  /**
   * @brief Обработчик нажатия кнопки "Показать детали"
   */
  void onShowDetailsClicked();

  /**
   * @brief Обработчик нажатия кнопки "Загрузить CSV"
   */
  void onLoadCSVClicked();

  /**
   * @brief Обработчик нажатия кнопки "Создать демо CSV"
   */

  void onCreateDemoCSVClicked();
  /**
   * @brief Загрузка данных из CSV файла
   * @param filename Имя файла для загрузки
   */
  void loadDataFromCSV(const QString& filename);



private:
  Ui::MainWindow* _ui_po = nullptr;          ///< Указатель на UI форму
  QVector<PatientData> _patientsData_vec;     ///< Данные о пациентах

  /**
   * @brief Попытка загрузки файла с указанной кодировкой
   * @param filename Имя файла
   * @param encoding Кодировка файла
   * @return true, если загрузка прошла успешно
   */
  bool tryLoadWithEncoding(const QString& filename, const QString& encoding);

  /**
   * @brief Обработка одной строки CSV файла
   * @param line Строка данных
   */
  void processCSVLine(const QString& line);
};

#endif // MAINWINDOW_H
