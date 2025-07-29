#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>

class QProgressBar;
class QLabel;

/**
 * @brief Виджет для отображения прогресса выполнения процедур
 */
class ProgressWidget : public QWidget
{
  Q_OBJECT
      public:
               /**
                * @brief Конструктор виджета
                * @param parent Родительский виджет
                */
               explicit ProgressWidget(QWidget *parent = nullptr);

  /**
   * @brief Установка данных о процедурах
   * @param completed Выполнено процедур
   * @param prescribed Назначено процедур
   */
  void setData(int completed, int prescribed);

private:
  QProgressBar* _progressBar_po = nullptr; ///< Индикатор прогресса
  QLabel* _statusLabel_po = nullptr;       ///< Текстовая метка
};

#endif // PROGRESSWIDGET_H
