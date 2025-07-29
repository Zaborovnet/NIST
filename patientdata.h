#ifndef PATIENTDATA_H
#define PATIENTDATA_H

#include <QString>
#include <QVector>

/**
 * @brief Класс для хранения данных о пациенте
 */
class PatientData
{
public:
  /**
   * @brief Конструктор по умолчанию
   */
  PatientData() = default;

  /**
   * @brief Конструктор с параметрами
   * @param surname Фамилия пациента
   * @param diagnosis Диагноз
   * @param procedure Название процедуры
   * @param parameters Параметры процедуры
   * @param prescribedCount Назначенное количество процедур
   * @param completedCount Выполненное количество процедур
   */
  PatientData(const QString& surname,
               const QString& diagnosis,
               const QString& procedure,
               const QString& parameters,
               int prescribedCount,
               int completedCount);

   /// Геттеры для полей класса
  QString surname() const;
  QString diagnosis() const;
  QString procedure() const;
  QString parameters() const;
  int prescribedCount() const;
  int completedCount() const;

private:
  QString _surname_o;          ///< Фамилия пациента
  QString _diagnosis_o;        ///< Диагноз
  QString _procedure_o;        ///< Название процедуры
  QString _parameters_o;       ///< Параметры процедуры
  int _prescribedCount_i = 0;  ///< Назначено процедур
  int _completedCount_i = 0;   ///< Выполнено процедур
};

#endif // PATIENTDATA_H
