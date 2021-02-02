#ifndef CHANNELHANDLER_H
#define CHANNELHANDLER_H

#include <QObject>
#include <QVariant>
#include <QLocalSocket>

class ChannelHandler : public QObject
{
    Q_OBJECT
public:
    ChannelHandler(int id, QObject *view, QObject *parent = nullptr);
signals:

public slots:
  void ConnectSlot();
  void StartMeasureSlot();
  void StopMeasureSlot();
  void SetRangeSlot(int range);
  void GetStatusSlot();
  void GetResultSlot();
private:
  inline static const QString SERVER_NAME = "/tmp/sock_file";
  int id;
  QObject *view;
  QLocalSocket socket;
  bool Send(const std::string &data);
  std::string Read();
  void PringMsg(const QString &msg);
};

#endif // CHANNELHANDLER_H
