#ifndef SETUP_H
#define SETUP_H

#include <QObject>
#include <QSettings>
#include <QDataStream>
#include <QFile>
#include <QStringList>
#include <QDebug>
/*Class used to load and edit ini file
 * and to serialize and deserialize queries data
 * */
class Setup : public QObject{
      Q_OBJECT
   public:
      //Constructor
      explicit Setup(QObject *parent = nullptr);
      //Destructor
      ~Setup();
      //loads .ini settings file
      bool loadIni();
      //serializes
      bool serialize(const QStringList & queries);
      //deserializes
      bool deserialize(QStringList & queries);
      //cleans up after applicaton
      bool cleanUp();
      //setter
      void setFilePath(const QString & path);
      //getter
      QString getFilePath();

   signals:

   public slots:

   private:
      bool m_SerializeQueries();
      bool m_DeserializeQueries();
      QString m_filePath;

};

#endif // SETUP_H
