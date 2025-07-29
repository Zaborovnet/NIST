#include "patientdata.h"

PatientData::PatientData(const QString& surname,
                          const QString& diagnosis,
                          const QString& procedure,
                          const QString& parameters,
                          int prescribedCount,
                          int completedCount) :
                                                _surname_o(surname),
                                                _diagnosis_o(diagnosis),
                                                _procedure_o(procedure),
                                                _parameters_o(parameters),
                                                _prescribedCount_i(prescribedCount),
                                                _completedCount_i(completedCount)
{}

QString PatientData::surname() const {
  return _surname_o;
}

QString PatientData::diagnosis() const {
  return _diagnosis_o;
}

QString PatientData::procedure() const {
  return _procedure_o;
}

QString PatientData::parameters() const {
  return _parameters_o;
}

int PatientData::prescribedCount() const {
  return _prescribedCount_i;
}

int PatientData::completedCount() const {
  return _completedCount_i;
}
