#include "channelhandler.h"
#include <QtDebug>

#include "../proto/proto.h"

ChannelHandler::ChannelHandler(int id, QObject *view, QObject *parent)
    : QObject(parent)
{
    this->id = id;
    this->view = view;

    QObject::connect(view,
                     SIGNAL(sig_connect()),
                     this,
                     SLOT(ConnectSlot()));
    QObject::connect(view,
                     SIGNAL(sig_start_measure()),
                     this,
                     SLOT(StartMeasureSlot()));
    QObject::connect(view,
                     SIGNAL(sig_stop_measure()),
                     this,
                     SLOT(StopMeasureSlot()));
    QObject::connect(view,
                     SIGNAL(sig_set_range(int)),
                     this,
                     SLOT(SetRangeSlot(int)));
    QObject::connect(view,
                     SIGNAL(sig_get_status()),
                     this,
                     SLOT(GetStatusSlot()));
    QObject::connect(view,
                     SIGNAL(sig_get_result()),
                     this,
                     SLOT(GetResultSlot()));
}

bool ChannelHandler::Send(const std::string &data)
{
    if(socket.state() == QLocalSocket::LocalSocketState::ConnectedState) {
        socket.readAll();
        qDebug() << "id = " << id << " Write data:" << data.c_str();
        socket.write(data.c_str(), static_cast<qint64>(data.length()));
        return true;
    } else {
        PringMsg("Disconnected");
        return false;
    }
}

std::string ChannelHandler::Read()
{
    const int BUF_LEN = 100;
    char buf[BUF_LEN];
    qint64 len = 0;
    if(socket.state() == QLocalSocket::LocalSocketState::ConnectedState) {
        const int TIMEOUT_MS = 1000;
        if(socket.waitForReadyRead(TIMEOUT_MS)) {
            len = socket.read(buf,BUF_LEN);
        } else {
            len = -1;
        }
        if(len == -1) {
            len = 0;
            PringMsg("Read error");
        }
    }
    buf[len] = '\0';
    qDebug() << "id = " << id << " Read len =" << len << " data:" << buf;
    return std::string(buf);
}

void ChannelHandler::PringMsg(const QString &msg)
{
    QObject *text_msg = view->findChild<QObject*>("text_msg");
    if(text_msg != nullptr) {
        text_msg->setProperty("text", msg);
    }
}

void ChannelHandler::ConnectSlot()
{
    QString button_text;
    if(socket.state() == QLocalSocket::LocalSocketState::ConnectedState) {
        socket.disconnectFromServer();
        PringMsg("Disconnected");
        button_text = "Connect";
    } else {
        socket.connectToServer(SERVER_NAME);
        const int TIMEOUT_MS = 1000;
        bool is_ok = socket.waitForConnected(TIMEOUT_MS);
        if(!is_ok) {
            button_text = "Connect";
            PringMsg(" connection error:" + socket.errorString());
        } else {
            button_text = "Disonnect";
            PringMsg("Connected");
        }
    }
    QObject *btn = view->findChild<QObject*>("connect_button");
    if(btn != nullptr) {
        btn->setProperty("text", button_text);
    }
}

void ChannelHandler::StartMeasureSlot()
{
    auto cmd = new proto::CommandStartMeasure(id, nullptr);
    std::string request = cmd->GetRequest();
    if(Send(request)) {
        std::string response = Read();
        auto result = cmd->CheckResponse(response);
        if(result == proto::Command::Result::Ok) {
            PringMsg("Start measure OK");
        } else {
            PringMsg("Start measure Fail");
        }
    }
}

void ChannelHandler::StopMeasureSlot()
{
    auto cmd = new proto::CommandStopMeasure(id, nullptr);
    std::string request = cmd->GetRequest();
    if(Send(request)) {
        std::string response = Read();
        auto result = cmd->CheckResponse(response);
        if(result == proto::Command::Result::Ok) {
            PringMsg("Stop measure OK");
        } else {
            PringMsg("Stop measure Fail");
        }
    }
}

void ChannelHandler::SetRangeSlot(int range)
{
    auto cmd = new proto::CommandSetRange(id, range, nullptr);
    std::string request = cmd->GetRequest();
    if(Send(request)) {
        std::string response = Read();
        auto result = cmd->CheckResponse(response, &range);
        if(result == proto::Command::Result::Ok) {
            QObject *combo_range = view->findChild<QObject*>("combo_range");
            if(combo_range != nullptr) {
                combo_range->setProperty("currentIndex", range);
            }
            PringMsg("Set range OK");
        } else {
            PringMsg("Set range Fail");
        }
    }
}

void ChannelHandler::GetStatusSlot()
{
    auto cmd = new proto::CommandGetStatus(id, nullptr);
    std::string request = cmd->GetRequest();
    if(Send(request)) {
        std::string response = Read();
        proto::CommandGetStatus::Status status;
        auto result = cmd->CheckResponse(response, &status);
        if(result == proto::Command::Result::Ok) {
            QString str_status;
            switch (status) {
            case proto::CommandGetStatus::Status::Idle:
                str_status = "Idle";
                break;
            case proto::CommandGetStatus::Status::Measure:
                str_status = "Measure";
                break;
            case proto::CommandGetStatus::Status::Busy:
                str_status = "Busy";
                break;
            case proto::CommandGetStatus::Status::Error:
                str_status = "Error";
                break;
            }
            QObject *text_status = view->findChild<QObject*>("text_status");
            if(text_status != nullptr) {
                text_status->setProperty("text", str_status);
            }
            PringMsg("Get status OK");
        } else {
            PringMsg("Get status Fail");
        }
    }
}

void ChannelHandler::GetResultSlot()
{
    auto cmd = new proto::CommandGetResult(id, nullptr);
    std::string request = cmd->GetRequest();
    if(Send(request)) {
        std::string response = Read();
        float result_val;
        auto result = cmd->CheckResponse(response, &result_val);
        if(result == proto::Command::Result::Ok) {
            QObject *text_result = view->findChild<QObject*>("text_result");
            if(text_result != nullptr) {
                text_result->setProperty("text", result_val);
            }
            PringMsg("Get result OK");
        } else {
            PringMsg("Get result Fail");
        }
    }
}





